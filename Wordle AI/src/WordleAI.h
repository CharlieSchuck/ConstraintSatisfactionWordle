#pragma once
#include <string>
#include "WordleSim.h"
// ================================================================================================================================ //

class WordleAI
{
public:
	Dictionary dict;
	WordleAI(const Dictionary& dict_g);
	std::string makeGuess(std::size_t try_count);
	void updateDictionary(Results feedback, std::string& guess);

	template<typename T>
	void erase_if(T condition) {
		dict.erase(std::remove_if(dict.begin(), dict.end(), condition), dict.end());
	}
};


void ai_play();

// ================================================================================================================================ //
