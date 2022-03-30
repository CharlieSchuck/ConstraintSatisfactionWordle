#pragma once

#include <cstddef>

#include <vector>
#include <string>
#include <string_view>

// ================================================================================================================================ //

// Forward-Declaration
class WordleSim;

// -------------------------------------------------------------------------------------------------------------------------------- //

enum class Result
{
	Invalid,
	Exists,
	Correct,
};

// -------------------------------------------------------------------------------------------------------------------------------- //

class Results
{
	friend class WordleSim;

private: // Variables

	std::vector<Result> results;

public: // Data Structures

	using iterator = std::vector<Result>::const_iterator;

public: // Functions

	Results(std::size_t count);

	bool is_won() const noexcept;
	std::string str() const;

	Result& at(const std::size_t i);
	const Result& at(const std::size_t i) const;
	
	std::size_t size() const noexcept;
	
	iterator begin() const noexcept;
	iterator end() const noexcept;

};

// ================================================================================================================================ //

using Dictionary = std::vector<std::string>;

Dictionary load_dictionary(const char* const filename);

// ================================================================================================================================ //

class WordleSim
{
private: // Variables

	std::string word;
	std::size_t try_count;

public: // Functions

	WordleSim(const Dictionary& dictionary);

	Results make_guess(const std::string_view guess);

	std::size_t word_length() const noexcept;

	std::size_t tries() const noexcept;

};

// ================================================================================================================================ //
