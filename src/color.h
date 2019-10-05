#ifndef MESA_CRITTERS_COLOR_H
#define MESA_CRITTERS_COLOR_H

#include <array>
#include <iosfwd>
#include <string>

/**
 * @class color
 * Defines the colors used to color dritters in the sim.
 * Color definitons are currently using the "lowest common denominator", that is 8.
 * Color in the sim is defined abstractly, that is a color <em>type</em>
 * is defined, but it is up to each View to actually implement color using whatever
 * mechanisms exist for the view implementation.
 *
 * For example, ncurses defines colors using C style macros:
 *     init_pair(2,COLOR_WHITE, COLOR_BLACK);
 *
 * whereas SDL defines color using an object.
 */
enum class color {
  BLACK,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  MAGENTA,
  CYAN,
  WHITE
};

/**
 * An array container for the colors
 */
const std::array<color,8> colors = 
{
  {
    color::BLACK,
    color::RED,
    color::GREEN,     
    color::BLUE,
    color::YELLOW,    
    color::MAGENTA,
    color::CYAN,     
    color::WHITE
  }
};

/**
 * Extract a color to an output stream.
 *
 * @param os reference to an output stream
 * @param rhs reference to the Color
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream& os, const color& rhs);

#endif
