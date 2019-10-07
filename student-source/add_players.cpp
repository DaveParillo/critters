#include <memory>
#include <vector>

#include <add_players.h>
#include <critter.h>

// add your competitor(s) include files here
#include "olympian.h"


using players = std::vector<std::shared_ptr<critter>>;

players add_players () {
  players p;

  // push back your Critter onto the players vector
  p.push_back(std::make_shared<olympian>());

  return p;
}


