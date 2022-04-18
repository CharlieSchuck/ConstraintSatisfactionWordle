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

// Constructor (uses the provided word).
WordleSim::WordleSim(const std::string_view word)
	:
	word{ word }, try_count{}
{}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Takes a String as a Guess and returns a Results representing how accurate the guess was.
Results WordleSim::make_guess(const Word guess)
{
	if (guess->size() != word.size())
		throw std::runtime_error("Guess size does not equal Word size.");

	// All Results are initialized to Invalid by default.
	Results feedback{ word.size() };

	// For all Letters in the Hidden Word.
	for (std::size_t wi{}; wi < word.size(); ++wi)
	{
		// Assign letter from guess to feedback.
		feedback.at(wi).letter = guess->at(wi);
		
		const auto current_letter{ std::tolower(word.at(wi)) };

		// Number of occurrences of the current Letter.
		// Counts down as Result values are assigned later.
		auto count{ std::count(word.begin(), word.end(), current_letter) };

		// Lambda Function that assigns results based on whether or not letters are in the right position.
		const auto assign = [&](const bool should_match, const Result result)
		{
			// For all Letters in the Guess.
			for (std::size_t gi{}; gi < word.size(); ++gi)
			{
				// All non-Invalid Results have been assigned, all else must be Invalid.
				if (count == 0) break;

				const auto wchar{ std::tolower(word.at(gi)) };
				const auto gchar{ std::tolower(guess->at(gi)) };
				if (gchar != current_letter) continue;
				
				if ((gchar == wchar) == should_match)
				{
					feedback.at(gi).result = result;
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

// -------------------------------------------------------------------------------------------------------------------------------- //

std::string_view WordleSim::answer() const noexcept
{
	return word;
}

// ================================================================================================================================ //
