#include "Species.h"

using std::endl;

ostream& operator<<(ostream &os, const Species &s) {
  os << s.name() << " species:" << endl;
  os << "Alive: " << s.alive() << endl;
  os << "Dead: " << s.dead() << endl;
  os << "Kills: " << s.kills() << endl;
  os << "Feedings: " << s.feedings() << endl;
  os << "Starved: " << s.starved() << endl;
  os << "Score: " << s.score() << endl;
  return os;
}
