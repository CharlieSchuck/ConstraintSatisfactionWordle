#include "WordleAI.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <set>

#include "WordleSim.h"

// ================================================================================================================================ //

WordleAI::WordleAI(const Dictionary& dict_g, const std::size_t word_length)
	:
	dict{ dict_g }
{
	erase_if([=](const std::string& word) { return word.size() != word_length; });
}

// ================================================================================================================================ //

std::string WordleAI::makeGuess([[maybe_unused]] const std::size_t try_count)
{
	if (dict.empty())
		throw std::runtime_error("AI Dictionary is empty.");

	constexpr auto is_vowel = [](const char c)
	{
		return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
	};

	// Let's assign one point for each unique consonant and two for each unique vowel.

	std::size_t topValue{};
	std::string_view bestGuess{};

	for (const std::string& word : dict)
	{
		std::string letters{};
		letters.reserve(word.size());

		std::size_t value{};

		for (const char ch : word)
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

	return std::string(bestGuess);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::string WordleAI::randomGuess()
{
	std::string guess{};
	std::sample(dict.begin(), dict.end(), &guess, 1, std::random_device{});
	return guess;
}

// ================================================================================================================================ //

void WordleAI::updateDictionary(const Results& feedback, [[maybe_unused]] const std::string& guess)
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
				erase_if([=](const std::string& word) {
					return (word.at(i) != letter) || (std::count(word.begin(), word.end(), letter) < count);
				});
			}
			break;

			case Result::Exists:
			{
				erase_if([=](const std::string& word) {
					return (word.at(i) == letter) || (std::count(word.begin(), word.end(), letter) < count);
				});
			}
			break;

			case Result::Invalid:
			{
				erase_if([=](const std::string& word) {
					return (word.at(i) == letter) || (std::count(word.begin(), word.end(), letter) != count);
				});
			}
			break;
		}

	}
}

// ================================================================================================================================ //

void ai_play()
{
	// Dictionary of Hidden Words that can be chosen.
	const Dictionary dict_a{ load_dictionary("../Dictionaries/wordle-answers.txt") };

	// Dictionary of Valid Word that can be guessed.
	const Dictionary dict_g{ load_dictionary("../Dictionaries/wordle-guesses.txt") };

	WordleSim sim{ dict_a };
	WordleAI ai{ dict_g, sim.word_length() };

	std::cout << "\n==== WORDLE AI ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues until the Game is Won or 6 turns have passed.
	Results feedback{ sim.word_length() };
	while (/*(sim.tries() < 6) &&*/ !feedback.is_won())
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

// ================================================================================================================================ //

void ai_test()
{
#define USE_WORDLE

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


	std::size_t wins{};
	std::uintmax_t total_turns{};

	std::cout << "\n==== WORDLE AI TEST ====\n\n";

	for (const std::string& word : dict_a)
	{
		WordleSim sim{ { word } };
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
		if (won) ++wins;

		total_turns += sim.tries();

		std::cout << sim.tries() << " turns. [" << (won ? "WIN" : "LOSE") << "]\n";
	}

	const std::size_t total_games{ dict_a.size() };
	const std::size_t losses{ total_games - wins };
	const double avg_turns{ double(total_turns) / double(total_games) };
	const double win_ratio{ 100.0 * double(wins) / double(total_games) };

	std::cout << "\n==== WORDLE AI STATS ====\n\n";
	std::cout << " Words: " << total_games << '\n';
	std::cout << "  Wins: " << wins << '\n';
	std::cout << "Losses: " << losses << '\n';
	std::cout << " Win %: " << win_ratio << '%' << '\n';
	std::cout << "Avg. Turns: " << avg_turns << '\n';
	std::cout << "\n=========================\n\n";
}

// ================================================================================================================================ //
