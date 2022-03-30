#include "WordleSim.h"

#include <stdexcept>
#include <algorithm>
#include <random>
#include <fstream>

// ================================================================================================================================ //

Results::Results(std::size_t count)
	:
	results(count)
{}

// -------------------------------------------------------------------------------------------------------------------------------- //

bool Results::is_won() const noexcept
{
	for (const Result result : results)
	{
		if (result != Result::Correct) return false;
	}
	return true;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::string Results::str() const
{
	constexpr std::string_view chars{ "X-O" };

	std::string tmp(size(), 0);
	for (std::size_t i{}; i < size(); ++i)
	{
		tmp.at(i) = chars.at(std::size_t(results.at(i)));
	}
	return tmp;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

Result& Results::at(const std::size_t i)
{
	return results.at(i);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

const Result& Results::at(const std::size_t i) const
{
	return results.at(i);
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::size_t Results::size() const noexcept
{
	return results.size();
}

// -------------------------------------------------------------------------------------------------------------------------------- //

Results::iterator Results::begin() const noexcept
{
	return results.cbegin();
}

// -------------------------------------------------------------------------------------------------------------------------------- //

Results::iterator Results::end() const noexcept
{
	return results.cend();
}

// ================================================================================================================================ //

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

Results WordleSim::make_guess(const std::string_view guess)
{
	if (guess.size() != word.size())
		throw std::runtime_error("Guess size does not equal Word size.");

	Results results{ word.size() };

	for (std::size_t wi{}; wi < word.size(); ++wi)
	{
		const auto wc{ std::tolower(word.at(wi)) };
		
		auto count{ std::count_if(word.begin(), word.end(), [wc](const char chr) { return std::tolower(chr) == wc; }) };

		const auto assign = [&](const bool same_position, const Result result)
		{
			for (std::size_t gi{}; gi < word.size(); ++gi)
			{
				if (count == 0) break;

				const auto gc{ std::tolower(guess.at(gi)) };

				if (((wi == gi) == same_position) && (wc == gc))
				{
					results.at(gi) = std::max(results.at(gi), result);
					--count;
				}
			}
		};

		assign(true, Result::Correct);
		assign(false, Result::Exists);
	}

	++try_count;
	return results;
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::size_t WordleSim::word_length() const noexcept
{
	return word.size();
}

// -------------------------------------------------------------------------------------------------------------------------------- //

std::size_t WordleSim::tries() const noexcept
{
	return try_count;
}

// ================================================================================================================================ //
