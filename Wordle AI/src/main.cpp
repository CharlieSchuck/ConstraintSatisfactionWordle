/*
	Charles Schuck - Eric Grandizio
	
	Wordle AI / Wordle Sim
*/

// ================================================================================================================================ //

#include "Testing.h"

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
		// Should call a function from `testing.h` here.
		test_ai(DictType::Scrabble, {});

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
