#pragma once

#include <cstdint>
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
    int16_t _x = 0;     /**< The x-coordinate of this position */
    int16_t _y = 0;     /**< The y-coordinate of this position */

  public:
    /**
     * Create a new point at a default location.
     */
    Point() = default;
    /**
     * Create a new point at a specific location.
     */
    Point(int16_t x, int16_t y)
      : _x{x}, _y{y}
    {}
    
    /**
     * Point copy constructor.
     */
    Point(const Point&) = default;
    /**
     * Point move constructor.
     */
    Point(Point&&) = default;
    /**
     * Point copy assignment.
     */
    Point& operator=(const Point&) = default;
    /**
     * Point move assignment.
     */
    Point& operator=(Point&&) = default;


    /**
     * Get the X-coordinate of a Point.
     * @return the current x-coordinate.
     */
    int16_t x() const { return _x; }
    /**
     * Get the Y-coordinate of a Point.
     * @return the current y-coordinate.
     */
    int16_t y() const { return _y; }

    /**
     * Given a Point and Direction, calculate a new Point.
     * @param p the starting point
     * @param movement the direction of movement
     * @param max_x the limit on movement in the x direction.
     * @param max_y the limit on movement in the y direction.
     * @return the new Point
     */
    Point translate(const Point& p, const Direction& movement,
                    const int16_t max_x, const int16_t max_y) const;

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

        return ((hash<int>()(719)
              ^ (hash<int16_t>()(p.x()) << 1)) >> 1)
              ^ (hash<int16_t>()(p.y()) << 1);
      }
    };

}



