
#include "Color.h"

std::ostream& operator<<(std::ostream& os, const Color& rhs) {
  std::string color;
  switch (rhs) {
    case Color::BLACK:   color = "BLACK";   break;
    case Color::RED:     color = "RED";     break;
    case Color::GREEN:   color = "GREEN";   break;
    case Color::BLUE:    color = "BLUE";    break;
    case Color::YELLOW:  color = "YELLOW";  break;
    case Color::MAGENTA: color = "MAGENTA"; break;
    case Color::CYAN:    color = "CYAN";    break;
    case Color::WHITE:   color = "WHITE";   break;
  }
  return os << color;
}


