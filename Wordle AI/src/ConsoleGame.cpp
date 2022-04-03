#include "WordleSim.h"

#include <algorithm>
#include <iostream>
#include <sstream>

// ================================================================================================================================ //

// Converts the given String to Uppercase.
void make_uppercase(std::string& str)
{
	for (char& chr : str)
		chr = char(std::toupper(chr));
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Converts the given String to Lowercase.
void make_lowercase(std::string& str)
{
	for (char& chr : str)
		chr = char(std::tolower(chr));
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Prompts the User to enter a Guess until valid input is received.
std::string read_guess(const Dictionary& dict, const std::size_t length)
{
	std::string guess{};

	// Loop until valid input.
	while (true)
	{
		// Read in an entire line.
		std::cout << "\nPlease enter a Guess: ";
		std::string line{};
		std::getline(std::cin, line);

		// Read only the first word in the given line.
		std::istringstream stream{ line };
		stream >> guess;

		// Check if more than 1 word was input.
		if (stream >> guess)
		{
			std::cout << "\nERROR! Too many words were input.\n";
			continue;
		}

		// Check for improperly sized guesses.
		if (guess.size() != length)
		{
			std::cout << "\nERROR! Word must be " << length << " letters. [Not " << guess.size() << "]\n";
			continue;
		}

		// Convert to Lowercase.
		make_lowercase(guess);

		// Check if the Dictionary contains the Guess.
		if (!std::binary_search(dict.begin(), dict.end(), guess))
		{
			std::cout << "\nERROR! \"" << guess << "\" could not be found in dictionary.\n";
			continue;
		}

		// Input is valid, break the loop.
		break;
	}
	std::cout << '\n';

	// Convert to Uppercase.
	make_uppercase(guess);
	return guess;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Launches a Console Game version of Wordle for the User to Play.
void play_game()
{
	// Dictionary of Hidden Words to be chosen.
	const Dictionary dict_a{ load_dictionary("../Dictionaries/wordle-answers.txt") };

	// Dictionary of Valid Word to be guessed.
	const Dictionary dict_g{ load_dictionary("../Dictionaries/wordle-guesses.txt") };


	WordleSim sim{ dict_a };
	Results results{ sim.word_length() };

	std::cout << "\n==== WORDLE SIM ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues infinitely until the Game is Won.
	while (!results.is_won())
	{
		std::cout << "\n\n-- Guess " << (sim.tries() + 1) << " --\n";

		const std::string guess{ read_guess(dict_g, sim.word_length()) };

		results = sim.make_guess(guess);

		std::cout << "  Guess: " << guess << '\n';
		std::cout << "Results: " << results.str() << '\n';
	}

	std::cout << "\n\n==== YOU WIN! ====\n\n";
}

// ================================================================================================================================ //
