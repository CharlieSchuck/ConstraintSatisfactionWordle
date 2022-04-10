#pragma once
#include <string>
#include "WordleSim.h"
// ================================================================================================================================ //

class WordleAI
{
public:
	Dictionary dict;
	WordleAI();
	std::string& makeGuess();
	void updateDictionary(Results results, std::string& guess);

	template<typename T>
	void erase_if(T condition) {
		dict.erase(std::remove_if(dict.begin(), dict.end(), condition), dict.end());
	}
};


void ai_play();

// ================================================================================================================================ //
