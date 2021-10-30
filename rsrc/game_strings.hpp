// (c) 2019-21 by SwordLord - the coding crew
// This file is part of the pnk game

#pragma once


namespace pnk
{
/**
 * Generic strings
 */
const static std::string_view str_game_over{"OH NO!\n\
You lost.\n\
\n\
Game Over!\n\
\n\
(press 'X' to quit)"};

/**
 * Level 1 strings
 */
const static std::string_view str_lvl1_intro{"You enter your chambers\n\
in the search for The Lost Axe\n\
But alas, what do you see??\n\
Pigs all around!\n\
\n\
(press 'X' to start)"};

/*
 * We will replace below text with that here once we have level 2!
 const static std::string_view str_lvl1_end{"The pig boss is\n\
vanquished!\n\
Lets clean up the next floor\n\
You may proceed to the door"};

 */
// TODO replace me with the text above once we have level 2
const static std::string_view str_lvl1_end{"The pig boss is\n\
vanquished!\n\
\n\
But let us first design\n\
the next few levels\n\
\n\
You will now return\n\
to the main screen"};

/**
 * Level 2 strings
 */

const static std::string_view str_lvl2_intro{"You enter your chambers\n\
in the 2nd floor\n\
But AAARGH\n\
Even more pigs around!\n\
\n\
(press 'X' to start)"};

const static std::string_view str_lvl2_end{"The 2nd floor\n\
pig boss is vanquished!\n\
Lets clean up the next floor\n\
You may proceed to the door"};

}