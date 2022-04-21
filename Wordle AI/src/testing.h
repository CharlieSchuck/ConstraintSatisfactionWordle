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

// ================================================================================================================================ //

struct Stats
{
public: // Variables

	std::uintmax_t wins;
	std::uintmax_t losses;
	std::uintmax_t win_turns;
	std::uintmax_t loss_turns;

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
		return 100.0 * double(wins) / double(total_games());
	}

	constexpr double loss_ratio() const noexcept
	{
		return 100.0 * double(losses) / double(total_games());
	}

	constexpr double average_turns() const noexcept
	{
		return double(total_turns()) / double(total_games());
	}
	
	constexpr double average_turns_wins() const noexcept
	{
		return double(win_turns) / double(wins);
	}

	constexpr double average_turns_losses() const noexcept
	{
		return double(loss_turns) / double(losses);
	}

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

};

// ================================================================================================================================ //
