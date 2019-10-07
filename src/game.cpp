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
    if (help)                             view_->show_help();
    if (command_ == 'p')                   play = !play;
    if (command_ == '-')                   delay = std::min(25000, delay + 100);
    if (command_ == '=' || command_ == '+') delay = std::max(  10, delay - 100);
    sleep_for(std::chrono::microseconds(100));

    if (play && count > delay) {
      count = 0;
      ++tick_;
      update_tiles();
      view_->update_time(tick_);
      view_->update_score(players_);
      if (lone_species())                 play = false;
    }
    ++count;
  }
}

bool game::lone_species() {
  auto count = 0;
  for (const auto& p : players_) {
    if (p.second->alive() > 0) ++count;
  }
  return count <= 1;
}

void game::update_tiles() {
  for (const auto& t : tiles_) {
    if (t.second->is_player()) {
      update(t.first, t.second);
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

void game::update (const point& p, const shared_ptr<critter>& it) {
  if (it->updated()) return;

  it->set_update(true);
  it->tick();  // update critter state variables
  if (it->food_remaining() == 0) {
    players_[it->name()]->add_starved();
    if (debug_ != 0) std::cerr << it->name() << " @ " << p << " starved to death.\n";
    tiles_[p] = blank_tile;
    view_->draw(p, tiles_[p]);
  } else if (it->is_asleep()) {
    if (debug_ != 0) std::cerr << it->name() << " @ " << p << " is asleep.\n";
    // can't initiate any actions
    view_->draw(p, tiles_[p]);
  }  else if (it->is_mating()) {
    if (debug_ != 0) std::cerr << it->name() << " @ " << p << " is mating.\n";
    view_->draw(p, tiles_[p]);
  } else {


    auto neighbors = get_neighbors(p);
    auto move_dir = it->move(neighbors);
    if (move_dir < direction::CENTER || move_dir > direction::NORTH_WEST) move_dir = direction::CENTER;
    if (can_move(p, it, move_dir)) {
      move (p, it, move_dir);
    } else {
      if (move_dir != direction::CENTER) {
        auto dest = p.translate(p, move_dir, view_->width(),view_->height());
        take_action(p, it, dest);
      } else {
        view_->draw(p, tiles_[p]);
      }
    }
  }


}


bool game::can_move (const point& p, const shared_ptr<critter>& it, const direction& move_dir) {
  if (it->wait_remaining() != 0u)    return false;
  if (move_dir == direction::CENTER) return false;

  if (debug_ != 0) std::cerr << "Can " << it->name() << " at " << p <<  " move?\n";

  auto tmp =  p.translate(p, move_dir, view_->width(),view_->height());
  return tiles_[tmp] == blank_tile;
}

void game::move (const point& p, const shared_ptr<critter>& it, const direction& move_dir) {
  if (debug_ != 0) std::cerr << "Get move from  " << it->name() << "\n";
  auto new_posit = p.translate(p, move_dir, view_->width(),view_->height());
  assert(new_posit != p);
  if ("Stone" != tiles_[new_posit]->name()) {
    if (debug_ != 0) std::cerr << it->name() << " moving to " << new_posit << "\n";
    std::swap(tiles_[p], tiles_[new_posit]);
  }
  view_->draw(new_posit, tiles_[new_posit]);
  view_->draw(p, tiles_[p]);
}


void game::move (const point& src, const point& dest) {
  assert (src != dest);
  assert (tiles_[dest] == blank_tile || tiles_[dest]->name() == "Food");
  if (debug_ != 0) std::cerr << "moving from " << src << " to " << dest << "\n";
  std::swap(tiles_[src], tiles_[dest]);
  view_->draw(src, tiles_[src]);
  view_->draw(dest, tiles_[dest]);
}


std::map<direction,  shared_ptr<critter>>
game::get_neighbors(const point& p) {
  std::map<direction, shared_ptr<critter>> neighbors;

  for (auto& dir: directions) {
    auto new_posit = p.translate(p, dir, view_->width(),view_->height());
    neighbors[dir] = tiles_[new_posit];
    //if (debug != 0) std::cerr << DirectionNames[dir] << ", "
    //    << tiles[new_posit]->name() << " @ "
    //        << new_posit << ";\t";

  }
  //if (debug != 0) std::cerr << "\n";
  assert (neighbors.size() == 8);
  return neighbors;
}

void game::take_action (const point& src, shared_ptr<critter> src_it, const point& dest) {
  if (debug_ != 0) std::cerr << "taking action for " << src_it->name() << " at " << src << "\n";

  auto dest_it = tiles_[dest];
  if ("Stone" == dest_it->name()) {
    if (debug_ != 0) std::cerr << src_it->name() << " at " << src << " tried to fight a " << dest_it->name() << "\n";
    src_it->sleep(20);
    src_it->sleep();  // inform critter we put it to sleep
  } else if ("Food" == dest_it->name()) {
    process_food(src, src_it, dest);
  } else if (can_mate(src_it, dest_it)) {
    process_mate(src, src_it, dest, dest_it);
  } else if (src_it->name() == dest_it->name()) {
    // same species, let the mating begin...
    if (debug_ != 0) std::cerr << src_it->name() << " at " << src << " is mating with "
      << dest_it->name() <<  " at " << dest << "\n";
  } else {
    // 2 different species always fight
    if (can_fight(src, src_it, dest, dest_it)) {
      if (debug_ != 0) std::cerr << src_it->name() << " at " << src << " is fighting " << dest_it->name() << "\n";
      if (debug_ != 0) std::cerr << dest_it->name() << " is a player? " << dest_it->is_player() << "\n";

      process_fight(src, src_it, dest, dest_it);
    }
  }

}

void game::process_food(const point& src,  shared_ptr<critter> src_it, const point& dest)   {
  if (src_it->eat()) {
    if (debug_ != 0) std::cerr << src_it->name() << " at " << src << " is eating and has "
      << src_it->food_remaining() << " food remaining.\n";
    src_it->eat_food();
    players_[src_it->name()]->add_feeding();

    if (debug_ && src_it->is_asleep()) {
      if (debug_ != 0) std::cerr << src_it->name() << " at " << src << " has eaten and has "
        << src_it->wait_remaining() << " wait remaining.\n";
    }
    tiles_[dest] = blank_tile;
    move(src,dest);
    // make more food somewhere else
    auto food_spot = std::next(std::begin(tiles_), std::uniform_int_distribution<int> {0, int(tiles_.size())} (gen));
    auto p = std::get<0>(*food_spot);
    if (tiles_[p] == blank_tile) {
      tiles_[p] = std::make_shared<food>();
      view_->draw(p, tiles_[p]);
    }
  }

}

bool game::can_mate (const shared_ptr<critter>& src_it, const shared_ptr<critter>& dest_it)  {
  if (src_it->name() != dest_it->name()) return false;
  if (src_it->is_baby() || dest_it->is_baby()) return false;

  return (!src_it->is_parent() && !dest_it->is_parent() && !dest_it->is_asleep() && !dest_it->is_mating());

}

void game::process_mate(const point& src,  shared_ptr<critter> src_it, const point& dest, shared_ptr<critter> dest_it)   {

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
    auto b_dest = src.translate(src, dir, view_->width(),view_->height());
    auto baby = src_it->create();
    players_[baby->name()]->add_member();
    src_it->start_mating(30);
    dest_it->start_mating(30);

    tiles_[b_dest] = baby;
    view_->draw(b_dest, baby);
    view_->draw(src, tiles_[src]);
    view_->draw(dest, tiles_[dest]);
    if(debug_ != 0)    std::cerr << src_it->name() << " made baby. The baby is at: " << b_dest << "\n";
  }
}

bool game::can_fight (const point& src,  const shared_ptr<critter>& src_it,
                           const point& dest, const shared_ptr<critter>& dest_it)  {
  if (src_it->wait_remaining() > 0) return false;

  if (dest_it == blank_tile) return false;

  if (debug_ != 0) std::cerr << " can fight thinks the opponent is a " << dest_it->name() << " at " << dest << "\n";
  if (debug_ != 0) std::cerr << " current player is a " << src_it->name() << " at " << src << "\n";
  if (debug_ != 0) std::cerr << " is player? " << dest_it->is_player() << "\n";

  return dest_it->is_player();
}

void game::process_fight(const point& src,  shared_ptr<critter> src_it,
    const point& dest, shared_ptr<critter> dest_it)   {
  auto results = get_fight_results(src_it, dest_it);

  if(!dest_it->is_player()) {
    std::cerr << "Error! fighting a non-player entitiy\n";
    std::cerr << "\tName: " << dest_it->name() << " @" << dest_it << "\n";
    std::cerr << "\tSource pt: " << src << ", Dest pt: " << dest << "\n";
    std::cerr << "\tBlank_tile: " << dest_it->name() << " @" << blank_tile << "\n";

  }

  if (debug_ != 0) std::cerr << " process fight thinks the opponent is a " << tiles_[dest]->name() << "\n";

  //if the attacker wins, move to the dest tile
  //if the attacker loses, it is removed & the defender stays put.
  //On a draw, nothing else happens

  if (results == game::fight_results::ATTACKER) {
    if (debug_ != 0) std::cerr << src_it->name() << " won \n";
    tiles_[dest] = blank_tile;
    view_->draw(dest, tiles_[dest]);
    if (debug_ != 0) std::cerr << "move after victory \n";
    move(src,dest);
    update_kill_stats(src_it, dest_it);
  } else if (results == game::fight_results::DEFENDER) {
    if (debug_ != 0) std::cerr << dest_it->name() << " won \n";
    tiles_[src] = blank_tile;
    view_->draw(src, tiles_[src]);
    update_kill_stats(dest_it, src_it);
  } else {
    if (debug_ != 0) std::cerr << "fight was a draw \n";
    src_it->draw();   // report back to attacker
    dest_it->draw();  // and defender
  }
}

game::fight_results
game::get_fight_results (shared_ptr<critter> attacker,
    shared_ptr<critter> defender) {
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

void game::update_kill_stats(shared_ptr<critter> winner, shared_ptr<critter> loser) {
  players_[winner->name()]->add_kill();
  if (debug_ != 0) std::cerr << winner->name() << " gets a kill \n";
  if (debug_ != 0) std::cerr << players_[winner->name()];
  players_[loser->name()]->kill();
  if (debug_ != 0) std::cerr << loser->name() << " is dead \n";
  if (debug_ != 0) std::cerr << players_[loser->name()];

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
    point p = get_random_blank_tile();
    assert(tiles_[p] == blank_tile);
    tiles_[p] = c;
    blanks_.erase(p);
    view_->draw(p, c);
  }

  //add item to species set
  if (item->is_player()) {
    players_[item->name()] = std::make_shared<species>(item->name(), num_items);
  }
}

point game::get_random_blank_tile() {
  auto random_it = std::next(std::begin(blanks_), std::uniform_int_distribution<int> {0, int(blanks_.size()-1)} (gen));
  return std::get<0>(*random_it);
}


