#ifndef COLOR_H
#define COLOR_H

#include <map>

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
 * Allows the Color class members to be printed with meaningful strings.
 * Primarily used for debugging.
 * Example:
 *
 * cout << "Color is " << ColorNames[Color::RED] << endl;
 */
static std::map<Color, const char *> ColorNames = {
  {Color::BLACK, "BLACK"},
  {Color::RED, "RED"},
  {Color::GREEN, "GREEN"},
  {Color::BLUE, "BLUE"},
  {Color::YELLOW, "YELLOW"},
  {Color::MAGENTA, "MAGENTA"},
  {Color::CYAN, "CYAN"},
  {Color::WHITE, "WHITE"}
};

#endif
