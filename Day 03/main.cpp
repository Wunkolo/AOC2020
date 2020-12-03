#include <cstdio>
#include <iostream>
#include <cstdint>
#include <string>
#include <numeric>
#include <vector>

struct Toboggan
{
	std::uintmax_t SlopeX{}, SlopeY{}, CurPosX{}, CurPosY{}, Trees;
};

int main(int argc, char *argv[])
{
	std::vector<Toboggan> SlopeSums = {
		{1, 1},
		{3, 1},
		{5, 1},
		{7, 1},
		{1, 2}
	};

	std::vector<std::string> Forest;
	std::string CurLine;
	while( std::cin >> CurLine ) Forest.push_back(CurLine);

	for( std::size_t i = 0; i < Forest.size(); ++i )
	{
		for( auto& CurSlope : SlopeSums )
		{
			CurSlope.CurPosX += CurSlope.SlopeX;
			CurSlope.CurPosY += CurSlope.SlopeY;
			if( CurSlope.CurPosY < Forest.size())
			{
				CurSlope.Trees += Forest[CurSlope.CurPosY][CurSlope.CurPosX % CurLine.length()] == '#';
			}
		}
	}

	for( auto& CurSlope : SlopeSums )
	{
		std::printf(
			"%ju:%ju : %ju\n", CurSlope.SlopeX, CurSlope.SlopeY, CurSlope.Trees
		);
	}
	std::printf(
		"%ju",
		std::accumulate(
			SlopeSums.cbegin(), SlopeSums.cend(), 1,
			[](auto Pi, const auto& Toboggan){ return Pi * Toboggan.Trees;}
		)
	);
}
