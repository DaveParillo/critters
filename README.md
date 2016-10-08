Critters
========

This is a C++ version of a homework project originally offerred
as far as I can tell, first at Univerity of Washington
School of Computer Science & Engineering, CSE 142: Computer Programming I.
see: http://courses.cs.washington.edu/courses/cse142/

The whole idea is that the game is incomplete - you have to write the code to make
the critters move, fight, mate, and eat.

Runs the critters (Rock, Paper, Scissors) competition using 'critters' as
combatants. The combat occurs in a 2D grid 'world' with animals moving around.

The Critter world is divided into cells with integer coordinates. 
The world size is determined by the console size when the program is started.
The upper-left cell has coordinates (0, 0); 
x increases to the right and y increases downward.

Currently implemented in ncurses and in SDL2, but only runs in Windows under cygwin.

run with -h command line argument to see what options are available.

If when running the program over an ssh connection, no colors appear,
try changing your terminal settings.  On the command line, try:

    TERM="screen-256color"

the ncurses view should display colors if the environment supports it.
You may have to modify your putty settings if using ssh via putty.


Movement
--------
On each round of the simulation, the simulator asks each critter object which 
direction it wants to move. 
Each round a critter can move one square in any of the 8 'cardinal' directions, 
or stay at its current location.

If a critter moves into an empty square, the critter takes no other action
on that move.
If a critter moves into an occupied square, this informs the simulator that the
critter intends to take an action with the item in the destination cell.
Examples of actions are fighting, mating, and eating.

The world has a finite size, but it wraps around in all four directions. 
For example, moving east from the eastern-most (i.e. right) edge brings you back 
to the western-most (i.e. left) edge. 
You might want your critters to make several moves at once using a loop, but you can't. 
The only way a critter moves is to wait for the simulator to ask it for a 
single move and return that move.

Fighting
--------
A fight always occurs if two critters of two different Species attempt to occupy the same location.
The winner lives and the loser is removed from the game.
Each critter chooses one of the 4 possible Attack options.
Each attack is strong against one other attack (e.g. roar beats scratch) 
and weak against another (roar loses to pounce). 
The following table summarizes the choices and which critter will win in each case. 
To remember which beats which, notice that the starting letters of 
**R** oar, **P** ounce, **S** cratch match those of a (hopefully) more familiar game, 
**R** ock, **P** aper, **S** cissors. 

 - Rock smashes Scissors
 - Scissors cut Paper
 - Paper covers Rock

If the critters make the same choice, the result is a Draw.

|            |             |          | Critter #2 |             |             |
|------------|:------------|:--------:|:----------:|:-----------:|:-----------:|
|            |             | **ROAR** | **POUNCE** | **SCRATCH** | **FORFEIT** |
|            | **ROAR**    |   Draw   |   #2 wins  | #1 wins     |  #1 wins    |
| Critter #1 | **POUNCE**  | #1 wins  |    Draw    | #2 wins     |  #1 wins    |
|            | **SCRATCH** | #2 wins  |   #1 wins  |   Draw      |  #1 wins    |
|            | **FORFEIT** | #2 wins  |   #2 wins  | #2 wins     |    Draw     |

Mating
------
If two animals of the same species collide, they "mate" to produce a baby. 
Critters are vulnerable to attack while mating: 
any other species that attacks them wins automatically. 
A critter can initiate mating only once during its lifetime.

Eating
------
There are pieces of food on the world initially, and new food slowly grows into the world over time. 
As a critter moves, it may encounter food, in which case the simulator will ask your
critter whether it wants to eat it. 
Different kinds of animals have different eating behavior; 
some always eat, and others only eat under certain conditions. 
Every time one class of animals eats a few pieces of food, that critter will be 
put to "sleep" by the simulator for a small amount of time. 
While asleep, critters cannot move, and if they are attacked, they will always lose.

Obstacles / Hazards
-------------------
Some items in the world are hazards and should be avoided.
If a critter moves onto a Stone, for example, it is 'stunned' for a time.
While stunned, a critter cannot move, and if they are attacked, they will always lose.



Scoring
-------
The simulator keeps a score for each class (species) of critter, 
shown on the top of the screen. 
A species's score is based on how many animals of that class are alive, 
how much food they have eaten, and how many other animals they have killed.


Compiling
=========

Requires a ISO C++11 compatible compiler and both the ncurses and SDL2 libraries.
In either * nix, MacOSX, or cygwin, install ncurses-devel, SDL2, and SDLttf.
The fonts used are checked in here.

Compiles in clang or g++ under cygwin with:

    c++ *.cpp -std=c++11 -Wall -Wextra -pedantic -lncurses

The ncurses library is available for windows here:
Pre-built binaries:

    https://sourceforge.net/projects/pdcurses/files/pdcurses/
Source:

    https://github.com/wmcbrine/PDCurses

I have not tested this, but simply used a cygwin environment instead.

The supplied Makefile by default places all output in a debug directory.
The default 'configuration' is debug.
Type:

    make

to build and then:

    ./debug/critters

to run.

To build without debugging symbols, use:

    make config=release

Technically, you can use any word other than 'debug'.
A new directory will be created containing all the build artifacts.
To clean a specific configuration you need to declare it on the command line:

    make clean config=release

A CodeBlocks project file is also included, but is currently a work in progress.
Use what you can, but expect to have to modify it for your environment.
 
Building documentation
----------------------

The documentation can be generated using doxygen.
Run `doxygen` to regenerate the docs.
Output will be written to 'html'.
Open ./html/index.html to browse the docs.
This README will be included as the default page.



