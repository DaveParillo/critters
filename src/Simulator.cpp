#include <algorithm>
#include <cassert>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <utility>
#include <unistd.h>

#include "Simulator.h"
#include "Food.h"
#include "ViewCurses.h"

using std::this_thread::sleep_for;
using std::shared_ptr;

namespace {
  std::random_device r;
  std::default_random_engine gen(r());
} // end anonymous namespace

void Simulator::set_debug(int debug_level) {
  debug = debug_level;
}
void Simulator::set_view(std::unique_ptr<View> v) {
  assert (v != nullptr);
  view = std::move(v);
  init_tiles();
}

void Simulator::start() {
  bool play = false;
  bool help = false;
  int delay = 1000;
  int count = 0;

  view->redraw(tiles);
  view->update_score(players);

  while (command != 'q')
  {
    command = view->get_key();

    if (command == 'h') {
      if (help) view->hide_help();  // hide only if already being shown
      help = !help;
    }
    if (help)                             view->show_help();
    if (command == 'p')                   play = !play;
    if (command == '-')                   delay = std::min(25000, delay + 100);
    if (command == '=' || command == '+') delay = std::max(  10, delay - 100);
    sleep_for(std::chrono::microseconds(100));

    if (play && count > delay) {
      count = 0;
      ++tick;
      update_tiles();
      view->update_time(tick);
      view->update_score(players);
      if (lone_species())                 play = false;
    }
    ++count;
  }
}

bool Simulator::lone_species() {
  auto count = 0;
  for (const auto& p : players) {
    if (p.second->alive() > 0) ++count;
  }
  return count <= 1;
}

void Simulator::update_tiles() {
  for (const auto& t : tiles) {
    if (t.second->is_player()) {
      update(t.first, t.second);
    }
  }
  for (const auto& t : tiles) {
    t.second->set_update(false);
  }
  view->redraw(tiles);
}

void Simulator::init_tiles() {
  assert(tiles.empty());
  if (debug != 0) std::cerr << "height: " << view->height() << ", width: " << view->width() << "\n";
  for (int16_t x=0; x<=view->width()-1; ++x) {
    for (int16_t y=0; y<=view->height()-1; ++y) {
      Point p = {x, y};
      tiles[p] = blank_tile;
      blanks[p] = blank_tile;
    }
  }
}

void Simulator::update (const Point& p, const shared_ptr<Critter>& it) {
  if (it->updated()) return;

  it->set_update(true);
  it->tick();  // update critter state variables
  if (it->food_remaining() == 0) {
    players[it->name()]->add_starved();
    if (debug != 0) std::cerr << it->name() << " @ " << p << " starved to death.\n";
    tiles[p] = blank_tile;
    view->draw(p, tiles[p]);
  } else if (it->is_asleep()) {
    if (debug != 0) std::cerr << it->name() << " @ " << p << " is asleep.\n";
    // can't initiate any actions
    view->draw(p, tiles[p]);
  }  else if (it->is_mating()) {
    if (debug != 0) std::cerr << it->name() << " @ " << p << " is mating.\n";
    view->draw(p, tiles[p]);
  } else {


    auto neighbors = get_neighbors(p);
    auto move_dir = it->move(neighbors);
    if (move_dir < Direction::CENTER || move_dir > Direction::NORTH_WEST) move_dir = Direction::CENTER;
    if (can_move(p, it, move_dir)) {
      move (p, it, move_dir);
    } else {
      if (move_dir != Direction::CENTER) {
        auto dest = p.translate(p, move_dir, view->width(),view->height());
        take_action(p, it, dest);
      } else {
        view->draw(p, tiles[p]);
      }
    }
  }


}


bool Simulator::can_move (const Point& p, const shared_ptr<Critter>& it, const Direction& move_dir) {
  if (it->wait_remaining() != 0u)    return false;
  if (move_dir == Direction::CENTER) return false;

  if (debug != 0) std::cerr << "Can " << it->name() << " at " << p <<  " move?\n";

  auto tmp =  p.translate(p, move_dir, view->width(),view->height());
  return tiles[tmp] == blank_tile;
}

void Simulator::move (const Point& p, const shared_ptr<Critter>& it, const Direction& move_dir) {
  if (debug != 0) std::cerr << "Get move from  " << it->name() << "\n";
  auto new_posit = p.translate(p, move_dir, view->width(),view->height());
  assert(new_posit != p);
  if ("Stone" != tiles[new_posit]->name()) {
    if (debug != 0) std::cerr << it->name() << " moving to " << new_posit << "\n";
    std::swap(tiles[p], tiles[new_posit]);
  }
  view->draw(new_posit, tiles[new_posit]);
  view->draw(p, tiles[p]);
}


void Simulator::move (const Point& src, const Point& dest) {
  assert (src != dest);
  assert (tiles[dest] == blank_tile || tiles[dest]->name() == "Food");
  if (debug != 0) std::cerr << "moving from " << src << " to " << dest << "\n";
  std::swap(tiles[src], tiles[dest]);
  view->draw(src, tiles[src]);
  view->draw(dest, tiles[dest]);
}


std::map<Direction,  shared_ptr<Critter>>
Simulator::get_neighbors(const Point& p) {
  std::map<Direction, shared_ptr<Critter>> neighbors;

  for (auto& dir: directions) {
    auto new_posit = p.translate(p, dir, view->width(),view->height());
    neighbors[dir] = tiles[new_posit];
    //if (debug != 0) std::cerr << DirectionNames[dir] << ", "
    //    << tiles[new_posit]->name() << " @ "
    //        << new_posit << ";\t";

  }
  //if (debug != 0) std::cerr << "\n";
  assert (neighbors.size() == 8);
  return neighbors;
}

void Simulator::take_action (const Point& src, shared_ptr<Critter> src_it, const Point& dest) {
  if (debug != 0) std::cerr << "taking action for " << src_it->name() << " at " << src << "\n";

  auto dest_it = tiles[dest];
  if ("Stone" == dest_it->name()) {
    if (debug != 0) std::cerr << src_it->name() << " at " << src << " tried to fight a " << dest_it->name() << "\n";
    src_it->sleep(20);
    src_it->sleep();  // inform critter we put it to sleep
  } else if ("Food" == dest_it->name()) {
    process_food(src, src_it, dest);
  } else if (can_mate(src_it, dest_it)) {
    process_mate(src, src_it, dest, dest_it);
  } else if (src_it->name() == dest_it->name()) {
    // same species, let the mating begin...
    if (debug != 0) std::cerr << src_it->name() << " at " << src << " is mating with "
      << dest_it->name() <<  " at " << dest << "\n";
  } else {
    // 2 different species always fight
    if (can_fight(src, src_it, dest, dest_it)) {
      if (debug != 0) std::cerr << src_it->name() << " at " << src << " is fighting " << dest_it->name() << "\n";
      if (debug != 0) std::cerr << dest_it->name() << " is a player? " << dest_it->is_player() << "\n";

      process_fight(src, src_it, dest, dest_it);
    }
  }

}

void Simulator::process_food(const Point& src,  shared_ptr<Critter> src_it, const Point& dest)   {
  if (src_it->eat()) {
    if (debug != 0) std::cerr << src_it->name() << " at " << src << " is eating and has "
      << src_it->food_remaining() << " food remaining.\n";
    src_it->eat_food();
    players[src_it->name()]->add_feeding();

    if (debug && src_it->is_asleep()) {
      if (debug != 0) std::cerr << src_it->name() << " at " << src << " has eaten and has "
        << src_it->wait_remaining() << " wait remaining.\n";
    }
    tiles[dest] = blank_tile;
    move(src,dest);
    // make more food somewhere else
    auto food = std::next(std::begin(tiles), std::uniform_int_distribution<int> {0, int(tiles.size())} (gen));
    auto p = std::get<0>(*food);
    if (tiles[p] == blank_tile) {
      tiles[p] = std::make_shared<Food>();
      view->draw(p, tiles[p]);
    }
  }

}

bool Simulator::can_mate (const shared_ptr<Critter>& src_it, const shared_ptr<Critter>& dest_it)  {
  if (src_it->name() != dest_it->name()) return false;
  if (src_it->is_baby() || dest_it->is_baby()) return false;

  return (!src_it->is_parent() && !dest_it->is_parent() && !dest_it->is_asleep() && !dest_it->is_mating());

}

void Simulator::process_mate(const Point& src,  shared_ptr<Critter> src_it, const Point& dest, shared_ptr<Critter> dest_it)   {

  auto neighbors = get_neighbors(src);
  Direction dir = Direction::CENTER;
  // find empty neightbor to put baby
  for (int i=0; i<8; ++i) {
    if ("Empty" == neighbors[directions[i]]->name()) {
      dir = directions[i];
    }
  }
  if (dir == Direction::CENTER) {
    if(debug != 0)    std::cerr << "Could not find a place to have baby.\n";
  } else {
    auto b_dest = src.translate(src, dir, view->width(),view->height());
    auto baby = src_it->create();
    players[baby->name()]->add_member();
    src_it->start_mating(30);
    dest_it->start_mating(30);

    tiles[b_dest] = baby;
    view->draw(b_dest, baby);
    view->draw(src, tiles[src]);
    view->draw(dest, tiles[dest]);
    if(debug != 0)    std::cerr << src_it->name() << " made baby. The baby is at: " << b_dest << "\n";
  }
}

bool Simulator::can_fight (const Point& src,  const shared_ptr<Critter>& src_it,
                           const Point& dest, const shared_ptr<Critter>& dest_it)  {
  if (src_it->wait_remaining() > 0) return false;

  if (dest_it == blank_tile) return false;

  if (debug != 0) std::cerr << " can fight thinks the opponent is a " << dest_it->name() << " at " << dest << "\n";
  if (debug != 0) std::cerr << " current player is a " << src_it->name() << " at " << src << "\n";
  if (debug != 0) std::cerr << " is player? " << dest_it->is_player() << "\n";

  return dest_it->is_player();
}

void Simulator::process_fight(const Point& src,  shared_ptr<Critter> src_it,
    const Point& dest, shared_ptr<Critter> dest_it)   {
  auto results = fight_results(src_it, dest_it);

  if(!dest_it->is_player()) {
    std::cerr << "Error! fighting a non-player entitiy\n";
    std::cerr << "\tName: " << dest_it->name() << " @" << dest_it << "\n";
    std::cerr << "\tSource pt: " << src << ", Dest pt: " << dest << "\n";
    std::cerr << "\tBlank_tile: " << dest_it->name() << " @" << blank_tile << "\n";

  }

  if (debug != 0) std::cerr << " process fight thinks the opponent is a " << tiles[dest]->name() << "\n";

  //if the attacker wins, move to the dest tile
  //if the attacker loses, it is removed & the defender stays put.
  //On a draw, nothing else happens

  if (results == Simulator::AttackResults::ATTACKER) {
    if (debug != 0) std::cerr << src_it->name() << " won \n";
    tiles[dest] = blank_tile;
    view->draw(dest, tiles[dest]);
    if (debug != 0) std::cerr << "move after victory \n";
    move(src,dest);
    update_kill_stats(src_it, dest_it);
  } else if (results == Simulator::AttackResults::DEFENDER) {
    if (debug != 0) std::cerr << dest_it->name() << " won \n";
    tiles[src] = blank_tile;
    view->draw(src, tiles[src]);
    update_kill_stats(dest_it, src_it);
  } else {
    if (debug != 0) std::cerr << "fight was a draw \n";
    src_it->draw();   // report back to attacker
    dest_it->draw();  // and defender
  }
}

Simulator::AttackResults
Simulator::fight_results (shared_ptr<Critter> attacker,
    shared_ptr<Critter> defender) {
  if (defender->is_asleep() || defender->is_mating()) {
    return Simulator::AttackResults::ATTACKER;
  }
  using Attack = Critter::Attack;
  auto a_attack = attacker->fight(defender->name());
  auto d_attack = defender->fight(attacker->name());
  if (a_attack < Attack::ROAR || a_attack > Attack::SCRATCH) a_attack = Attack::FORFEIT;
  if (d_attack < Attack::ROAR || d_attack > Attack::SCRATCH) d_attack = Attack::FORFEIT;

  if (a_attack == d_attack) {
    return Simulator::AttackResults::DRAW;
  }

  if ((a_attack == Critter::Attack::ROAR    && d_attack == Critter::Attack::SCRATCH) ||
      (a_attack == Critter::Attack::POUNCE  && d_attack == Critter::Attack::ROAR) ||
      (a_attack == Critter::Attack::SCRATCH && d_attack == Critter::Attack::POUNCE) ||
       d_attack == Critter::Attack::FORFEIT) {
    return Simulator::AttackResults::ATTACKER;
  }

  return Simulator::AttackResults::DEFENDER;
}

void Simulator::update_kill_stats(shared_ptr<Critter> winner, shared_ptr<Critter> loser) {
  players[winner->name()]->add_kill();
  if (debug != 0) std::cerr << winner->name() << " gets a kill \n";
  if (debug != 0) std::cerr << players[winner->name()];
  players[loser->name()]->kill();
  if (debug != 0) std::cerr << loser->name() << " is dead \n";
  if (debug != 0) std::cerr << players[loser->name()];

  winner->won();            // report status
  loser->lost();            // report status
}

void Simulator::addItem(shared_ptr<Critter> item, const int num_items) {
  assert(item != nullptr);
  typedef std::unordered_map<Point,shared_ptr<Critter>>::value_type tile_type;

  int free = count_if(tiles.begin(), tiles.end(), 
      [&](tile_type t) {
        return t.second == this->blank_tile;
      });
  if (free < num_items) {
    view->teardown();
    std::cerr << "Not enough blank tiles remaining to add " 
              << num_items << ' ' << item->name() << std::endl;
    std::cerr << "Try reducing critters, food, or stones, or increasing x and y.\n\n";
    std::cerr << "Exiting.\n\n";
    exit(-1);
  }

  for (auto i = 0; i < num_items; ++i) {
    auto c = item->create();
    Point p = get_random_blank_tile();
    assert(tiles[p] == blank_tile);
    tiles[p] = c;
    blanks.erase(p);
    view->draw(p, c);
  }

  //add item to species set
  if (item->is_player()) {
    players[item->name()] = std::make_shared<Species>(item->name(), num_items);
  }
}

Point Simulator::get_random_blank_tile() {
  auto random_it = std::next(std::begin(blanks), std::uniform_int_distribution<int> {0, int(blanks.size())} (gen));
  return std::get<0>(*random_it);
}


