#pragma once

#include <cstddef>

#include <string>
#include <algorithm>

#include "Dictionary.h"
#include "WordleSim.h"

// ================================================================================================================================ //

class WordleAI
{
public: // Variables

	// The Dictionary holding valid guesses.
	Dictionary dict;

public: // Functions

	// Constructs the AI with the given Dictionary.
	WordleAI(const Dictionary& dict_g, const std::size_t word_length);

	// Returns a Guess from the Dictionary.
	std::string makeGuess(const std::size_t try_count);

	// Returns a Random Word from the Dictionary.
	std::string randomGuess();

	// Analyzes the Feedback on the given guess and updates the Dictionary accordingly.
	void updateDictionary(const Results& feedback, const std::string& guess);



};

// ================================================================================================================================ //
