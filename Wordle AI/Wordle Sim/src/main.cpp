/*
	Eric Grandizio
	Wordle Sim
	3/27/2022
*/

// ================================================================================================================================ //

#include <exception>
#include <iostream>

#include "WordleSim.h"
#include "ConsoleGame.h"

// ================================================================================================================================ //

/*
	The main function where the program starts.
	This Program tests the Wordle Sim by letting the user play it in the console.
*/
int main()
{
	// Attempt to launch the Wordle Sim.
	try
	{
		play_game();
		return 0;
	}
	// Log an error message if an exception occured.
	catch (const std::exception& ex)
	{
		std::cerr << "\n\nERROR!\n" << ex.what() << "\n\n";
		return 1;
	}
	// Log an error message if an unknown exception occured.
	catch (...)
	{
		std::cerr << "\n\nUNKNOWN ERROR!\n\n";
		return 2;
	}
}

// ================================================================================================================================ //
