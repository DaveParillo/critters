#include "Direction.h"
#include "Point.h"

Point Point::translate(const Point& p, const Direction& movement, 
                       const int max_x, const int max_y) const {
  auto x     = p.x();
  auto y     = p.y();
  auto north = (p.y() == 0? max_y-1: p.y()-1);
  auto south = (p.y()+1)%max_y;
  auto east  = (p.x()+1)%max_x;
  auto west  = (p.x() == 0? max_x-1: p.x()-1);

  switch (movement) {
    case Direction::NORTH:
      y = north;
      break;
    case Direction::NORTH_EAST:
      y = north;
      x = east;
      break;
    case Direction::EAST:
      x = east;
      break;
    case Direction::SOUTH_EAST:
      y = south;
      x = east;
      break;
    case Direction::SOUTH:
      y = south;
      break;
    case Direction::SOUTH_WEST:
      y = south;
      x = west;
      break;
    case Direction::WEST:
      x = west;
      break;
    case Direction::NORTH_WEST:
      y = north;
      x = west;
      break;
    case Direction::CENTER: // no change in position [[fallthrough]]
    default:
      break;
  }
  return Point {x, y};
}



//
// operator overloads
//
ostream& operator<<(ostream& os, const Point& rhs) {
  return os << "(" << rhs.x() << "," << rhs.y() << ")";
}


bool Point::operator==(const Point& rhs) const {
  return (_x == rhs.x() && _y == rhs.y());
}
bool Point::operator!=(const Point& rhs) const {
  return (_x != rhs.x() || _y != rhs.y());
}

bool Point::operator<(const Point& rhs) const {
  return (_y < rhs.y() || (_y == rhs.y() && _x < rhs.x()));
}




