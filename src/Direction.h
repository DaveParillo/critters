#ifndef DIRECTION_H
#define DIRECTION_H

#include <array>
#include <map>

/**
 * @class Direction
 * Defines the direction that a moving Critter is allowed to travel.
 *
 * - Increasing south moves a Critter down
 * - Increasing west moves a Critter right
 */
enum class Direction {
  CENTER,                 /*!< No movement. */
  NORTH,                  /*!< Move one cell up. */
  NORTH_EAST,             /*!< Move one cell up and to the right. */
  EAST,                   /*!< Move one cell right. */
  SOUTH_EAST,             /*!< Move one cell down and to the right. */
  SOUTH,                  /*!< Move one cell down. */
  SOUTH_WEST,             /*!< Move one cell down and to the left. */
  WEST,                   /*!< Move one cell left. */
  NORTH_WEST,             /*!< Move one cell up and to the left. */
};


/**
 * Send text representation of a Direction to an output stream.
 *
 * @param os reference to an output stream
 * @param rhs reference to the Direction
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream& os, const Direction& rhs);


/**
 * Allows the Direction class members to be used in a range for loop
 * Example:
 *    map<Direction, shared_ptr<Critter>> neighbors;
 *    for (const auto& d: directions) {
 *      // do something with neighbors[d] . . . 
 *    }
 * @see Simulator::get_neighbors
 */
const std::array<Direction,8> directions = 
{
  {
    Direction::NORTH,
    Direction::NORTH_EAST,
    Direction::EAST,     
    Direction::SOUTH_EAST,
    Direction::SOUTH,    
    Direction::SOUTH_WEST,
    Direction::WEST,     
    Direction::NORTH_WEST
  }
};


#endif
