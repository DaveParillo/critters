#include <cassert>
#include <iostream>

#include "Critter.h"

using std::endl;

Critter::~Critter() 
{
 // currently no resources to clean up 
}

void Critter::start_mating(unsigned int length) {
  assert(!_mating && _awake);
  _mating = true;
  _has_mated = true;
  _wait_time = length;
}

void Critter::sleep(unsigned int num_turns) { 
  _awake = false; 
  _wait_time = num_turns;
}

void Critter::eat_food() { 
  _food_remaining += 50;
  if (_food_remaining >= MAX_FOOD) {
    sleep(_food_remaining - MAX_FOOD);
  } else {
    sleep(5);
  }

}

void Critter::tick() {  
  if (_food_remaining > 0) {
    _food_remaining--;
  }
  if (is_baby()) {
    _baby_timer--;
  }

  if (_wait_time > 0) {
    _wait_time--;  
  } else {
    _mating = false; 
    _awake = true; 
  }
}

const std::array<Critter::Attack, 4> Critter::attacks = {
  {
      Critter::Attack::ROAR,
      Critter::Attack::POUNCE,
      Critter::Attack::SCRATCH,
      Critter::Attack::FORFEIT
  }
};


std::ostream& operator<<(std::ostream &os, const Critter& it) {
  os << "Name: " << it.name() << std::endl;
  os << "\tColor: " << it.color() << std::endl;
  os << "\tIs awake?: " << it.is_awake() << std::endl;
  os << "\tIs mating?: " << it.is_mating() << std::endl;
  os << "\tIs player?: " << it.is_player() << std::endl;
  return os;
}

std::ostream& operator<<(std::ostream& os, const Critter::Attack& rhs) {
  std::string a;
  switch (rhs) {
    case Critter::Attack::ROAR:    a = "ROAR";    break;
    case Critter::Attack::POUNCE:  a = "POUNCE";  break;
    case Critter::Attack::SCRATCH: a = "SCRATCH"; break;
    case Critter::Attack::FORFEIT: a = "FORFEIT"; break;
    default:                       a = "NULL"; break;
  }
  return os << a;

}

