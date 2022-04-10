/*
	Charles Schuck - Eric Grandizio
	
	Wordle AI / Wordle Sim
*/

// ================================================================================================================================ //

#include <exception>
#include <iostream>

#include "WordleAI.h"
#include "WordleSim.h"
#include "ConsoleGame.h"

// ================================================================================================================================ //

/*
	The Main function for the Wordle AI.
*/
void run_ai()
{
	ai_play();

	/*=== TODO: Implement Wordle AI ===*/
}

// -------------------------------------------------------------------------------------------------------------------------------- //

/*
	The Main function for the Wordle Sim.
	Simply runs the `play_game` function from the ConsoleGame.h file.
*/
void run_sim()
{
	play_game();
}

// ================================================================================================================================ //

/*
	The main function where the program starts.
	This Program tests the Wordle Sim by letting the user play it in the console.
*/
int main()
{
	// Attempt to launch the Wordle AI or Sim.
	try
	{
		run_ai(); // Should call either `run_sim` or `run_ai` from here.
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
