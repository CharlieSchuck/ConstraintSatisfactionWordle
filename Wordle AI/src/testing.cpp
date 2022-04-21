#include "testing.h"

#include <sstream>
#include <chrono>

// ================================================================================================================================ //

void play_ai(const DictType type, const std::size_t suggested_length, const std::string_view suggested_word)
{
	const std::string word{ !suggested_word.empty() ? std::string(suggested_word) : pick_word(load_answers(type, suggested_length)) };
	const Dictionary dict_g{ load_guesses(type, word.size()) };

	WordleSim sim{ pick_word(dict_g, word) };
	WordleAI ai{ dict_g, sim.word_length() };

	std::cout << "\n==== WORDLE AI ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues until the Game is Won.
	Results feedback{ sim.word_length() };
	while (!feedback.is_won() /*&& sim.tries() < 6 */)
	{
		std::cout << "\n-- Turn " << (sim.tries() + 1) << " --\n";
		std::cout << "AI Dictionary Size: " << ai.dict.size() << "\n";

		const std::string guess{ ai.makeGuess(sim.tries()) };
		std::cout << "  Guess: " << guess << '\n';

		feedback = sim.make_guess(guess);
		std::cout << "Results: " << feedback.str() << '\n';

		ai.updateDictionary(feedback, guess);
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
	
	Stats stats{};

	std::cout << "\n==== WORDLE AI TEST ====\n\n";

	const auto start_time{ std::chrono::steady_clock::now() };

	for (const std::string& word : dict_a)
	{
		WordleSim sim{ word };
		WordleAI ai{ dict_g, sim.word_length() };

		std::cout << sim.answer() << ": ";

		Results feedback{ sim.word_length() };
		while (!feedback.is_won())
		{
			const std::string guess{ ai.makeGuess(sim.tries()) };
			feedback = sim.make_guess(guess);
			ai.updateDictionary(feedback, guess);
		}

		const bool won{ sim.tries() <= 6 };

		if (won)
			stats.add_win(sim.tries());
		else
			stats.add_loss(sim.tries());

		std::cout << sim.tries() << " turns. [" << (won ? "WIN" : "LOSE") << "]\n";
	}

	const auto end_time{ std::chrono::steady_clock::now() };
	const std::chrono::duration<double> elapsed_time{ end_time - start_time };

	std::cout
		<< '\n'
		<< "========================== WORDLE AI  STATS ==========================" << '\n'
		<< '\n'
		<< dictionary_name(type, word_length) << '\n'
		<< "* Took " << elapsed_time.count() << " seconds." << '\n'
		<< '\n'
		<< "Overall:\t" << stats.total_games() << " games\t\t\t[" << stats.average_turns() << " Turn Avg.]" << '\n'
		<< "   Wins:\t" << stats.wins << " games\t(" << stats.win_ratio() << "%)\t[" << stats.average_turns_wins() << " Turn Avg.]" << '\n'
		<< " Losses:\t" << stats.losses << " games\t(" << stats.loss_ratio() << "%)\t[" << stats.average_turns_losses() << " Turn Avg.]" << '\n'
		<< '\n'
		<< "======================================================================" << '\n'
		<< '\n';
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
