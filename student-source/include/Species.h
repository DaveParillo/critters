#pragma once

#include <iostream>
#include <string>
#include "Critter.h"

using std::string;

/**
 * Stores summary statistics for a set of Critters.
 */
class Species {
  private:
    string       _name;           /**< Name of this species */
    unsigned int num_alive;       /**< Total number of living critters. */
    unsigned int num_dead;        /**< Total number of dead critters. */
    unsigned int num_kills;       /**< Total number of other critters killed by this species. */
    unsigned int num_feedings;    /**< Total number of times critters of this species have eaten. */
    unsigned int num_starved;     /**< Total number of of dead critters that starved to death. */

    /**
     * The default constructor is private to prevent creating a Species without a name.
     */
    Species() {}

  public:
    /**
     * Create a new species with an initial population.
     * @param species_name name of this species, which will be used as a key in a map
     *        by the simulator and passed to the View.
     * @param initial_pop the starting population.
     */
    Species(string species_name, unsigned int initial_pop) : 
      _name(species_name), num_alive(initial_pop), 
      num_dead(0), num_kills(0), num_feedings(0), num_starved(0) {}

    /**
     * Get the name of this Species.
     * @return the name
     */
    string name() const { return _name; }
    /**
     * Get the living members count of this Species.
     * @return the total number of living members
     */
    unsigned int alive() const { return num_alive; }
    /**
     * Get the dead members count of this Species.
     * @return the total number of dead members
     */
    unsigned int dead() const { return num_dead; }
    /**
     * Get the combat victories count of this Species.
     * @return the total number of other species killed by this one
     */
    unsigned int kills() const { return num_kills; }
    /**
     * Get the food consumption count of this Species.
     * @return the total number of food eaten by this species
     */
    unsigned int feedings() const { return num_feedings; }
    /**
     * Get the count of those who died by letting their food reserves drop to 0.
     * @return the total number of member that dies by starvation
     */
    unsigned int starved() const { return num_starved; }

    /**
     * Get the current total score.
     * @return the score
     */
    unsigned int score() const { 
      return num_alive + num_kills + num_feedings; 
    }

    /**
     * Add a new member to the population.
     * This should only happen when two members mate.
     */
    void add_member() { ++num_alive; }
    /**
     * Record that another critter was killed by a member of this species.
     */
    void add_kill() { ++num_kills; }
    /**
     * Record that food was eaten by a member of this species.
     */
    void add_feeding() { ++num_feedings; }
    /**
     * Record a death from starvation.
     */
    void add_starved() { kill(); ++num_starved; }

    /**
     * Kill off a member of this species.
     */
    void kill() {
      if (num_alive > 0) {
        --num_alive;
        ++num_dead;
      }
    }


};

/**
 * Insert formatted Species state information into an output stream.
 * @param os reference to an output stream
 * @param s reference to a Species to print
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream& os, const Species& s);


