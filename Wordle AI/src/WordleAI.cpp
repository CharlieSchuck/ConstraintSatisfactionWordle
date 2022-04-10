#include "WordleAI.h"
#include "WordleSim.h"

#include <algorithm>
#include <iostream>
// ================================================================================================================================ //
WordleAI::WordleAI() {
	
}

std::string WordleAI::makeGuess() {
	return ""; //"just to appease the compiler".
}

void WordleAI::updateDictionary(Results feedback, std::string& guess) {
	for (std::size_t i = 0; i < feedback.size(); i++) {
		Feedback f = feedback.at(i);
		char p = f.letter;
		Result q = f.result;
		
		switch (q) {
			case Result::Correct: {
				erase_if([=](std::string& word) { return word.at(i) != p; });
				break;
			}

			case Result::Exists: {
				erase_if([=](std::string& word) { return word.at(i) == p; });
				break;
			}

			case Result::Invalid: {
				erase_if([=](std::string& word) { return word.at(i) == p; });
				auto n = std::count(guess.begin(), guess.end(), p);
				erase_if([=](std::string& word) { return std::count(word.begin(), word.end(), p) == n; });
				break;
			}
			
		}

	}
}

void ai_play() {
	WordleAI ai{};

	// Dictionary of Hidden Words to be chosen.
	const Dictionary dict_a{ load_dictionary("../Dictionaries/wordle-answers.txt") };

	// Dictionary of Valid Word to be guessed.
	const Dictionary dict_g{ load_dictionary("../Dictionaries/wordle-guesses.txt") };


	WordleSim sim{ dict_a };
	Results feedback{ sim.word_length() };

	std::cout << "\n==== WORDLE SIM ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues infinitely until the Game is Won.
	while (!feedback.is_won())
	{
		std::cout << "\n\n-- Guess " << (sim.tries() + 1) << " --\n";

		std::string guess = ai.makeGuess();
		feedback = sim.make_guess(guess);
		ai.updateDictionary(feedback, guess);


		std::cout << "  Guess: " << guess << '\n';
		std::cout << "Results: " << feedback.str() << '\n';
	}
	std::cout << "\n\n==== YOU WIN! ====\n\n";

}

// ================================================================================================================================ //
