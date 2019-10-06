#ifndef MESA_CRITTERS_SPECIES_H
#define MESA_CRITTERS_SPECIES_H

#include <iosfwd>
#include <string>

#include "critter.h"

/**
 * Stores summary statistics for a set of critters.
 */
class species {
  private:
    std::string       name_;           /**< Name of this species */
    unsigned int num_alive_;       /**< Total number of living critters. */
    unsigned int num_dead_;        /**< Total number of dead critters. */
    unsigned int num_kills_;       /**< Total number of other critters killed by this species. */
    unsigned int num_feedings_;    /**< Total number of times critters of this species have eaten. */
    unsigned int num_starved_;     /**< Total number of of dead critters that starved to death. */

    /**
     * The default constructor is private to prevent creating a Species without a name.
     */
    species() {}

  public:
    /**
     * Create a new species with an initial population.
     * @param species_name name of this species, which will be used as a key in a map
     *        by the simulator and passed to the View.
     * @param initial_pop the starting population.
     */
    species(std::string species_name, unsigned int initial_pop) : 
      name_(species_name), num_alive_(initial_pop), 
      num_dead_(0), num_kills_(0), num_feedings_(0), num_starved_(0) {}

    /**
     * Get the name of this Species.
     * @return the name
     */
    std::string name() const { return name_; }
    /**
     * Get the living members count of this Species.
     * @return the total number of living members
     */
    unsigned int alive() const { return num_alive_; }
    /**
     * Get the dead members count of this Species.
     * @return the total number of dead members
     */
    unsigned int dead() const { return num_dead_; }
    /**
     * Get the combat victories count of this Species.
     * @return the total number of other species killed by this one
     */
    unsigned int kills() const { return num_kills_; }
    /**
     * Get the food consumption count of this Species.
     * @return the total number of food eaten by this species
     */
    unsigned int feedings() const { return num_feedings_; }
    /**
     * Get the count of those who died by letting their food reserves drop to 0.
     * @return the total number of member that dies by starvation
     */
    unsigned int starved() const { return num_starved_; }

    /**
     * Get the current total score.
     * @return the score
     */
    unsigned int score() const { 
      return num_alive_ + num_kills_ + num_feedings_; 
    }

    /**
     * Add a new member to the population.
     * This should only happen when two members mate.
     */
    void add_member() { ++num_alive_; }
    /**
     * Record that another critter was killed by a member of this species.
     */
    void add_kill() { ++num_kills_; }
    /**
     * Record that food was eaten by a member of this species.
     */
    void add_feeding() { ++num_feedings_; }
    /**
     * Record a death from starvation.
     */
    void add_starved() { kill(); ++num_starved_; }

    /**
     * Kill off a member of this species.
     */
    void kill() {
      if (num_alive_ > 0) {
        --num_alive_;
        ++num_dead_;
      }
    }


};

/**
 * Insert formatted Species state information into an output stream.
 * @param os reference to an output stream
 * @param s reference to a Species to print
 * @return the modified output stream
 */
std::ostream& operator<<(std::ostream& os, const species& s);

#endif
