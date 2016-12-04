
#include <string>

#include "Direction.h"

std::ostream& operator<<(std::ostream& os, const Direction& rhs) {
  std::string dir;
  switch (rhs) {
    case Direction::CENTER:     dir = "CENTER";     break;
    case Direction::NORTH:      dir = "NORTH";      break;
    case Direction::NORTH_EAST: dir = "NORTH EAST"; break;
    case Direction::EAST:       dir = "EAST";       break;
    case Direction::SOUTH_EAST: dir = "SOUTH EAST"; break;
    case Direction::SOUTH:      dir = "SOUTH";      break;
    case Direction::SOUTH_WEST: dir = "SOUTH WEST"; break;
    case Direction::WEST:       dir = "WEST";       break;
    case Direction::NORTH_WEST: dir = "NORTH WEST"; break;
  }
  return os << dir;

}


