#pragma once

#include <cstddef>

#include <string>
#include <string_view>

#include <iostream>
#include <exception>
#include <stdexcept>

#include "WordleSim.h"
#include "WordleAI.h"

// ================================================================================================================================ //

// Launches a Console Game version of Wordle for the AI to Play.
void play_ai(const DictType type = default_dictionary, const std::size_t suggested_length = default_length, const std::string_view suggested_word = "");

// Tests the AI on every word of the given length in the specified dictionary.
void test_ai(const DictType type = default_dictionary, const std::size_t word_length = default_length);

// Launches a Console Game version of Wordle for the User to Play.
void play_sim(const DictType type = default_dictionary, const std::size_t suggested_length = default_length, const std::string_view suggested_word = "");

// -------------------------------------------------------------------------------------------------------------------------------- //

// Displays a Help Message and allows the user to select how they want to run the AI.
void prompt_user();

// ================================================================================================================================ //

/*
	A utility class for storing the statistics of AI benchmarks.
*/
struct Stats
{
public: // Variables

	std::uintmax_t wins;
	std::uintmax_t losses;
	std::uintmax_t win_turns;
	std::uintmax_t loss_turns;

public: // Functions

	constexpr void add_win(const std::uintmax_t turns) noexcept
	{
		++wins;
		win_turns += turns;
	}

	constexpr void add_loss(const std::uintmax_t turns) noexcept
	{
		++losses;
		loss_turns += turns;
	}

	template <typename T>
	static constexpr double divide(const T a, const T b) noexcept
	{
		return (b == 0) ? (0.0) : (double(a) / double(b));
	}

public: // Getters

	constexpr std::uintmax_t total_games() const noexcept
	{
		return wins + losses;
	}

	constexpr std::uintmax_t total_turns() const noexcept
	{
		return win_turns + loss_turns;
	}

	constexpr double win_ratio() const noexcept
	{
		return 100.0 * divide(wins, total_games());
	}

	constexpr double loss_ratio() const noexcept
	{
		return 100.0 * divide(losses, total_games());
	}

	constexpr double average_turns() const noexcept
	{
		return divide(total_turns(), total_games());
	}
	
	constexpr double average_turns_wins() const noexcept
	{
		return divide(win_turns, wins);
	}

	constexpr double average_turns_losses() const noexcept
	{
		return divide(loss_turns, losses);
	}

};

// ================================================================================================================================ //
