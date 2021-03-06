#include <iostream>

#include "species.h"

using std::endl;

std::ostream& operator<<(std::ostream& os, const species& s) {
  os << s.name() << " species:" << endl;
  os << "Alive: " << s.alive() << endl;
  os << "Dead: " << s.dead() << endl;
  os << "Kills: " << s.kills() << endl;
  os << "Feedings: " << s.feedings() << endl;
  os << "Starved: " << s.starved() << endl;
  os << "Score: " << s.score() << endl;
  return os;
}
