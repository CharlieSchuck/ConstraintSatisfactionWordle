#include "Dictionary.h"

#include <stdexcept>
#include <fstream>
#include <random>
#include <string>

// ================================================================================================================================ //

Dictionary load_dictionary(const char* const filename, const std::size_t word_length)
{
	Dictionary dict{};

	std::ifstream file{ filename };
	if (!file)
		throw std::runtime_error("Unable to open Dictionary File.");

	constexpr auto is_normal = [](const unsigned char chr)
	{
		return (chr < 0x80) && std::isalpha(chr);
	};

	while (file.good())
	{
		std::string word{};
		file >> word;

		const bool lengths_match{ (word_length == any_length) || (word.size() == word_length) };

		if (!word.empty() && lengths_match)
		{
			if (std::all_of(word.begin(), word.end(), is_normal))
			{
				make_lowercase(word);
				dict.push_back(word);
			}
			else
			{
				throw std::runtime_error("Irregular characters found in word from dictionary.");
			}
		}
	}

	if (dict.empty())
		throw std::runtime_error("Dictionary does not contain any words of the given length.");

	std::sort(dict.begin(), dict.end());

	return dict;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

Dictionary load_answers(const DictType type, const std::size_t word_length)
{
	switch (type)
	{
	case DictType::Wordle:   return load_dictionary("../Dictionaries/wordle-answers.txt", word_length);
	case DictType::Scrabble: return load_dictionary("../Dictionaries/scrabble-dict.txt", word_length);
	}
	throw std::runtime_error("Invalid Answers Dictionary.");
}

// -------------------------------------------------------------------------------------------------------------------------------- //

Dictionary load_guesses(const DictType type, const std::size_t word_length)
{
	switch (type)
	{
	case DictType::Wordle:   return load_dictionary("../Dictionaries/wordle-guesses.txt", word_length);
	case DictType::Scrabble: return load_dictionary("../Dictionaries/scrabble-dict.txt", word_length);
	}
	throw std::runtime_error("Invalid Guesses Dictionary.");
}

// ================================================================================================================================ //

std::string pick_word(const Dictionary& dict)
{
	if (dict.empty())
	{
		throw std::runtime_error("Empty Dictionary was given.");
	}
	else
	{
		std::string guess{};
		std::sample(dict.begin(), dict.end(), &guess, 1, std::random_device{});
		return guess;
	}
}

// -------------------------------------------------------------------------------------------------------------------------------- //

const std::string& pick_word(const Dictionary& dict, const std::string& word)
{
	if (!std::binary_search(dict.begin(), dict.end(), word))
	{
		throw std::runtime_error("Given word was not found in Dictionary.");
	}
	else
	{
		return word;
	}
}

// ================================================================================================================================ //
