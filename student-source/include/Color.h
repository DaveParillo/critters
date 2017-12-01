#pragma once

#include <iostream>
#include <string>

/**
 * @class Color
 * Defines the colors used to color dritters in the sim.
 * Color definitonis are currently using the "lowest common denominator", that is 8.
 * Color in the sim is defined abstractly, that is a color <em>type</em>
 * is defined, but it is up to each View to actually implement color using whatever
 * mechanisms exist for the view implementation.
 *
 * For example, ncurses defines colors using C style macros:
 *     init_pair(2,COLOR_WHITE, COLOR_BLACK);
 *
 * whereas SDL defines color using an object.
 */
enum class Color {
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
 * Send text representation of a Color to an output stream.
 *
 * @param os reference to an output stream
 * @param rhs reference to the Color
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream& os, const Color& rhs);


