#include <cassert>
#include <iostream>

#include "critter.h"

void critter::start_mating(unsigned int length) {
  assert(!mating_ && awake_);
  mating_ = true;
  has_mated_ = true;
  wait_time_ = length;
}

void critter::sleep(unsigned int num_turns) { 
  awake_ = false; 
  wait_time_ = num_turns;
}

void critter::eat_food() { 
  food_remaining_ += 50;
  if (food_remaining_ >= MAX_FOOD) {
    sleep(food_remaining_ - MAX_FOOD);
  } else {
    sleep(5);
  }

}

void critter::tick() {  
  if (food_remaining_ > 0) {
    food_remaining_--;
  }
  if (is_baby()) {
    baby_timer_--;
  }

  if (wait_time_ > 0) {
    wait_time_--;  
  } else {
    mating_ = false; 
    awake_ = true; 
  }
}

const std::array<critter::attack, 4> critter::attacks = {
  {
      critter::attack::ROAR,
      critter::attack::POUNCE,
      critter::attack::SCRATCH,
      critter::attack::FORFEIT
  }
};


std::ostream& operator<<(std::ostream &os, const critter& it) {
  os << "Name: " << it.name()
     << "\n\tColor: " << it.color()
     << "\n\tIs awake?: " << it.is_awake()
     << "\n\tIs mating?: " << it.is_mating()
     << "\n\tIs player?: " << it.is_player();
  return os;
}

std::ostream& operator<<(std::ostream& os, const critter::attack& rhs) {
  std::string a;
  switch (rhs) {
    case critter::attack::ROAR:    a = "ROAR";    break;
    case critter::attack::POUNCE:  a = "POUNCE";  break;
    case critter::attack::SCRATCH: a = "SCRATCH"; break;
    case critter::attack::FORFEIT: a = "FORFEIT"; break;
    default:                       a = "NULL"; break;
  }
  return os << a;

}

