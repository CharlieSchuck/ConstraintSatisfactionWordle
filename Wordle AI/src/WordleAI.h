#pragma once

#include <cstddef>

#include <string>
#include <algorithm>

#include "WordleSim.h"

// ================================================================================================================================ //

class WordleAI
{
public: // Variables

	// The Dictionary holding valid guesses.
	DictionaryView dict;

public: // Functions

	// Constructs the AI with the given Dictionary.
	WordleAI(const DictionaryView& dict_g, const std::size_t word_length);

	// Returns a Guess from the Dictionary.
	Word makeGuess(const std::size_t try_count);

	// Returns a Random Word from the Dictionary.
	Word randomGuess();

	// Analyzes the Feedback on the given guess and updates the Dictionary accordingly.
	void updateDictionary(const Results& feedback, const Word guess);

	// Erases the words that meet the given condition from the dictionary.
	template<class Predicate>
	void erase_if(Predicate&& condition)
	{
		dict.erase(std::remove_if(dict.begin(), dict.end(), condition), dict.end());
	}

};

// ================================================================================================================================ //

// Runs the AI on a random word.
void ai_play();

// Runs the AI on all the words and logs statistics.
void ai_test();

// ================================================================================================================================ //
