#pragma once

#include <cstddef>

#include <string>
#include <algorithm>

#include "Dictionary.h"
#include "WordleSim.h"

// ================================================================================================================================ //

/*
	The AI Class.
	It maintains a dictionary of valid guesses (among other info) in order to make "optimal guesses" and solve Wordle Puzzles.
*/
class WordleAI
{
public: // Variables

	// The Dictionary holding valid guesses.
	DictionaryView dict;

	// The Original DictionaryView that was used to start the AI.
	const DictionaryView& full_dict;

	// Array of characters that have been invalidated in the current game.
	bool invalidated[26];

public: // Functions

	// Constructs the AI with the given Dictionary.
	WordleAI(const DictionaryView& dict_g, const std::size_t word_length);

	// Returns a Guess from the Dictionary.
	const std::string& makeGuess(const std::size_t try_count);

	// Returns a Random Word from the Dictionary.
	const std::string& randomGuess();

	// Analyzes the Feedback from the Results of the previous guess and updates the Dictionary accordingly.
	void updateDictionary(const Results& feedback);

};

// ================================================================================================================================ //
