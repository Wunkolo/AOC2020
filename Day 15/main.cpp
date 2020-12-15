#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <unordered_map>

int main()
{
	std::unordered_map<std::uintmax_t, std::vector<std::intmax_t>> SaidNumbers;
	std::uintmax_t PrevNumber{}, CurTurn{};
	std::string CurToken;
	while( std::getline(std::cin, CurToken, ',') )
	{
		++CurTurn;
		PrevNumber = std::stoull(CurToken);
		SaidNumbers[PrevNumber].push_back(CurTurn);
	}

	++CurTurn;
	for(; true; ++CurTurn)
	{
		std::uintmax_t CurNumber{};
		if(SaidNumbers.count(PrevNumber) && SaidNumbers[PrevNumber].size() > 1)
		{
			CurNumber = std::abs(*SaidNumbers[PrevNumber].rbegin() - *(SaidNumbers[PrevNumber].rbegin() + 1));
		}
		SaidNumbers[CurNumber].push_back(CurTurn);
		PrevNumber = CurNumber;
		//if(CurTurn == 2021)
		//std::cout << CurNumber << std::endl;
		switch(CurTurn)
		{
			case 2020		:{ std::cout << CurNumber << std::endl; break;}
			case 30000000	:{ std::cout << CurNumber << std::endl; return 0;}
		}
	}
}