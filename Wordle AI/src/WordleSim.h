#pragma once

#include <cstddef>

#include <vector>
#include <string>
#include <string_view>

#include "Dictionary.h"

// ================================================================================================================================ //

// Feedback for each letter in a guess.
enum class Result : unsigned char
{
	Invalid, // There are no more of the Letter.
	Exists,  // Letter is in the wrong position.
	Correct, // Letter is in the right position.
};

// -------------------------------------------------------------------------------------------------------------------------------- //

// A combination of a Letter and its Result.
struct Feedback
{
	char letter;
	Result result;
};

// -------------------------------------------------------------------------------------------------------------------------------- //

// Forward-Declaration.
class WordleSim;

// A Collection of "Feedback"s.
class Results
{
	// Allows WordleSim to modify Results to return after each guess.
	friend class WordleSim;

private: // Variables

	// Holds all the individual "Feedback"s.
	std::vector<Feedback> feedbacks;

public: // Data Structures

	// Type Alias for the iterator returned by begin()/end().
	using iterator = decltype(feedbacks)::const_iterator;

public: // Functions

	// Constructor (takes the number of letters in the guess).
	Results(const std::size_t count);

	// Returns true/false depending on if all letters are Correct.
	bool is_won() const noexcept;

	// Returns a Textual Representation of the results.
	std::string str() const;

	
	// Returns a reference to the Result at index i.
	Feedback& at(const std::size_t i);

	// Returns a const reference to the Result at index i.
	const Feedback& at(const std::size_t i) const;

	Feedback& operator[](const std::size_t i);

	const Feedback& operator[](const std::size_t i) const;


	// Returns the number of letters in the guess.
	std::size_t size() const noexcept;
	
	// Returns the begin iterator for the underlying container.
	iterator begin() const noexcept;

	// Returns the end iterator for the underlying container.
	iterator end() const noexcept;

};

// ================================================================================================================================ //

// Used to Run a Simulation of a Wordle Game.
class WordleSim
{
private: // Variables

	// The Word that is to be guessed.
	const std::string& word;

	// The number of guesses made so far.
	std::size_t try_count;

public: // Functions

	// Constructor
	WordleSim(const std::string& word);

	// Takes a String as a Guess and returns a Results representing how accurate the guess was.
	Results make_guess(const std::string& guess);

	// Returns the length of the word to be guessed.
	std::size_t word_length() const noexcept;

	// Returns the number of guesses made so far.
	std::size_t tries() const noexcept;

	// Returns the correct answer.
	const std::string& answer() const noexcept;

};

// ================================================================================================================================ //
