#pragma once
#include <Critter.h>
#include <Color.h>
#include <Direction.h>

#include <chrono> 
#include <iostream>
#include <map>
#include <memory>


/**
 * A sample Critter that uses the neighbors map to make fewer random choices.
 */
class Wombat : public Critter {
  private:
    /**
     * Logic a wombat uses to evaluate nearby enemies.
     */
    Direction look_for_enemies(std::map<Direction, std::shared_ptr<Critter>> neighbors);
    /**
     * Logic a wombat uses to look for mates.
     */
    Direction look_for_mates  (std::map<Direction, std::shared_ptr<Critter>> neighbors);
    /**
     * Logic a wombat uses to look for nearby empty tiles.
     */
    Direction look_for_empty  (std::map<Direction, std::shared_ptr<Critter>> neighbors);
    /**
     * Logic a wombat uses to look for food.
     */
    Direction look_for_food   (std::map<Direction, std::shared_ptr<Critter>> neighbors);

    /**
     * A seed for random behaviors
     */
    unsigned seed_ = std::chrono::system_clock::now().time_since_epoch().count();


  public:
    /**
     * Create a new Wombat.
     */
    Wombat() : Critter("Wily Wombat") { }

    /**
     * Informs the Simulator that this Wombat is a 'player'.
     * @return true always.  A Wombat is a combatant.
     */
    bool is_player() const override { return true; }

    /**
     * Informs the simulator of this critter color.
     * @return the Color of this critter
     */
    Color  color()   const override { return Color::GREEN; }

    /**
     * Tell the sim that this Wombat wants to eat.
     *
     * @return true if this Wombat wants to eat this turn.
     */
    bool eat() override {
  return food_remaining() < 350;
}


    /**
     * Informs the Simulator of how to respond during a fight.
     * Wombats have logic to fight against Lions and Tigers.
     * Against other Critters, they choose a random attack.
     *
     * @return the Attack this Critter should make this turn
     */
    Attack fight(std::string opponent) override;

    /**
     * Informs the Simulator of Wombat movement during a turn.
     *
     * Wombats move in the same direction until they encouter something
     * that forces a change (Stone, Food, another Critter).
     * Then they will pick a new direction.
     *
     * They may move in any of the 8 available directions, 
     * but never choose to stay put (Direction::CENTER).
     * @param neighbors who or what is in the cells adjacent to this wombat
     * @return the Direction this Wombat should move this turn
     */
    Direction move(std::map<Direction, std::shared_ptr<Critter>> neighbors) override;

    
    /**
     * Make a new Wombat.
     * @return a shared pointer to a new Wombat object.
     */
    std::shared_ptr<Critter> create() override;
};


