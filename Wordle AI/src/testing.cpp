#include "testing.h"

#include <iomanip>
#include <fstream>
#include <sstream>
#include <chrono>

// This Macro can be set externally with compilation flags.
#ifndef NO_MULTITHREADING
#  include <execution>
#endif

// ================================================================================================================================ //

void play_ai(const DictType type, const std::size_t suggested_length, const std::string_view suggested_word)
{
	const std::string word{ !suggested_word.empty() ? std::string(suggested_word) : pick_word(load_answers(type, suggested_length)) };
	const Dictionary dict_g{ load_guesses(type, word.size()) };
	const DictionaryView dict_v{ dict_g };

	WordleSim sim{ pick_word(dict_g, word) };
	WordleAI ai{ dict_v, sim.word_length() };

	std::cout << "\n==== WORDLE AI ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues until the Game is Won.
	Results feedback{ sim.word_length() };
	while (!feedback.is_won() /*&& sim.tries() < 6 */)
	{
		std::cout << "\n-- Turn " << (sim.tries() + 1) << " --\n";
		std::cout << "AI Dictionary Size: " << ai.dict.size() << "\n";

		const std::string& guess{ ai.makeGuess(sim.tries()) };
		std::cout << "  Guess: " << guess << '\n';

		feedback = sim.make_guess(guess);
		std::cout << "Results: " << feedback.str() << '\n';

		ai.updateDictionary(feedback);
	}

	if (feedback.is_won())
	{
		std::cout << "\n==== YOU WIN! ====\n\n";
	}
	else
	{
		std::cout << "\n==== YOU LOSE! ====\t (The Answer was: " << sim.answer() << ")\n\n";
	}
}

// -------------------------------------------------------------------------------------------------------------------------------- //

void test_ai(const DictType type, const std::size_t word_length)
{
	const Dictionary dict_a{ load_answers(type, word_length) };
	const Dictionary dict_g{ load_guesses(type, word_length) };
	const DictionaryView dict_v{ dict_g };

	std::cout << "\n=========================== WORDLE AI TEST ===========================\n\n";

	std::cout << "Testing AI on all words in '" << dictionary_name(type, word_length) << "'...\n";

	const auto start_time{ std::chrono::steady_clock::now() };

	std::vector<unsigned char> games(dict_a.size());

#ifndef NO_MULTITHREADING
	std::transform(std::execution::par_unseq, dict_a.begin(), dict_a.end(), games.begin(), [&](const std::string& word)
#else
	std::transform(dict_a.begin(), dict_a.end(), games.begin(), [&](const std::string& word)
#endif
	{
		WordleSim sim{ word };
		WordleAI ai{ dict_v, sim.word_length() };

		Results feedback{ sim.word_length() };
		while (!feedback.is_won())
		{
			const std::string& guess{ ai.makeGuess(sim.tries()) };
			feedback = sim.make_guess(guess);
			ai.updateDictionary(feedback);
		}
		
		return static_cast<unsigned char>(sim.tries());
	});

	Stats stats{};
	for (const auto tries : games)
	{
		const bool won{ tries <= 6 };

		if (won)
			stats.add_win(tries);
		else
			stats.add_loss(tries);
	}

	const auto end_time{ std::chrono::steady_clock::now() };
	const std::chrono::duration<double> elapsed_time{ end_time - start_time };


	std::ostringstream o_games{};
	std::ostringstream w_games{}; 
	std::ostringstream l_games{};
	o_games << stats.total_games() << " games";
	w_games << stats.wins << " games";
	l_games << stats.losses << " games";

	std::ostringstream w_ratio{};
	std::ostringstream l_ratio{};
	w_ratio << '(' << stats.win_ratio() << "%)";
	l_ratio << '(' << stats.loss_ratio() << "%)";
	
	std::ostringstream o_turns{};
	std::ostringstream w_turns{};
	std::ostringstream l_turns{};
	o_turns << '[' << stats.average_turns() << " Turn Avg.]";
	w_turns << '[' << stats.average_turns_wins() << " Turn Avg.]";
	l_turns << '[' << stats.average_turns_losses() << " Turn Avg.]";

	const std::string og_str{ o_games.str() };
	const std::string or_str{ "" };
	const std::string ot_str{ o_turns.str() };
	
	const std::string wg_str{ w_games.str() };
	const std::string wr_str{ w_ratio.str() };
	const std::string wt_str{ w_turns.str() };
	
	const std::string lg_str{ l_games.str() };
	const std::string lr_str{ l_ratio.str() };
	const std::string lt_str{ l_turns.str() };

	const std::size_t g_size{ std::max({ og_str.size(), wg_str.size(), lg_str.size() }) };
	const std::size_t r_size{ std::max({ or_str.size(), wr_str.size(), lr_str.size() }) };
	const std::size_t t_size{ std::max({ ot_str.size(), wt_str.size(), lt_str.size() }) };

	std::ostringstream statstream{};
	statstream
		<< "========================== WORDLE AI  STATS ==========================" << '\n'
		<< '\n'
		<< dictionary_name(type, word_length) << '\n'
		<< "* Took " << elapsed_time.count() << " seconds." << '\n'
		<< '\n'
		<< "Overall:   " << std::setw(g_size) << og_str << "   " << std::setw(r_size) << or_str << "   " << std::setw(t_size) << ot_str << '\n'
		<< "   Wins:   " << std::setw(g_size) << wg_str << "   " << std::setw(r_size) << wr_str << "   " << std::setw(t_size) << wt_str << '\n'
		<< " Losses:   " << std::setw(g_size) << lg_str << "   " << std::setw(r_size) << lr_str << "   " << std::setw(t_size) << lt_str << '\n'
		<< '\n'
		<< "======================================================================" << '\n';

	std::cout << '\n' << statstream.str() << '\n';

	const std::string filename{ std::string("./Tests/") + dictionary_name(type, word_length) + std::string(" Results.txt")};
	std::ofstream file{ filename };
	
	if (!file)
	{
		throw std::runtime_error("Unable to open output file.");
	}
	else
	{
		std::cout << "Writing results to '" << filename << "'...\n";
		
		file << statstream.str() << '\n';

		file << "\n==== GAME RESULTS ====\n\n";

		for (std::size_t i{}; i < dict_a.size(); ++i)
		{
			const std::string& word{ dict_a[i] };
			const std::size_t tries{ static_cast<std::size_t>(games[i]) };
			const bool won{ tries <= 6 };
		
			file << word << ": " << tries << " turns  [" << (won ? "WIN" : "LOSE") << "]\n";
		}

		file << "\n======================\n";
		
		std::cout << "Done!\n";
	}

	std::cout << "\n======================================================================\n\n";
}

// ================================================================================================================================ //

// Prompts the User to enter a Guess until valid input is received.
std::string read_guess(const Dictionary& dict, const std::size_t length);

// -------------------------------------------------------------------------------------------------------------------------------- //

void play_sim(const DictType type, const std::size_t suggested_length, const std::string_view suggested_word)
{
	const std::string word{ !suggested_word.empty() ? std::string(suggested_word) : pick_word(load_answers(type, suggested_length)) };
	const Dictionary dict_g{ load_guesses(type, word.size()) };

	WordleSim sim{ pick_word(dict_g, word) };

	std::cout << "\n==== WORDLE SIM ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues infinitely until the Game is Won.
	Results feedback{ sim.word_length() };
	while (!feedback.is_won())
	{
		std::cout << "\n\n-- Guess " << (sim.tries() + 1) << " --\n";

		const std::string guess{ read_guess(dict_g, sim.word_length()) };

		feedback = sim.make_guess(guess);

		std::cout << "  Guess: " << guess << '\n';
		std::cout << "Results: " << feedback.str() << '\n';
	}

	std::cout << "\n\n==== YOU WIN! ====\n\n";
}

// -------------------------------------------------------------------------------------------------------------------------------- //

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

// ================================================================================================================================ //

inline constexpr auto& help_message
{
R"(
================ Wordle AI - Help ================

==== DICTIONARIES ====

-- Wordle --
Description: The Wordle Dictionaries of 5-letter words pulled from the New York Times's website.

-- Scrabble --
Description: The full Scrabble Dictionary of words of various lengths pulled from https://scrabble.merriam.com/.


==== COMMANDS ====

-- play --
Description: Has the AI play a single game and prints the full game out to the console.
Syntax: play <Dictionary> [Word Length or Word]
(REQUIRED) <Dictionary>  : One of the two dictionaries listed above.
(OPTIONAL) [Word Length] : If specified, picks a random word from the dictionary of the specified length.
(OPTIONAL) [Word]        : If specified, plays the AI against the given word.
Ex: play Wordle
Ex: play Scrabble heaps

-- test --
Description: Tests the AI against every word in a dictionary and writes the statistics to a file.
Syntax: test <Dictionary> [Word Length]
(REQUIRED) <Dictionary>  : One of the two dictionaries listed above.
(OPTIONAL) [Word Length] : If specified, only tests words of the specified length.
Ex: test Wordle
Ex: test Scrabble 5

-- quit --
Description: Terminates the program.

==================================================
)"
};

// -------------------------------------------------------------------------------------------------------------------------------- //

void prompt_user()
{
	std::cout << help_message << '\n';

	while (true)
	{
		std::string line{};
		std::string command{};
		std::string dictionary{};
		std::string arg{};
		std::string excess{};

		while (line.empty())
		{
			std::cout << "\nPlease enter a command: ";
			std::getline(std::cin, line);

			if (!std::cin.good())
				throw std::runtime_error("Standard Input stream failure.");
		}

		make_lowercase(line);
		std::istringstream stream{ line };
		stream >> command >> dictionary >> arg >> excess;

		if (command.empty())
		{
			std::cout << "ERROR: No command was specified.\n";
			continue;
		}
		
		if (command == "quit")
		{
			std::cout << "\nTerminating Wordle-AI program...\n";
			return;
		}
		else if ((command != "play") && (command != "test"))
		{
			std::cout << "ERROR: Invalid command specified.\n";
			continue;
		}

		if (dictionary.empty())
		{
			std::cout << "ERROR: No dictionary was specified.\n";
			continue;
		}
		if (!excess.empty())
		{
			std::cout << "ERROR: Too many arguments provided.\n";
			continue;
		}

		DictType type{};
		if (dictionary == "wordle")
		{
			type = DictType::Wordle;
		}
		else if (dictionary == "scrabble")
		{
			type = DictType::Scrabble;
		}
		else
		{
			std::cout << "ERROR: Invalid dictionary specified.\n";
			continue;
		}

		const bool is_number{ !arg.empty() && std::all_of(arg.begin(), arg.end(), std::isdigit) };
		const bool is_word{ !arg.empty() && std::all_of(arg.begin(), arg.end(), std::isalpha) };
		if (!arg.empty() && !is_number && !is_word)
		{
			std::cout << "ERROR: Invalid argument specified.\n";
			continue;
		}

		const std::size_t length{ is_number ? std::stoi(arg) : any_length };

		if (command == "play")
		{
			const std::string word{ is_word ? arg : "" };
			play_ai(type, length, word);
		}
		else if (command == "test")
		{
			test_ai(type, length);
		}
		else
		{
			
		}
	}
}

// ================================================================================================================================ //
