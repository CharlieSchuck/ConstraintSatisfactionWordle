#pragma once

#include <cstddef>

#include <vector>
#include <string>
#include <string_view>

#include <algorithm>

// ================================================================================================================================ //

/*
	Holds a collection of Words.
	Is assumed to be in All Lowercase and Alphabetical Order.
*/
class Dictionary : public std::vector<std::string>
{
public:

	// Erases the words that meet the given condition from the dictionary.
	template<class Predicate>
	inline void erase_if(Predicate&& condition)
	{
		erase(std::remove_if(begin(), end(), condition), end());
	}

};

// -------------------------------------------------------------------------------------------------------------------------------- //

class DictionaryView : public std::vector<const std::string*>
{
public:

	inline DictionaryView(const Dictionary& dict)
	{
		this->reserve(dict.size());
		for (const auto& elem : dict)
		{
			this->push_back(&elem);
		}
	}

	// Erases the words that meet the given condition from the dictionary.
	template<class Predicate>
	inline void erase_if(Predicate&& condition)
	{
		erase(std::remove_if(begin(), end(), condition), end());
	}

};

// ================================================================================================================================ //

enum class DictType { Wordle, Scrabble };

// -------------------------------------------------------------------------------------------------------------------------------- //

inline constexpr DictType default_dictionary{ DictType::Wordle };

inline constexpr std::size_t default_length{ 5 };
inline constexpr std::size_t any_length{ 0 };

// ================================================================================================================================ //

// Attempts to load a Dictionary from the given file.
Dictionary load_dictionary(const char* const filename, const std::size_t word_length = any_length, const bool allow_empty = false);

// Loads a Dictionary of Words that can be chosen as the Answer.
Dictionary load_answers(const DictType type, const std::size_t word_length = any_length, const bool allow_empty = false);

// Loads a Dictionary of Words that are valid guesses.
Dictionary load_guesses(const DictType type, const std::size_t word_length = any_length, const bool allow_empty = false);

// ================================================================================================================================ //

// Picks a random word from the dictionary.
std::string pick_word(const Dictionary& dict);

// Verifies that the word is actually in the dictionary before returning it.
const std::string& pick_word(const Dictionary& dict, const std::string& word);

// ================================================================================================================================ //

// Converts the given String to Lowercase.
inline void make_lowercase(std::string& str)
{
	for (char& chr : str)
		chr = static_cast<char>(std::tolower(chr));
}

// -------------------------------------------------------------------------------------------------------------------------------- //

// Converts the given String to Uppercase.
inline void make_uppercase(std::string& str)
{
	for (char& chr : str)
		chr = static_cast<char>(std::toupper(chr));
}

// ================================================================================================================================ //

inline std::string dictionary_name(const DictType type, const std::size_t word_length)
{
	const std::string length_str{ word_length == any_length ? "All" : std::to_string(word_length) };

	switch (type)
	{
	case DictType::Wordle:   return std::string("Wordle Dictionary - ") + length_str;
	case DictType::Scrabble: return std::string("Scrabble Dictionary - ") + length_str;
	}
	return "Invalid Dictionary";
}

// ================================================================================================================================ //
