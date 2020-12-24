#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

static glm::ivec2 Offsets[] {
	{  2,  0 }, // e
	{ -2,  0 }, // w
	{  1, -1 }, // se
	{  1,  1 }, // ne
	{ -1, -1 }, // sw
	{ -1,  1 }  // nw
};

std::uintmax_t Simulate(std::unordered_set<glm::ivec2> BlackTiles)
{
	for( std::size_t Day = 0; Day < 100; ++Day )
	{
		std::unordered_map<glm::ivec2, std::uint8_t> NeighborSums;
		for( const auto& CurTile : BlackTiles )
		{
			// Each black tile adds 1 to all neighbor tiles
			for( const auto& Offset : Offsets ) ++NeighborSums[CurTile + Offset];
		}
		std::unordered_set<glm::ivec2> NewTiles;
		// Iterate each tile-sum
		// Determine which tiles to add
		for( auto& CurSum : NeighborSums )
		{
			// White tile with 2 neighbors
			if( CurSum.second == 2 && !BlackTiles.count(CurSum.first))
			{
				// Turn it black
				NewTiles.insert(CurSum.first);
			}
		}
		// Iterate previously black tiles
		for( auto& CurTile : BlackTiles )
		{
			// Black tiles at least one or two other black tiles stay black tiles
			if( NeighborSums[CurTile] == 1 || NeighborSums[CurTile] == 2 )
			{
				NewTiles.insert(CurTile);
			}
		}
		BlackTiles = NewTiles;
	}

	return BlackTiles.size();
}

int main()
{
	std::unordered_set<glm::ivec2> BlackTiles;
	{
		std::string CurLine;
		while( std::getline(std::cin, CurLine) )
		{
			glm::ivec2 CurCoord{};
			const std::regex DirectionPattern("(e)|(w)|(se)|(ne)|(sw)|(nw)", std::regex::optimize);
			for(
				auto Match = std::sregex_iterator(CurLine.cbegin(), CurLine.cend(), DirectionPattern);
				Match != std::sregex_iterator(); ++Match
			)
			{
				const std::smatch CurMatch = *Match;
				std::size_t MatchIndex = 1;
				for( ;MatchIndex < CurMatch.size(); ++MatchIndex )
				{
					if(CurMatch[MatchIndex].matched) break;
				}
				CurCoord += Offsets[--MatchIndex];
			}
			if(BlackTiles.count(CurCoord)) BlackTiles.erase(CurCoord);
			else BlackTiles.insert(CurCoord);
		}
	}

	std::cout << BlackTiles.size() << std::endl;
	std::cout << Simulate(BlackTiles) << std::endl;
}