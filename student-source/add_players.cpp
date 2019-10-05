
// add your competitor(s) include files here
#include "olympian.h"

#include <critter.h>
#include <memory>
#include <vector>

using players = std::vector<std::shared_ptr<critter>>;

players add_players () {
  players p;

  // push back your Critter onto the players vector
  p.push_back(std::make_shared<olympian>());

  return p;
}


