#include "WordleAI.h"

#include <chrono>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <random>
#include <set>

#include "WordleSim.h"

// ================================================================================================================================ //

// ==== OPTIONS ====
// USE_WORDLE		: Plays using Wordle Dictionary.
// USE_SCRABBLE_5	: Plays using 5-letter Scrabble Dictionary.
// USE_SCRABBLE		: Plays using full Scrabble Dictionary.

#define USE_SCRABBLE

// ================================================================================================================================ //

WordleAI::WordleAI(const DictionaryView& dict_g, const std::size_t word_length)
	:
	dict{ dict_g }
{
	erase_if([=](const Word word) { return word->size() != word_length; });
}

// ================================================================================================================================ //

Word WordleAI::makeGuess([[maybe_unused]] const std::size_t try_count)
{
	if (dict.empty())
		throw std::runtime_error("AI Dictionary is empty.");

	constexpr auto is_vowel = [](const char c)
	{
		return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
	};

	// Let's assign one point for each unique consonant and two for each unique vowel.

	std::size_t topValue{};
	Word bestGuess{};

	for (const Word word : dict)
	{
		std::string letters{};
		letters.reserve(word->size());

		std::size_t value{};

		for (const char ch : *word)
		{
			if (std::find(letters.begin(), letters.end(), ch) == letters.end())
			{
				letters.push_back(ch);
				value += is_vowel(ch) ? 2 : 1;
			}
		}

		if (value > topValue)
		{
			bestGuess = word;
			topValue = value;
		}
	}

	return bestGuess;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

Word WordleAI::randomGuess()
{
	Word guess{};
	std::sample(dict.begin(), dict.end(), &guess, 1, std::random_device{});
	return guess;
}

// ================================================================================================================================ //

void WordleAI::updateDictionary(const Results& feedback, [[maybe_unused]] const Word guess)
{
	for (std::size_t i{}; i < feedback.size(); i++)
	{
		const Feedback f{ feedback.at(i) };
		const char letter{ f.letter };
		const Result result{ f.result };
		
		// Number of non-invalid occurrences of the current letter in the guess.
		const auto count{ std::count_if(feedback.begin(), feedback.end(),
			[=](const Feedback fb) { return (fb.letter == letter) && (fb.result != Result::Invalid); }
		)};

		switch (result)
		{
			case Result::Correct:
			{
				erase_if([=](const Word word) {
					return (word->at(i) != letter) || (std::count(word->begin(), word->end(), letter) < count);
				});
			}
			break;

			case Result::Exists:
			{
				erase_if([=](const Word word) {
					return (word->at(i) == letter) || (std::count(word->begin(), word->end(), letter) < count);
				});
			}
			break;

			case Result::Invalid:
			{
				erase_if([=](const Word word) {
					return (word->at(i) == letter) || (std::count(word->begin(), word->end(), letter) != count);
				});
			}
			break;
		}

	}
}

// ================================================================================================================================ //

void ai_play()
{
	// dict_a - Dictionary of Hidden Words that can be chosen.
	// dict_g - Dictionary of Valid Word that can be guessed.
#ifdef USE_WORDLE
	const Dictionary dict_a{ load_dictionary("../Dictionaries/wordle-answers.txt") };
	const Dictionary dict_g{ load_dictionary("../Dictionaries/wordle-guesses.txt") };
#endif
#ifdef USE_SCRABBLE_5
	const Dictionary dict_a{ load_dictionary("../Dictionaries/scrabble-dict5.txt") };
	const Dictionary dict_g{ load_dictionary("../Dictionaries/scrabble-dict5.txt") };
#endif
#ifdef USE_SCRABBLE
	const Dictionary dict_a{ load_dictionary("../Dictionaries/scrabble-dict.txt") };
	const Dictionary dict_g{ load_dictionary("../Dictionaries/scrabble-dict.txt") };
#endif

	const DictionaryView view_a{ dict_view(dict_a) };
	const DictionaryView view_g{ dict_view(dict_g) };


	WordleSim sim{ "aa" };
	WordleAI ai{ view_g, sim.word_length() };

	std::cout << "\n==== WORDLE AI ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues until the Game is Won.
	Results feedback{ sim.word_length() };
	while (!feedback.is_won() /*&& (sim.tries() < 6)*/)
	{
		std::cout << "\n-- Turn " << (sim.tries() + 1) << " --\n";
		std::cout << "AI Dictionary Size: " << ai.dict.size() << "\n";

		const Word guess{ ai.makeGuess(sim.tries()) };
		std::cout << "  Guess: " << *guess << '\n';

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

// ================================================================================================================================ //

template <typename... Args>
void print_out(std::ostream& console, std::ostream& file, Args&&... args)
{
	(console << ... << args);
	//(file << ... << args);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

void ai_test()
{
	// dict_a - Dictionary of Hidden Words that can be chosen.
	// dict_g - Dictionary of Valid Word that can be guessed.
#ifdef USE_WORDLE
	const Dictionary dict_a{ load_dictionary("../Dictionaries/wordle-answers.txt") };
	const Dictionary dict_g{ load_dictionary("../Dictionaries/wordle-guesses.txt") };
#endif
#ifdef USE_SCRABBLE_5
	const Dictionary dict_a{ load_dictionary("../Dictionaries/scrabble-dict5.txt") };
	const Dictionary dict_g{ load_dictionary("../Dictionaries/scrabble-dict5.txt") };
#endif
#ifdef USE_SCRABBLE
	const Dictionary dict_a{ load_dictionary("../Dictionaries/scrabble-dict.txt") };
	const Dictionary dict_g{ load_dictionary("../Dictionaries/scrabble-dict.txt") };
#endif

	const DictionaryView view_a{ dict_view(dict_a) };
	const DictionaryView view_g{ dict_view(dict_g) };


	std::ofstream out_file{ "tests/test-results.txt" };
	if (!out_file)
		throw std::runtime_error("Unable to create result-output file.");


	std::size_t wins{};
	std::uintmax_t total_turns{};

	const auto start_time{ std::chrono::steady_clock::now() };

	print_out(std::cout, out_file, "\n==== WORDLE AI TEST ====\n\n");

	for (const std::string& word : dict_a)
	{
		WordleSim sim{ word };
		WordleAI ai{ view_g, sim.word_length() };

		Results feedback{ sim.word_length() };
		while (!feedback.is_won())
		{
			const Word guess{ ai.makeGuess(sim.tries()) };
			feedback = sim.make_guess(guess);
			ai.updateDictionary(feedback, guess);
		}

		const bool won{ sim.tries() <= 6 };
		if (won) ++wins;

		total_turns += sim.tries();

		print_out(std::cout, out_file, sim.answer(), ": ", sim.tries(), " turns. [", (won ? "WIN" : "LOSE"), "]\n");
	}

	const auto end_time{ std::chrono::steady_clock::now() };
	const std::chrono::duration<double> elapsed_time{ end_time - start_time };

	const std::size_t total_games{ dict_a.size() };
	const std::size_t losses{ total_games - wins };
	const double avg_turns{ double(total_turns) / double(total_games) };
	const double win_ratio{ 100.0 * double(wins) / double(total_games) };

	print_out(std::cout, out_file, "\n==== WORDLE AI STATS ====\n\n");
	print_out(std::cout, out_file, " Words: ", total_games, '\n');
	print_out(std::cout, out_file, "  Wins: ", wins, '\n');
	print_out(std::cout, out_file, "Losses: ", losses, '\n');
	print_out(std::cout, out_file, " Win %: ", win_ratio, '%', '\n');
	print_out(std::cout, out_file, '\n');
	print_out(std::cout, out_file, "Avg. Turns: ", avg_turns, '\n');
	print_out(std::cout, out_file, '\n');
	print_out(std::cout, out_file, "Took ", elapsed_time.count(), " second(s)", '\n');
	print_out(std::cout, out_file, "\n=========================\n\n");
}

// ================================================================================================================================ //
