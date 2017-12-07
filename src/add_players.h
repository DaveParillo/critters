#pragma once

#include <Critter.h>

#include <memory>
#include <vector>


/**
 * Add all the student players to the sim using this function.
 */
std::vector<std::shared_ptr<Critter>> add_players ();

