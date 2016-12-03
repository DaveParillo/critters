#include <algorithm>
#include <cassert>
#include <iostream>
#include <map>
#include <unordered_map>
#include <memory>
#include <random>
#include <string>
#include <utility>
#include <unistd.h>

#include "Simulator.h"
#include "Critter.h"
#include "Point.h"
#include "Food.h"
#include "Species.h"
#include "ViewCurses.h"

using std::shared_ptr;

Simulator::Simulator() :
  command('x'), debug(false), tick(0) {
    srand(time(0));
  }

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
  int delay = 350;
  int count = 0;

  view->redraw(tiles);

  while (command != 'q')
  {
    command = view->get_key();

    if (command == 'h') {
      if (help) view->hide_help();  // hide only if already being shown
      help = !help;
    }
    if (help)                             view->show_help();
    if (command == 'p')                   play = !play;
    if (command == '-')                   delay = std::min(1250, delay + 50);
    if (command == '=' || command == '+') delay = std::max(  50, delay - 50);
    usleep(1000);

    if (play && count > delay) {
      count = 0;
      tick++;
      update_tiles();
      view->update_time(tick);
      view->update_score(players);
      if (lone_species())                 play = false;
    }
    count += 1;
  }

}

bool Simulator::lone_species() {
  auto count = 0;
  for (auto p : players) {
    if (p.second->alive() > 0) count++;
  }
  return count <= 1;
}

void Simulator::update_tiles() {
  for (auto t : tiles) {
    if (t.second->is_player()) {
      update(t.first, t.second);
    }
  }
  for (auto t : tiles) {
    t.second->set_update(false);
  }
  view->redraw(tiles);
}

void Simulator::init_tiles() {
  assert(tiles.empty());
  if (debug) std::cerr << "height: " << view->height() << ", width: " << view->width() << "\n";
  for (int x=0; x<=view->width()-1; x++) {
    for (int y=0; y<=view->height()-1; y++) {
      Point p = {x, y};
      tiles[p] = blank_tile;
      blanks[p] = blank_tile;
    }
  }
}

void Simulator::update (const Point& p, shared_ptr<Critter> it) {
  if (it->updated()) return;

  it->set_update(true);
  it->tick();  // update critter state variables
  if (it->food_remaining() == 0) {
    players[it->name()]->add_starved();
    if (debug) std::cerr << it->name() << " @ " << p << " starved to death.\n";
    tiles[p] = blank_tile;
    view->draw(p, tiles[p]);
  } else if (it->is_asleep()) {
    if (debug) std::cerr << it->name() << " @ " << p << " is asleep.\n";
    // can't initiate any actions
    view->draw(p, tiles[p]);
  }  else if (it->is_mating()) {
    if (debug) std::cerr << it->name() << " @ " << p << " is mating.\n";
    view->draw(p, tiles[p]);
  } else {


    auto neighbors = get_neighbors(p);
    auto move_dir = it->move(neighbors);
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


bool Simulator::can_move (const Point& p, shared_ptr<Critter> it, const Direction move_dir) {
  if (it->wait_remaining())          return false;
  if (move_dir == Direction::CENTER) return false;

  if (debug) std::cerr << "Can " << it->name() << " at " << p <<  " move?\n";

  auto tmp =  p.translate(p, move_dir, view->width(),view->height());
  return tiles[tmp] == blank_tile;
}

void Simulator::move (const Point& p, shared_ptr<Critter> it, const Direction move_dir) {
  if (debug) std::cerr << "Get move from  " << it->name() << "\n";
  auto new_posit = p.translate(p, move_dir, view->width(),view->height());
  assert(new_posit != p);
  if ("Stone" != tiles[new_posit]->name()) {
    if (debug) std::cerr << it->name() << " moving to " << new_posit << "\n";
    std::swap(tiles[p], tiles[new_posit]);
  }
  view->draw(new_posit, tiles[new_posit]);
  view->draw(p, tiles[p]);
}


void Simulator::move (const Point& src, const Point& dest) {
  assert (src != dest);
  assert (tiles[dest] == blank_tile || tiles[dest]->name() == "Food");
  if (debug) std::cerr << "moving from " << src << " to " << dest << "\n";
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
    //if (debug) std::cerr << DirectionNames[dir] << ", "
    //    << tiles[new_posit]->name() << " @ "
    //        << new_posit << ";\t";

  }
  //if (debug) std::cerr << "\n";
  assert (neighbors.size() == 8);
  return neighbors;
}

void Simulator::take_action (const Point& src, shared_ptr<Critter> src_it, const Point& dest) {
  if (debug) std::cerr << "taking action for " << src_it->name() << " at " << src << "\n";

  auto dest_it = tiles[dest];
  if ("Stone" == dest_it->name()) {
    if (debug) std::cerr << src_it->name() << " at " << src << " tried to fight a " << dest_it->name() << "\n";
    src_it->sleep(20);
    src_it->sleep();  // inform critter we put it to sleep
  } else if ("Food" == dest_it->name()) {
    process_food(src, src_it, dest);
  } else if (can_mate(src_it, dest_it)) {
    process_mate(src, src_it, dest, dest_it);
  } else if (src_it->name() == dest_it->name()) {
    // same species, let the mating begin...
    if (debug) std::cerr << src_it->name() << " at " << src << " is mating with "
      << dest_it->name() <<  " at " << dest << "\n";
  } else {
    // 2 different species always fight
    if (can_fight(src, src_it, dest, dest_it)) {
      if (debug) std::cerr << src_it->name() << " at " << src << " is fighting " << dest_it->name() << "\n";
      if (debug) std::cerr << dest_it->name() << " is a player? " << dest_it->is_player() << "\n";

      process_fight(src, src_it, dest, dest_it);
    }
  }

}

void Simulator::process_food(const Point& src,  shared_ptr<Critter> src_it, const Point& dest)   {
  if (src_it->eat()) {
    if (debug) std::cerr << src_it->name() << " at " << src << " is eating and has "
      << src_it->food_remaining() << " food remaining.\n";
    src_it->eat_food();
    players[src_it->name()]->add_feeding();

    if (debug && src_it->is_asleep()) {
      if (debug) std::cerr << src_it->name() << " at " << src << " has eaten and has "
        << src_it->wait_remaining() << " wait remaining.\n";
    }
    tiles[dest] = blank_tile;
    move(src,dest);
    // make more food somewhere else
    auto food = std::next(std::begin(tiles), std::rand() % tiles.size());
    auto p = std::get<0>(*food);
    if (tiles[p] == blank_tile) {
      tiles[p] = std::make_shared<Food>();
      view->draw(p, tiles[p]);
    }
  }

}

bool Simulator::can_mate (shared_ptr<Critter> src_it, shared_ptr<Critter> dest_it)  {
  if (src_it->name() != dest_it->name()) return false;
  if (src_it->is_baby() || dest_it->is_baby()) return false;

  return (!src_it->is_parent() && !dest_it->is_parent() && !dest_it->is_asleep() && !dest_it->is_mating());

}

void Simulator::process_mate(const Point& src,  shared_ptr<Critter> src_it, const Point& dest, shared_ptr<Critter> dest_it)   {

  auto neighbors = get_neighbors(src);
  Direction dir = Direction::CENTER;;
  // find empty neightbor to put baby
  for (int i=0; i<8; i++) {
    if ("Empty" == neighbors[directions[i]]->name()) {
      dir = directions[i];
    }
  }
  if (dir == Direction::CENTER) {
    if(debug)    std::cerr << "Could not find a place to have baby.\n";
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
    if(debug)    std::cerr << src_it->name() << " made baby. The baby is at: " << b_dest << "\n";
  }
}

bool Simulator::can_fight (const Point& src,  shared_ptr<Critter> src_it,
    const Point& dest, shared_ptr<Critter> dest_it)  {
  if (src_it->wait_remaining() > 0) return false;

  if (dest_it == blank_tile) return false;

  if (debug) std::cerr << " can fight thinks the opponent is a " << dest_it->name() << " at " << dest << "\n";
  if (debug) std::cerr << " current player is a " << src_it->name() << " at " << src << "\n";
  if (debug) std::cerr << " is player? " << dest_it->is_player() << "\n";

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

  if (debug) std::cerr << " process fight thinks the opponent is a " << tiles[dest]->name() << "\n";

  //if the attacker wins, move to the dest tile
  //if the attacker loses, it is removed & the defender stays put.
  //On a draw, nothing else happens

  if (results == Simulator::AttackResults::ATTACKER) {
    if (debug) std::cerr << src_it->name() << " won \n";
    tiles[dest] = blank_tile;
    view->draw(dest, tiles[dest]);
    if (debug) std::cerr << "move after victory \n";
    move(src,dest);
    update_kill_stats(src_it, dest_it);
  } else if (results == Simulator::AttackResults::DEFENDER) {
    if (debug) std::cerr << dest_it->name() << " won \n";
    tiles[src] = blank_tile;
    view->draw(src, tiles[src]);
    update_kill_stats(dest_it, src_it);
  } else {
    if (debug) std::cerr << "fight was a draw \n";
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
  auto a_attack = attacker->fight(defender->name());
  auto d_attack = defender->fight(attacker->name());

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
  if (debug) std::cerr << winner->name() << " gets a kill \n";
  if (debug) std::cerr << players[winner->name()];
  players[loser->name()]->kill();
  if (debug) std::cerr << loser->name() << " is dead \n";
  if (debug) std::cerr << players[loser->name()];

  winner->won();            // report status
  loser->lost();            // report status
}





void Simulator::addItem(shared_ptr<Critter> item, const int num_items) {
  assert(item != nullptr);

  for (auto i = 0; i < num_items; i++) {
    auto c = item->create();
    Point p = get_random_blank_tile(); //{rand()%view->width()+1, rand()%view->height()+1};
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
  auto random_it = std::next(std::begin(blanks), std::rand() % blanks.size());
  return std::get<0>(*random_it);
}


