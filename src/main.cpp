#include <iostream>
#include <memory>
#include <string>
#include <unistd.h>

// non player entities
#include "Food.h"
#include "Stone.h"

// player types
#include "Olympian.h"
#include "Bear.h"
#include "Lion.h"
#include "Tiger.h"
#include "Wombat.h"

#include "Simulator.h"
#include "View.h"
#include "ViewCurses.h"

using std::make_shared;
using std::string;

/**
 * Display a usage statement for this program.
 * @param name the name of this program as determined by args[0]
 */
static void show_usage(const string name)
{
  std::cerr << "Usage: " << name << " [-hd] [-f #] [-s #] [-n #]"
    << "Options:\n"
    << "  -h   Show this text\n"
    << "  -d   Enable debug output.\n"
    << "\t Output is written to std::cerr.  Redirect accordingly, for example\n"
    << "\t a.out 2> debug.txt\n"
    << "  -f   Set the amount of Food on the board.  Default = 250.\n"
    << "  -s   Set the number of Stones on the board.  Default = 10.\n"
    << "  -n   Set the number of Critters for each Species.  Default = 25.\n"
    << std::endl;
  exit(0);
}

/**
 * Main program.
 * - Processes command line arguments
 * - Creates Simulator and adds players and non-player entities to the simulation.
 * - starts the sim.
 * @param argc # of comamnd line args
 * @param argv array of comamnd line args as strings
 */
int main(int argc, char* argv[])
{
  int max_food = 250;
  int max_stones = 10;
  int max_critters = 25;

  int c;
  int debug = 0;
  string view = "default";
  string prog = argv[0];

  while ((c = getopt (argc, argv, "hdf:n:s:")) != -1) {
    switch (c) {
      case 'h':
        show_usage(prog);
        break;
      case 'd':
        debug = 1;
        break;
      case 'f':
        max_food = std::atoi(optarg);
        break;
      case 'n':
        max_critters = std::atoi(optarg);
        break;
      case 's':
        max_stones = std::atoi(optarg);
        break;
      default:
        show_usage(prog);
        break;
    }
  }


  Simulator s;
  s.set_view(std::unique_ptr<View>(new ViewCurses()));
  s.set_debug(debug);
  s.addItem(make_shared<Stone>(),     max_stones);
  s.addItem(make_shared<Food>(),      max_food);
  s.addItem(make_shared<Bear>(),      max_critters);
  s.addItem(make_shared<Lion>(),      max_critters);
  s.addItem(make_shared<Tiger>(),     max_critters);
  s.addItem(make_shared<Wombat>(),    max_critters);
  s.addItem(make_shared<Olympian>(),  max_critters);

  s.start();
  return 0;
}




