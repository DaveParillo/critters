/*
 * Lion.h
 *
 *  Created on: Apr 22, 2014
 *      Author: Tasha
 */

#pragma once

#include <Critter.h>
#include <Color.h>
#include <Direction.h>


/**
 * A Critter with lion behaviors.
 */
class Lion : public Critter {
  private:
    bool foughtSinceLastEat; /**< flag to limit the amount of eating.  Ensure at least 1 fight between meals */
    int moveNum;             /**< counts how many times this lion has moved */

  public:
    Lion() : 
      Critter("Lion"), 
      foughtSinceLastEat(false), moveNum(0)  { }

    bool   is_player() const override { return true; }
    Color  color()     const override { return Color::YELLOW; }

    bool eat() override;

    Attack fight(std::string opponent) override;

    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    std::shared_ptr<Critter> create() override;
};


