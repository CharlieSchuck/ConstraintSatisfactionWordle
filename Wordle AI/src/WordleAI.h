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
	DictionaryView dict;

	const DictionaryView& full_dict;

	bool invalidated[26];

public: // Functions

	// Constructs the AI with the given Dictionary.
	WordleAI(const DictionaryView& dict_g, const std::size_t word_length);

	// Returns a Guess from the Dictionary.
	const std::string& makeGuess(const std::size_t try_count);

	// Returns a Random Word from the Dictionary.
	const std::string& randomGuess();

	// Analyzes the Feedback on the given guess and updates the Dictionary accordingly.
	void updateDictionary(const Results& feedback);

};

// ================================================================================================================================ //
