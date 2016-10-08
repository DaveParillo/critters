#ifndef POINT_H
#define POINT_H

#include <iostream>
#include <functional>
#include <memory>

#include "Direction.h"

using std::ostream;

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

    /**
     * Member comparison for ==.
     * @param rhs the Point to compare with this point.
     * @return true if this point == rhs point
     */
    bool operator==(const Point& rhs) const;
    /**
     * Member comparison for !=.
     * @param rhs the Point to compare with this point.
     * @return true if this point != rhs point
     */
    bool operator!=(const Point& rhs) const;
    /**
     * Member comparison for less than
     * @param rhs the Point to compare with this point.
     * @return true if this point < rhs point
     */
    bool operator< (const Point& rhs) const;

    /**
     * Send point to output stream.
     *
     * @todo should not be a friend
     *
     * @param os reference to an output stream
     * @param rhs reference to the Point
     * @return the modified output stream
     */
    friend ostream& operator<<(ostream& os, const Point& rhs);

};



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
       * @param p the Point to generate a hascode for
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

#endif
