
#include <string>

#include "direction.h"

std::ostream& operator<<(std::ostream& os, const direction& rhs) {
  std::string dir;
  switch (rhs) {
    case direction::CENTER:     dir = "CENTER";     break;
    case direction::NORTH:      dir = "NORTH";      break;
    case direction::NORTH_EAST: dir = "NORTH EAST"; break;
    case direction::EAST:       dir = "EAST";       break;
    case direction::SOUTH_EAST: dir = "SOUTH EAST"; break;
    case direction::SOUTH:      dir = "SOUTH";      break;
    case direction::SOUTH_WEST: dir = "SOUTH WEST"; break;
    case direction::WEST:       dir = "WEST";       break;
    case direction::NORTH_WEST: dir = "NORTH WEST"; break;
  }
  return os << dir;

}


