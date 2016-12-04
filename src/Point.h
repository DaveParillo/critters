#pragma once

#include <iostream>
#include <functional>
#include <memory>

#include "Direction.h"

/**
 * Defines a single position in the Critter world.
 * Positions are organized in a traditional X-Y grid.
 */
class Point {
  private:
    const unsigned char _x;     /**< The x-coordinate of this position */
    const unsigned char _y;     /**< The y-coordinate of this position */

  public:
    /**
     * Create a new point at a default location.
     */
    Point() : _x(0), _y(0) {}
    /**
     * Create a new point at a specific location.
     * @param x the new x-coordinate
     * @param y the new y-coordinate
     */
    Point(int x, int y) : _x(x), _y(y) {}
    /**
     * Point copy constructor.
     * @param p the current point.
     */
    Point( const Point& p ) : _x(p.x()), _y(p.y()) {}

    /**
     * Get the X-coordinate of a Point.
     * @return the current x-coordinate.
     */
    int x() const { return _x; }
    /**
     * Get the Y-coordinate of a Point.
     * @return the current y-coordinate.
     */
    int y() const { return _y; }

    /**
     * Given a Point and Direction, calculate a new Point.
     * @param p the starting point
     * @param movement the direction of movement
     * @param max_x the limit on movement in the x direction.
     * @param max_y the limit on movement in the y direction.
     * @return the new Point
     */
    Point translate(const Point& p, const Direction& movement,
                    const int max_x, const int max_y) const;

};

/**
 * Send point to output stream.
 *
 * @param os reference to an output stream
 * @param rhs reference to the Point
 * @return the modified output stream
 */
inline std::ostream& operator<<(std::ostream& os, const Point& rhs) {
  return os << "(" << rhs.x() << "," << rhs.y() << ")";
}

/**
 * Comparison for ==.
 * @param lhs the Point left operand
 * @param rhs the Point right operand
 * @return true if the coordinates of both points are equal.
 */
inline bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x() == rhs.x() && lhs.y() == rhs.y());
}

/**
 * Comparison for !=.
 * @param lhs the Point left operand
 * @param rhs the Point right operand
 * @return true if either coordinate between the two points differs.
 */
inline bool operator!=(const Point& lhs, const Point& rhs) {
  return !operator==(lhs,rhs);
}

/**
 * Comparison for less than.
 * @param lhs the Point left operand
 * @param rhs the Point right operand
 * @return true if this point < rhs point
 */
inline bool operator< (const Point& lhs, const Point& rhs) {
  return (lhs.y() < rhs.y() || (lhs.y() == rhs.y() && lhs.x() < rhs.x()));
}



/**
 * Add an overload to hash to include Points.
 */
namespace std {

  /**
   * Add an overload to hash.
   */
  template <>
    struct hash<Point>
    {
      /**
       * Return the hash code for a point.
       *
       * @see http://en.cppreference.com/w/cpp/utility/hash
       * @param p the Point to generate a hashcode for
       * @return the hashcode
       */
      std::size_t operator()(const Point& p) const
      {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<int>()(17)
              ^ (hash<int>()(p.x()) << 1)) >> 1)
              ^ (hash<int>()(p.y()) << 1);
      }
    };

}



