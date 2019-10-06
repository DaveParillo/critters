#include <cstdint>

#include "direction.h"
#include "point.h"

point point::translate(const point& p, const direction& movement, 
                       const int16_t max_x, const int16_t max_y) const {
  auto x     = p.x;
  auto y     = p.y;
  auto north = (p.y == 0? max_y-1: p.y-1);
  auto south = (p.y+1)%max_y;
  auto east  = (p.x+1)%max_x;
  auto west  = (p.x == 0? max_x-1: p.x-1);

  switch (movement) {
    case direction::NORTH:
      y = north;
      break;
    case direction::NORTH_EAST:
      y = north;
      x = east;
      break;
    case direction::EAST:
      x = east;
      break;
    case direction::SOUTH_EAST:
      y = south;
      x = east;
      break;
    case direction::SOUTH:
      y = south;
      break;
    case direction::SOUTH_WEST:
      y = south;
      x = west;
      break;
    case direction::WEST:
      x = west;
      break;
    case direction::NORTH_WEST:
      y = north;
      x = west;
      break;
    case direction::CENTER: // no change in position [[fallthrough]]
    default:
      break;
  }
  return point {x, y};
}


