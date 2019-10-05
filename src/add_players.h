#pragma once

#include <critter.h>

#include <memory>
#include <vector>


/**
 * Add all the student players to the sim using this function.
 */
std::vector<std::shared_ptr<critter>> add_players ();

