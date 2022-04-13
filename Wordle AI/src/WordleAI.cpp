#include "WordleAI.h"
#include "WordleSim.h"

#include <algorithm>
#include <iostream>
#include <queue>
// ================================================================================================================================ //
WordleAI::WordleAI(const Dictionary& dict_g) 
	: dict{ dict_g } { //weird and gross. Man, C++ is the worst. 

}

std::string WordleAI::makeGuess(std::size_t try_count) {
	//Local BEAM! -- find the four guesses that give the most information in the dictionary at hand.
	//generate dictionaries that are the consequences of each of those guesses... find the four strongest in each of those dictionaries
	//so on and so forth equal to the number of guesses remaining... 

	for (std::size_t i = 0; i < dict.size(); i++) {

	}

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
				auto n = std::count_if(feedback.begin(), feedback.end(), [=](Feedback fb) { return (fb.letter == p) && (fb.result != q); });
				erase_if([=](std::string& word) { return std::count_if(word.begin(), word.end(), p) != n; });
				break;
			}
			
		}

	}
}

void ai_play() {

	// Dictionary of Hidden Words to be chosen.
	const Dictionary dict_a{ load_dictionary("../Dictionaries/wordle-answers.txt") };

	// Dictionary of Valid Word to be guessed.
	const Dictionary dict_g{ load_dictionary("../Dictionaries/wordle-guesses.txt") };

	WordleAI ai{ dict_g };
	WordleSim sim{ dict_a };
	Results feedback{ sim.word_length() };

	std::cout << "\n==== WORDLE SIM ====\n";
	std::cout << "\nWord Length is " << sim.word_length() << '\n';

	// Play continues infinitely until the Game is Won.
	while (!feedback.is_won())
	{
		std::cout << "\n\n-- Guess " << (sim.tries() + 1) << " --\n";

		std::string guess = ai.makeGuess(sim.tries());
		feedback = sim.make_guess(guess);
		ai.updateDictionary(feedback, guess);


		std::cout << "  Guess: " << guess << '\n';
		std::cout << "Results: " << feedback.str() << '\n';
	}
	std::cout << "\n\n==== YOU WIN! ====\n\n";

}

// ================================================================================================================================ //
