#include "WordleSim.h"

#include <stdexcept>
#include <algorithm>
#include <random>
#include <fstream>

// ================================================================================================================================ //

// Constructor (takes the number of letters in the guess).
Results::Results(const std::size_t count)
	:
	feedbacks(count)
{}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns true/false depending on if all letters are Correct.
bool Results::is_won() const noexcept
{
	for (const Feedback feedback : feedbacks)
	{
		if (feedback.result != Result::Correct) return false;
	}
	return true;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns a Textual Representation of the results.
std::string Results::str() const
{
	constexpr std::string_view chars{ "X-O" };

	std::string tmp(size(), 0);
	for (std::size_t i{}; i < size(); ++i)
	{
		// Result as Numerical Index.
		const auto result_i{ std::size_t(feedbacks.at(i).result) };

		tmp.at(i) = chars.at(result_i);
	}
	return tmp;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns a reference to the Result at index i.
Feedback& Results::at(const std::size_t i)
{
	return feedbacks.at(i);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns a const reference to the Result at index i.
const Feedback& Results::at(const std::size_t i) const
{
	return feedbacks.at(i);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns the number of letters in the guess.
std::size_t Results::size() const noexcept
{
	return feedbacks.size();
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns the begin iterator for the underlying container.
Results::iterator Results::begin() const noexcept
{
	return feedbacks.cbegin();
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns the end iterator for the underlying container.
Results::iterator Results::end() const noexcept
{
	return feedbacks.cend();
}

// ================================================================================================================================ //

// Attempts to load a Dictionary from the given file.
Dictionary load_dictionary(const char* const filename)
{
	Dictionary dictionary{};
	
	std::ifstream file{ filename };
	if (!file)
		throw std::runtime_error("Unable to open Dictionary File.");

	while (file.good())
	{
		std::string word{};
		file >> word;
			
		if (!word.empty())
			dictionary.push_back(word);
	}
	return dictionary;
}

// ================================================================================================================================ //

// Constructor (picks a random word from the given Dictionary).
WordleSim::WordleSim(const Dictionary& dictionary)
	:
	word{}, try_count{}
{
	if (dictionary.empty())
		throw std::runtime_error("Empty Dictionary was given.");

	std::uniform_int_distribution<std::size_t> distr{ 0, dictionary.size() - 1 };
	std::mt19937_64 rng{ std::random_device{}() };

	word = dictionary.at(distr(rng));
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Takes a String as a Guess and returns a Results representing how accurate the guess was.
Results WordleSim::make_guess(const std::string_view guess)
{
	if (guess.size() != word.size())
		throw std::runtime_error("Guess size does not equal Word size.");

	// All Results are initialized to Invalid by default.
	Results feedback{ word.size() };


	// For all Letters in the Hidden Word.
	for (std::size_t wi{}; wi < word.size(); ++wi)
	{
		// Assign letter from guess to feedback.
		feedback.at(wi).letter = guess.at(wi);


		// Hidden Word's current character as Lowercase.
		const auto wc{ std::tolower(word.at(wi)) };
		
		// Number of occurrences of the current Letter.
		// Counts down as Result values are assigned later.
		auto count{ std::count_if(word.begin(), word.end(), [wc](const char chr) { return std::tolower(chr) == wc; }) };

		// Lambda Function that assigns the given Result to all letters that in the Guess
		// that are/aren't in the same position as their corresponding letter in the Hidden Word.
		const auto assign = [&](const bool same_position, const Result result)
		{
			// For all Letters in the Guess.
			for (std::size_t gi{}; gi < word.size(); ++gi)
			{
				// All non-Invalid Results have been assigned, all else must be Invalid.
				if (count == 0) break;

				// Guess's current character as Lowercase.
				const auto gc{ std::tolower(guess.at(gi)) };

				// If the Letters match and their positions meet the given criteria...
				if (((wi == gi) == same_position) && (wc == gc))
				{
					// Assign the Result to the given letter.
					// max() is used to prevent overwriting Correct results with Exists.
					feedback.at(gi).result = std::max(feedback.at(gi).result, result);
					
					// Decrease the remaining occurrences of the letter by 1.
					--count;
				}
			}
		};

		// Must assign Correct results before assigning Exists results.
		assign(true, Result::Correct);
		assign(false, Result::Exists);
	}

	++try_count;
	return feedback;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns the length of the word to be guessed.
std::size_t WordleSim::word_length() const noexcept
{
	return word.size();
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Returns the number of guesses made so far.
std::size_t WordleSim::tries() const noexcept
{
	return try_count;
}

// ================================================================================================================================ //
