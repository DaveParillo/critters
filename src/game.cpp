#include <algorithm>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <thread>
#include <utility>
#include <unistd.h>

#include "game.h"
#include "food.h"
#include "view.h"
#include "view_curses.h"

using std::this_thread::sleep_for;
using std::shared_ptr;

// a single location in the critters world
using tile = std::pair<point,  std::shared_ptr<critter>>;

namespace {
  std::random_device r;
  std::default_random_engine gen(r());
} // end anonymous namespace

void game::set_debug(int debug_level) {
  debug_ = debug_level;
}
void game::set_view(std::unique_ptr<view> v) {
  assert (v != nullptr);
  view_ = std::move(v);
  init_tiles();
}

void game::start() {
  bool play = false;
  bool help = false;
  int delay = 1000;
  int count = 0;

  view_->redraw(tiles_);
  view_->update_score(players_);

  while (command_ != 'q')
  {
    command_ = view_->get_key();

    if (command_ == 'h') {
      if (help) view_->hide_help();  // hide only if already being shown
      help = !help;
    }
    if (help)                               view_->show_help();
    if (command_ == 'p')                    play = !play;
    if (command_ == '-')                    delay = std::min(25000, delay + 100);
    if (command_ == '=' || command_ == '+') delay = std::max(  10, delay - 100);
    sleep_for(std::chrono::microseconds(100));

    if (play && count > delay) {
      count = 0;
      ++tick_;
      update_tiles();
      view_->update_time(tick_);
      view_->update_score(players_);
      // one specied left standing
      auto alone = [this]() {
        auto count = 0;
        for (const auto& p : players_) {
          if (p.second->alive() > 0) ++count;
        }
        return count <= 1;
      };
      // if (alone())                 play = false;
    }
    ++count;
  }
  view_->teardown();
}

void game::update_tiles() {
  for (const auto& t : tiles_) {
    if (t.second->is_player()) {
      update(t);
    }
  }
  for (const auto& t : tiles_) {
    t.second->set_update(false);
  }
  view_->redraw(tiles_);
}

void game::init_tiles() {
  assert(tiles_.empty());
  if (debug_ != 0) std::cerr << "height: " << view_->height() << ", width: " << view_->width() << "\n";
  for (int16_t x=0; x<=view_->width()-1; ++x) {
    for (int16_t y=0; y<=view_->height()-1; ++y) {
      point p = {x, y};
      tiles_[p] = blank_tile;
      blanks_[p] = blank_tile;
    }
  }
}

void game::update (const tile& t) {
  const auto& pos = t.first;
  const auto& it = t.second;
  if (it->updated()) return;

  it->set_update(true);
  it->tick();  // update critter state variables

  if (it->food_remaining() == 0) {
    players_[it->name()]->add_starved();
    tiles_[pos] = blank_tile;
    view_->draw(pos, *tiles_[pos]);
  } else if (it->is_asleep() || it->is_mating()) {
    view_->draw(pos, *it);
  } else {

    auto neighbors = get_neighbors(pos);
    auto move_dir = it->move(neighbors);
    if (move_dir <= direction::CENTER ||
        move_dir > direction::NORTH_WEST) {
      view_->draw(pos, *it);
      return;
    }

    auto can_move = [&pos, &it, &move_dir, this]() {
      if (it->wait_remaining() != 0u)    return false;
      auto tmp =  pos.translate(pos, move_dir, view_->width(),view_->height());
      return tiles_[tmp] == blank_tile;
    };

    auto dest = pos.translate(pos, move_dir, view_->width(),view_->height());
    if (can_move()) {
      move(pos, dest);
    } else {
      take_action(pos, dest);
    }
    view_->draw(pos, *tiles_[pos]);
    view_->draw(dest, *tiles_[dest]);
  }
}

void game::move (const point& src, const point& dest) {
  assert (src != dest);
  assert (tiles_[dest] == blank_tile || tiles_[dest]->name() == "Food");
  assert (tiles_[dest]->name() != "Stone");
  std::swap(tiles_[src], tiles_[dest]);
  // view_->draw(src, *tiles_[src]);
  // view_->draw(dest, *tiles_[dest]);
}


std::map<direction,  shared_ptr<critter>>
game::get_neighbors(const point& p) {
  std::map<direction, shared_ptr<critter>> neighbors;
  for (auto& dir: directions) {
    auto new_posit = p.translate(p, dir, view_->width(),view_->height());
    neighbors[dir] = tiles_[new_posit];
  }
  assert (neighbors.size() == 8);
  return neighbors;
}

void game::take_action (const point& src, const point& dest) {
  auto me = tiles_[src];
  auto other = tiles_[dest];
  auto can_mate = [&me, &other]() {
    return (me->name() == other->name())
      && !me->is_baby() 
      && !other->is_baby() 
      && !me->is_parent() 
      && !other->is_parent() 
      && !other->is_asleep() 
      && !other->is_mating();
  };

  if ("Stone" == other->name()) {
    if (debug_ != 0) std::cerr << me->name() << " at " << src << " tried to fight a stone. sleep it off.\n";
    me->sleep(20);
    me->sleep();  // inform critter we put it to sleep
  } else if ("Food" == other->name()) {
    process_food(src, dest);
  } else if (me->name() == other->name()) {
    // 2 adult same species members can mate once
    if (can_mate()) {
      process_mate(src, dest);
    }
  } else {
    // 2 different species always fight

    auto can_fight = [&src, &dest, this]() {
      auto other =  tiles_[dest];
      if(tiles_[src]->wait_remaining() > 0 || other == blank_tile) {
        return false;
      }
      return other->is_player();
    };

    if (can_fight()) {
      process_fight(src, dest);
    }
  }
}

void game::process_food(const point& src, const point& dest)   {
  auto src_it = tiles_[src];
  if (src_it->eat()) {
    src_it->eat_food();
    players_[src_it->name()]->add_feeding();

    tiles_[dest] = blank_tile;
    move(src,dest);
    // make more food somewhere else
    auto food_spot = [&, this]() {
      auto random_it = std::next(std::begin(tiles_), std::uniform_int_distribution<int> {0, int(tiles_.size()-1)} (gen));
      return std::get<0>(*random_it);
    };
    auto p = food_spot();
    while (tiles_[p] != blank_tile) {
      p = food_spot();
    }
    tiles_[p] = std::make_shared<food>();
    view_->draw(p, *tiles_[p]);
  }
}

void game::process_mate(const point& src, const point& dest)   {
  auto dad = tiles_[src];
  auto mom = tiles_[dest];

  auto neighbors = get_neighbors(src);
  direction dir = direction::CENTER;
  // find empty neightbor to put baby
  for (int i=0; i<8; ++i) {
    if ("Empty" == neighbors[directions[i]]->name()) {
      dir = directions[i];
    }
  }
  if (dir == direction::CENTER) {
    if(debug_ != 0)    std::cerr << "Could not find a place to have baby.\n";
  } else {
    auto birthplace = src.translate(src, dir, view_->width(),view_->height());
    auto baby = mom->create();
    players_[baby->name()]->add_member();
    mom->start_mating(9);
    dad->start_mating(9);

    tiles_[birthplace] = baby;
    view_->draw(birthplace, *baby);
    view_->draw(src, *mom);
    view_->draw(dest, *dad);
    if(debug_ != 0)    std::cerr << mom->name() << " made baby. The baby is at: " << birthplace << "\n";
  }
}

void game::process_fight(const point& src, const point& dest)   {
  auto attacker = tiles_[src];
  auto defender = tiles_[dest];
  auto results = get_fight_results(&*attacker, &*defender);

  if(!defender->is_player()) {
    std::cerr << "Error! fighting a non-player entitiy\n";
    std::cerr << "\tName: " << defender->name() << " @" << defender << "\n";
    std::cerr << "\tSource pt: " << src << ", Dest pt: " << dest << "\n";
    std::cerr << "\tBlank_tile: " << defender->name() << " @" << blank_tile << "\n";
  }

  //if the attacker wins, move to the dest tile
  //if the attacker loses, it is removed & the defender stays put.
  //On a draw, nothing else happens

  if (results == game::fight_results::ATTACKER) {
    tiles_[dest] = blank_tile;
    view_->draw(dest, *tiles_[dest]);
    move(src,dest);
    update_kill_stats(&*attacker, &*defender);
  } else if (results == game::fight_results::DEFENDER) {
    tiles_[src] = blank_tile;
    // view_->draw(src, *tiles_[src]);
    update_kill_stats(&*defender, &*attacker);
  } else {
    attacker->draw();   // report back to attacker
    defender->draw();  // and defender
  }
}

game::fight_results
game::get_fight_results (critter* attacker, critter* defender) {
  if (defender->is_asleep() || defender->is_mating()) {
    return game::fight_results::ATTACKER;
  }
  using Attack = critter::attack;
  auto a_attack = attacker->fight(defender->name());
  auto d_attack = defender->fight(attacker->name());
  if (a_attack < Attack::ROAR || a_attack > Attack::SCRATCH) a_attack = Attack::FORFEIT;
  if (d_attack < Attack::ROAR || d_attack > Attack::SCRATCH) d_attack = Attack::FORFEIT;

  if (a_attack == d_attack) {
    return game::fight_results::DRAW;
  }

  if ((a_attack == critter::attack::ROAR    && d_attack == critter::attack::SCRATCH) ||
      (a_attack == critter::attack::POUNCE  && d_attack == critter::attack::ROAR) ||
      (a_attack == critter::attack::SCRATCH && d_attack == critter::attack::POUNCE) ||
       d_attack == critter::attack::FORFEIT) {
    return game::fight_results::ATTACKER;
  }

  return game::fight_results::DEFENDER;
}

void game::update_kill_stats(critter* winner, critter* loser) {
  players_[winner->name()]->add_kill();
  players_[loser->name()]->kill();
  winner->won();            // report status
  loser->lost();            // report status
}

void game::add_item(shared_ptr<critter> item, const int num_items) {
  assert(item != nullptr);
  typedef std::unordered_map<point,shared_ptr<critter>>::value_type tile_type;

  int free = count_if(tiles_.begin(), tiles_.end(), 
      [&](tile_type t) {
        return t.second == this->blank_tile;
      });
  if (free < num_items) {
    view_->teardown();
    std::cerr << "Not enough blank tiles remaining to add " 
              << num_items << ' ' << item->name() << std::endl;
    std::cerr << "Try reducing critters, food, or stones, or increasing x and y.\n\n";
    std::cerr << "Exiting.\n\n";
    exit(-1);
  }

  for (auto i = 0; i < num_items; ++i) {
    auto c = item->create();
    auto next_point = [&, this]() {
      auto random_it = std::next(std::begin(blanks_), std::uniform_int_distribution<int> {0, int(blanks_.size()-1)} (gen));
      return std::get<0>(*random_it);
    };
    point p = next_point();
    assert(tiles_[p] == blank_tile);
    tiles_[p] = c;
    blanks_.erase(p);
    view_->draw(p, *c);
  }

  //add item to species set
  if (item->is_player()) {
    players_[item->name()] = std::make_shared<species>(item->name(), num_items);
  }
}


