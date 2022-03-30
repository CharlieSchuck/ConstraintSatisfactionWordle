#include "WordleSim.h"

#include <iostream>
#include <sstream>

// ================================================================================================================================ //

void make_uppercase(std::string& str)
{
	for (char& chr : str)
		chr = char(std::toupper(chr));
}

// -------------------------------------------------------------------------------------------------------------------------------- //

void make_lowercase(std::string& str)
{
	for (char& chr : str)
		chr = char(std::tolower(chr));
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::string read_guess(const Dictionary& dict, const std::size_t length)
{
	std::string guess{};

	while (true)
	{
		std::string line{};
		std::cout << "\nPlease enter a Guess: ";
		std::getline(std::cin, line);
		std::istringstream stream{ line };
		stream >> guess;

		if (stream >> guess)
		{
			std::cout << "\nERROR! Too many words were input.\n";
			continue;
		}

		if (guess.size() != length)
		{
			std::cout << "\nERROR! Word must be " << length << " letters. [Not " << guess.size() << "]\n";
			continue;
		}

		make_lowercase(guess);

		if (std::find(dict.begin(), dict.end(), guess) == dict.end())
		{
			std::cout << "\nERROR! \"" << guess << "\" could not be found in dictionary.\n";
			continue;
		}

		break;
	}
	std::cout << '\n';

	make_uppercase(guess);
	return guess;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

void play_game()
{
	const Dictionary dict{ load_dictionary("../../Dictionaries/dictionary5.txt") };
	WordleSim sim{ dict };
	Results results{ sim.word_length() };

	std::cout << "\n==== WORDLE SIM ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	while (!results.is_won())
	{
		std::cout << "\n\n-- Guess " << (sim.tries() + 1) << " --\n";

		const std::string guess{ read_guess(dict, sim.word_length()) };

		results = sim.make_guess(guess);

		std::cout << "  Guess: " << guess << '\n';
		std::cout << "Results: " << results.str() << '\n';
	}

	std::cout << "\n\n==== YOU WIN! ====\n\n";
}

// ================================================================================================================================ //
