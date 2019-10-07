#include <unistd.h>

#include <iostream>
#include <memory>
#include <string>

#include <add_players.h>

// default players
#ifdef WITH_SOLUTIONS
#  include "solutions/bear.h"
#  include "solutions/lion.h"
#  include "solutions/tiger.h"
#  include "solutions/raccoon.h"
#  include "solutions/wombat.h"
#  include "solutions/duck.h"
#endif

// non player entities
#include "food.h"
#include "stone.h"

#include "game.h"
#include "view.h"
#include "view_curses.h"

using std::make_shared;
using std::string;

/**
 * Display a usage statement for this program.
 * @param name the name of this program as determined by args[0]
 */
static void show_usage(const string name)
{
  std::cerr << "Usage: " << name << " [-hd] [-f #] [-s #] [-n #] [-x #] [-y #]"
#ifdef WITH_SOLUTIONS
    << " [-LTBRWD]\n"
#else
    << '\n'
#endif
    << "Options:\n"
    << "  -h   Show this text\n"
    << "  -d   Enable debug output.\n"
    << "\t Output is written to std::cerr.  Redirect accordingly, for example\n"
    << "\t a.out 2> debug.txt\n"
    << "  -f   Set the amount of Food on the board.  Default = 250.\n"
    << "  -s   Set the number of Stones on the board.  Default = 10.\n"
    << "  -n   Set the number of Critters for each Species.  Default = 25.\n"
    << "  -x   Set the world width.  Default = window width.\n"
    << "  -y   Set the world height.  Default = window height - space allocated for the score.\n"
    << "\n"
#ifdef WITH_SOLUTIONS
    << "  -L   Add Lion to the simulation\n"
    << "  -T   Add Tiger to the simulation\n"
    << "  -B   Add Bear to the simulation\n"
    << "  -R   Add Raccoon to the simulation\n"
    << "  -W   Add Wombat to the simulation\n"
    << "  -D   Add Duck to the simulation\n"
#endif
    << std::endl;
  exit(0);
}


int main(int argc, char** argv) {
  int max_food = 50;
  int max_stones = 50;
  int max_critters = 25;

  // screen ht and width.  0 means use current window size to comute x & y
  int x = 0;
  int y = 0;

#ifdef WITH_SOLUTIONS
  bool use_lion = false;
  bool use_tiger = false;
  bool use_bear = false;
  bool use_raccoon = false;
  bool use_wombat = false;
  bool use_duck = false;
#endif

  int c;
  int debug = 0;
  string view = "default";
  string prog = argv[0];
#ifdef WITH_SOLUTIONS
  auto valid_args = "hdf:n:s:x:y:LTBRWD";
#else
  auto valid_args = "hdf:n:s:x:y:";
#endif

  while ((c = getopt (argc, argv, valid_args)) != -1) {
    switch (c) {
      case 'h':
        show_usage(prog);
        break;
      case 'd':
        debug = 1;
        break;
      case 'f': max_food     = std::atoi(optarg);
        break;
      case 'n': max_critters = std::atoi(optarg);
        break;
      case 's': max_stones   = std::atoi(optarg);
        break;
      case 'x': x = std::atoi(optarg);
        break;
      case 'y': y = std::atoi(optarg);
        break;

#ifdef WITH_SOLUTIONS
      // enable solution critters
      case 'L':
        use_lion = true;
        break;
      case 'T':
        use_tiger = true;
        break;
      case 'B':
        use_bear = true;
        break;
      case 'R':
        use_raccoon = true;
        break;
      case 'W':
        use_wombat = true;
        break;
      case 'D':
        use_duck = true;
        break;
#endif
      default:
        show_usage(prog);
        break;
    }
  }

  game g;

  g.set_view(std::unique_ptr<view_curses>(new view_curses(x, y)));
  g.set_debug(debug);
  g.add_item(make_shared<stone>(),     max_stones);
  g.add_item(make_shared<food>(),      max_food);

#ifdef WITH_SOLUTIONS
  if (use_bear)     g.add_item(make_shared<bear>(),    max_critters);
  if (use_lion)     g.add_item(make_shared<lion>(),    max_critters);
  if (use_tiger)    g.add_item(make_shared<tiger>(),   max_critters);
  if (use_raccoon)  g.add_item(make_shared<raccoon>(), max_critters);
  if (use_wombat)   g.add_item(make_shared<wombat>(),  max_critters);
  if (use_duck)     g.add_item(make_shared<duck>(),    max_critters);
#endif

  for (const auto& p: add_players()) {
    g.add_item(p,  max_critters);
  }

  g.start();
  return 0;
}



