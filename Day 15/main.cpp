#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

// struct Ring2{}

int main()
{
	// Todo, Hashmap of 2-element circular buffers?
	// std::unordered_map<std::uintmax_t, Ring2> SaidNumbers;
	std::unordered_map<std::uintmax_t, std::vector<std::intmax_t>> SaidNumbers;
	std::uintmax_t PrevNumber{}, CurTurn{};
	std::string CurToken;
	while( std::getline(std::cin, CurToken, ',') )
	{
		++CurTurn;
		PrevNumber = std::stoull(CurToken);
		SaidNumbers[PrevNumber].push_back(CurTurn);
	}

	++CurTurn; // 1-indexed -_-
	for(; true; ++CurTurn)
	{
		std::uintmax_t CurNumber{};
		if(SaidNumbers.count(PrevNumber) && SaidNumbers[PrevNumber].size() > 1)
		{
			// Ring2 Sum-absolute-difference?
			CurNumber = std::abs(*SaidNumbers[PrevNumber].rbegin() - *(SaidNumbers[PrevNumber].rbegin() + 1));
		}
		// Push to Ring2 here
		SaidNumbers[CurNumber].push_back(CurTurn);
		PrevNumber = CurNumber;
		switch(CurTurn)
		{
			case 2020		:{ std::cout << CurNumber << std::endl; break;}
			case 30000000	:{ std::cout << CurNumber << std::endl; return 0;}
		}
	}
}