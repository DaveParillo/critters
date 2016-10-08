#ifndef STONE_H
#define STONE_H

#include <iostream>
#include <memory>

#include "Direction.h"
#include "Critter.h"

/**
 * A Stone is an obstacle in the game.
 * Critters that move into a stone are temporarily stunned.
 * Stunned critters are unable to take any action and can't move
 * until they have recovered.
 *
 * Additionally, they are vulnerable to attack.
 * A stunned Critter immediately loses a fight against any other Critter
 * that attacks it while it is stunned.
 */
class Stone : public Critter {

  public:
    Stone() : Critter("Stone") { }

    /**
     * Rather than the default 'S', a Stone is represented as a '#'.
     * @return the symbol for a Stone.
     */
    char glyph() const override{ 
      return char(35); // #
    }


    /**
     * Make a new Stone.
     * @return a shared pointer to a new Stone object.
     */
    std::shared_ptr<Critter> create() override {
      return std::make_shared<Stone>();
    }
};

#endif
