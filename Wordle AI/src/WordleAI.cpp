#include "WordleAI.h"

#include <random>
#include <algorithm>
#include <iostream>

#include "WordleSim.h"

// ================================================================================================================================ //

WordleAI::WordleAI(const Dictionary& dict_g) 
	:
	dict{ dict_g }
{}

// ================================================================================================================================ //

std::string WordleAI::makeGuess(const std::size_t try_count)
{
	//Let's make a max priority queue, let's assign one point for each unique consonant and two for each unique vowel.

	std::size_t topValue = 0;
	std::string bestGuess = "";

	for (std::size_t i = 0; i < dict.size(); i++)
	{
		std::string temp = dict.at(i); 
		std::size_t value = 0;
		std::vector<char> letters;

		for (auto &ch : temp) {
			if (std::find(letters.begin(), letters.end(), ch) == letters.end()) {
				letters.push_back(ch);
			}
		}

		for (char c : letters) {
			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
				value += 2;
			}
			else {
				value += 1;
			}
		}

		if (value > topValue) {
			bestGuess = temp;
			topValue = value;
		}
	}
	std::cout << bestGuess;

	return bestGuess;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::string WordleAI::randomGuess()
{
	std::string guess{};
	std::sample(dict.begin(), dict.end(), &guess, 1, std::random_device{});
	return guess;
}

// ================================================================================================================================ //

void WordleAI::updateDictionary(const Results& feedback, const std::string& guess)
{
	for (std::size_t i{}; i < feedback.size(); i++)
	{
		const Feedback f{ feedback.at(i) };
		const char letter{ f.letter };
		const Result result{ f.result };
		
		switch (result)
		{
			case Result::Correct:
			{
				erase_if([=](const std::string& word) { return word.at(i) != letter; });

				const auto count{ std::count(guess.begin(), guess.end(), letter) };
				erase_if([=](const std::string& word) { return std::count(word.begin(), word.end(), letter) < count; });
			}
			break;

			case Result::Exists:
			{
				erase_if([=](const std::string& word) { return word.at(i) == letter; });

				const auto count{ std::count(guess.begin(), guess.end(), letter) };
				erase_if([=](const std::string& word) { return std::count(word.begin(), word.end(), letter) < count; });
			}
			break;

			case Result::Invalid:
			{
				erase_if([=](const std::string& word) { return word.at(i) == letter; });

				const auto count{ std::count_if(feedback.begin(), feedback.end(), [=](const Feedback fb) { return (fb.letter == letter) && (fb.result != result); }) };
				erase_if([=](const std::string& word) { return std::count(word.begin(), word.end(), letter) != count; });
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
	Results feedback{ sim.word_length() };

	WordleAI ai{ dict_g };

	std::cout << "\n==== WORDLE AI ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues infinitely until the Game is Won.
	for (int i = 0; i < 6 && !feedback.is_won(); i++)
	{
		std::cout << "\n-- Turn " << (sim.tries() + 1) << " --\n";
		std::cout << "AI Dictionary Size: " << ai.dict.size() << "\n";

		const std::string guess{ ai.makeGuess(sim.tries()) };
		std::cout << "  Guess: " << guess << '\n';

		feedback = sim.make_guess(guess);
		std::cout << "Results: " << feedback.str() << '\n';
		
		ai.updateDictionary(feedback, guess);
	}
	if(feedback.is_won()) std::cout << "\n==== YOU WIN! ====\n\n";
	else std::cout << "\n==== YOU LOSE! ====\n\n";

}

// ================================================================================================================================ //
