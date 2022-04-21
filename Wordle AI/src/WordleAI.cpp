#include "WordleAI.h"

#include <iostream>
#include <algorithm>
#include <random>
#include <set>
#include <map>

#include "WordleSim.h"

// ================================================================================================================================ //

WordleAI::WordleAI(const Dictionary& dict_g, const std::size_t word_length)
	:
	dict{ dict_g }
{
	dict.erase_if([=](const std::string& word) { return word.size() != word_length; });
}

// ================================================================================================================================ //

std::string WordleAI::makeGuess([[maybe_unused]] const std::size_t try_count)
{
	if (dict.empty())
		throw std::runtime_error("AI Dictionary is empty.");

	constexpr auto is_vowel = [](const char c)
	{
		return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
	};

	// Let's assign one point for each unique consonant and two for each unique vowel.

	std::size_t topValue{};
	std::string_view bestGuess{};

	size_t lettersMap[26]{};

	for (const std::string& word : dict)
	{
		//let's count the letters. 
		for (const char ch : word)
		{
			//yeah, increment the thing. woop!
			++lettersMap[ch - 'a'];
		}
	}

	for (const std::string& word : dict)
	{
		bool lettersFound[26]{};

		std::size_t value{};
		for (const char ch : word)
		{
			bool& found{ lettersFound[ch - 'a'] };
			if (!found)
			{
				found = true;
				value += lettersMap[ch - 'a'];
			}
		}

		if (value > topValue)
		{
			bestGuess = word;
			topValue = value;
		}
	}

	return std::string(bestGuess);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::string WordleAI::randomGuess()
{
	std::string guess{};
	std::sample(dict.begin(), dict.end(), &guess, 1, std::random_device{});
	return guess;
}

// ================================================================================================================================ //

void WordleAI::updateDictionary(const Results& feedback, [[maybe_unused]] const std::string& guess)
{
	for (std::size_t i{}; i < feedback.size(); i++)
	{
		const Feedback f{ feedback.at(i) };
		const char letter{ f.letter };
		const Result result{ f.result };
		
		// Number of non-invalid occurrences of the current letter in the guess.
		const auto count{ std::count_if(feedback.begin(), feedback.end(),
			[=](const Feedback fb) { return (fb.letter == letter) && (fb.result != Result::Invalid); }
		)};

		switch (result)
		{
			case Result::Correct:
			{
				dict.erase_if([=](const std::string& word) {
					return (word.at(i) != letter) || (std::count(word.begin(), word.end(), letter) < count);
				});
			}
			break;

			case Result::Exists:
			{
				dict.erase_if([=](const std::string& word) {
					return (word.at(i) == letter) || (std::count(word.begin(), word.end(), letter) < count);
				});
			}
			break;

			case Result::Invalid:
			{
				dict.erase_if([=](const std::string& word) {
					return (word.at(i) == letter) || (std::count(word.begin(), word.end(), letter) != count);
				});
			}
			break;
		}

	}
}

// ================================================================================================================================ //
