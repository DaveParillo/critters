
#include "color.h"

std::ostream& operator<<(std::ostream& os, const color& rhs) {
  std::string value;
  switch (rhs) {
    case color::BLACK:   value = "BLACK";   break;
    case color::RED:     value = "RED";     break;
    case color::GREEN:   value = "GREEN";   break;
    case color::BLUE:    value = "BLUE";    break;
    case color::YELLOW:  value = "YELLOW";  break;
    case color::MAGENTA: value = "MAGENTA"; break;
    case color::CYAN:    value = "CYAN";    break;
    case color::WHITE:   value = "WHITE";   break;
  }
  return os << value;
}


