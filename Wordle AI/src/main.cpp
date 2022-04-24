/*
	Charles Schuck - Eric Grandizio
	
	Wordle AI / Wordle Sim
*/

// ================================================================================================================================ //

#include "testing.h"

// ================================================================================================================================ //

/*
	The main function where the program starts.
	This Program tests the Wordle AI by letting the user enter commands for it in the console.
*/
int main()
{
	// Attempt to launch the Wordle AI in an interactive console.
	try
	{
		prompt_user();
		return 0;
	}
	// Log an error message if an exception occured.
	catch (const std::exception& ex)
	{
		std::cerr << "\n\nFATAL ERROR!\n" << ex.what() << "\n\n";
		return 1;
	}
	// Log an error message if an unknown exception occured.
	catch (...)
	{
		std::cerr << "\n\nUNKNOWN FATAL ERROR!\n\n";
		return 2;
	}
}

// ================================================================================================================================ //
