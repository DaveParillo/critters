#ifndef MESA_CRITTERS_POINT_H
#define MESA_CRITTERS_POINT_H

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>

#include "direction.h"

/**
 * Defines a single position in the Critter world.
 * Positions are organized in a traditional X-Y grid.
 */
struct point {
    int16_t x = 0;     /**< The x-coordinate of this position */
    int16_t y = 0;     /**< The y-coordinate of this position */

    /**
     * Create a new point at a default location.
     */
    point() = default;
    /**
     * Create a new point at a specific location.
     */
    point(int16_t x, int16_t y)
      : x{x}, y{y}
    {}
    
    /**
     * Point copy constructor.
     */
    point(const point&) = default;
    /**
     * Point move constructor.
     */
    point(point&&) = default;
    /**
     * Point copy assignment.
     */
    point& operator=(const point&) = default;
    /**
     * Point move assignment.
     */
    point& operator=(point&&) = default;

    /**
     * Given a Point and Direction, calculate a new Point.
     * @param p the starting point
     * @param movement the direction of movement
     * @param max_x the limit on movement in the x direction.
     * @param max_y the limit on movement in the y direction.
     * @return the new Point
     */
    point translate(const point& p, const direction& movement,
                    const int16_t max_x, const int16_t max_y) const;

};

/**
 * Send point to output stream.
 *
 * @param os reference to an output stream
 * @param rhs reference to the Point
 * @return the modified output stream
 */
inline
std::ostream& operator<<(std::ostream& os, const point& rhs) {
  return os << "(" << rhs.x << "," << rhs.y << ")";
}

/**
 * Comparison for ==.
 * @param lhs the Point left operand
 * @param rhs the Point right operand
 * @return true if the coordinates of both points are equal.
 */
inline
bool operator==(const point& lhs, const point& rhs) {
  return (lhs.x == rhs.x && lhs.y == rhs.y);
}

/**
 * Comparison for !=.
 * @param lhs the Point left operand
 * @param rhs the Point right operand
 * @return true if either coordinate between the two points differs.
 */
inline
bool operator!=(const point& lhs, const point& rhs) {
  return !operator==(lhs,rhs);
}

/**
 * Comparison for less than.
 * @param lhs the Point left operand
 * @param rhs the Point right operand
 * @return true if this point < rhs point
 */
inline
bool operator< (const point& lhs, const point& rhs) {
  return (lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x));
}



/**
 * Add an overload to hash to include Points.
 */
namespace std {

  /**
   * Add an overload to hash.
   */
  template <>
    struct hash<point>
    {
      /**
       * Return the hash code for a point.
       *
       * @see http://en.cppreference.com/w/cpp/utility/hash
       * @param p the Point to generate a hashcode for
       * @return the hashcode
       */
      std::size_t operator()(const point& p) const
      {
        using std::size_t;
        using std::hash;
        using std::string;

        return ((hash<int>()(719)
              ^ (hash<int16_t>()(p.x) << 1)) >> 1)
              ^ (hash<int16_t>()(p.y) << 1);
      }
    };

}

#endif

