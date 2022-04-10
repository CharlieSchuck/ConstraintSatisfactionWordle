#include "WordleAI.h"
#include "WordleSim.h"

#include <algorithm>
#include <iostream>
// ================================================================================================================================ //
struct Feedback {
	char letter;
	Result result;
};
// ================================================================================================================================ //
WordleAI::WordleAI() {
	
}

std::string WordleAI::makeGuess() {

}

void WordleAI::updateDictionary(Results results, std::string& guess) {
	for (std::size_t i = 0; i < guess.size(); i++) {
		char p = guess.at(i);
		Result q = results.at(i);
		
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
				int n = std::count(guess.begin(), guess.end(), p);
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
	Results results{ sim.word_length() };

	std::cout << "\n==== WORDLE SIM ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues infinitely until the Game is Won.
	while (!results.is_won())
	{
		std::cout << "\n\n-- Guess " << (sim.tries() + 1) << " --\n";

		std::string& guess = ai.makeGuess();
		results = sim.make_guess(guess);
		ai.updateDictionary(results, guess);


		std::cout << "  Guess: " << guess << '\n';
		std::cout << "Results: " << results.str() << '\n';
	}
	std::cout << "\n\n==== YOU WIN! ====\n\n";

}

// ================================================================================================================================ //
