
// add your competitor(s) include files here
#include "Olympian.h"

#include <Critter.h>
#include <memory>
#include <vector>

using players = std::vector<std::shared_ptr<Critter>>;

players add_players () {
  players p;

  // push back your Critter onto the players vector
  p.push_back(std::make_shared<Olympian>());

  return p;
}


