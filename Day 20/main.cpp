#include <bits/stdc++.h> 

struct TileT
{
	std::array<std::array<bool, 10>, 10> BitField;
	void FlipH()
	{
		for(auto& Row : BitField)
		{
			std::reverse(Row.begin(), Row.end());
		}
	}
	void FlipV()
	{
		std::reverse(BitField.begin(), BitField.end());
	}

	void RotateCW()
	{
		std::array<std::array<bool, 10>, 10> RotatedBitField;
		for(std::size_t i = 0; i < 10; ++i)
		{
			for(std::size_t j = 0; j < 10; ++j)
			{
				RotatedBitField[i][j] = BitField[10 - j - 1][i];
			}
		}
		BitField.swap(RotatedBitField);
	}

	bool MatchTop(std::uint16_t Target)
	{
		for(std::size_t i = 0; i < 2; ++i)
		{
			for(std::size_t j = 0; j < 2; ++j)
			{
				for(std::size_t k = 0; k < 4; ++k)
				{
					if(TopEdge() == Target) return true;
					RotateCW();
				}
				FlipH();
			}
			FlipH();
			FlipV();
		}
		return false;
	}
	
	bool MatchLeft(std::uint16_t Target)
	{
		for(std::size_t i = 0; i < 2; ++i)
		{
			for(std::size_t j = 0; j < 2; ++j)
			{
				for(std::size_t k = 0; k < 4; ++k)
				{
					if(LeftEdge() == Target) return true;
					RotateCW();
				}
				FlipH();
			}
			FlipH();
			FlipV();
		}
		return false;
	}

	// Top, Bottom, Left, Right
	std::array<std::uint16_t, 4> EdgeIDs;

	std::uint16_t TopEdge() const
	{
		std::uint16_t Mask{};
		for( std::size_t i = 0; i < 10; i++ )
		{
			Mask |= (BitField[0][i] << i);
		}
		return Mask;
	}
	std::uint16_t BottomEdge() const
	{
		std::uint16_t Mask{};
		for( std::size_t i = 0; i < 10; i++ )
		{
			Mask |= (BitField[9][i] << i);
		}
		return Mask;
	}
	std::uint16_t LeftEdge() const
	{
		std::uint16_t Mask{};
		for( std::size_t i = 0; i < 10; i++ )
		{
			Mask |= (BitField[i][0] << i);
		}
		return Mask;
	}
	std::uint16_t RightEdge() const
	{
		std::uint16_t Mask{};
		for( std::size_t i = 0; i < 10; i++ )
		{
			Mask |= (BitField[i][9] << i);
		}
		return Mask;
	}
};

std::uint16_t Reverse10(std::uint16_t Input)
{
	std::uint16_t Reversed = 0;
	std::size_t BitCount = 10;

	while (BitCount--)
	{
		Reversed = (Reversed << 1) | (Input & 1);
		Input >>= 1;
	}
	return Reversed;
}

int main()
{
	std::unordered_map<std::uintmax_t, TileT> Tiles;
	while( std::cin )
	{
		std::uintmax_t TileID{};
		std::scanf("Tile %ju: ", &TileID);
		TileT CurTile{};
		std::string CurLine;
		std::size_t CurRow = 0;
		while( std::getline(std::cin, CurLine) && CurLine.size() )
		{
			for( std::size_t CurCol = 0; CurCol < 10; ++CurCol )
			{
				CurTile.BitField[CurRow][CurCol] = (CurLine[CurCol] == '#');
			}
			++CurRow;
		}

		CurTile.EdgeIDs[0] = CurTile.TopEdge();
		CurTile.EdgeIDs[1] = CurTile.BottomEdge();
		CurTile.EdgeIDs[2] = CurTile.LeftEdge();
		CurTile.EdgeIDs[3] = CurTile.RightEdge();
		Tiles[TileID] = CurTile;
	}
	// Use the 10 bits of each of the tile's borders, and generate
	// all the "flipped/rotated" permutations of it
	std::unordered_map<std::uint16_t, std::size_t> EdgeHistogram;
	// For fast lookups
	std::multimap<std::uint16_t, TileT*> EdgeToTile;
	for( auto& CurTile : Tiles )
	{
		for( const auto& Edge : CurTile.second.EdgeIDs )
		{
			++EdgeHistogram[Edge];
			++EdgeHistogram[Reverse10(Edge)];
			EdgeToTile.insert({Edge, &CurTile.second});
			EdgeToTile.insert({Reverse10(Edge), &CurTile.second});
		}
	}

	// TileID to edges
	std::array<std::array<std::set<std::uintmax_t>, 13>, 13> PossibleEdges;
	std::set<std::uintmax_t> CornerEdges, BorderEdges, InteriorEdges;
	std::set<std::uintmax_t> CornerTiles, BorderTiles, InteriorTiles;

	std::uintmax_t Part1 = 1;
	for( const auto& CurTile : Tiles )
	{
		// Right,Left,Bottom,Top
		std::unordered_set<std::uintmax_t> UniqueEdges;
		std::uintmax_t UniqueEdgeMask{};
		for( std::size_t EdgeIdx = 0; EdgeIdx < 4; ++EdgeIdx )
		{
			const auto& Edge = CurTile.second.EdgeIDs[EdgeIdx];
			const std::uintmax_t EdgeCount = EdgeHistogram.at(Edge);
			const std::uintmax_t EdgeReversedCount = EdgeHistogram.at(Reverse10(Edge));
			if(EdgeCount == 1 && EdgeReversedCount == 1)
			{
				// This edge is shared with no one else
				UniqueEdgeMask |= (1 << EdgeIdx);
				UniqueEdges.insert(Edge); UniqueEdges.insert(Reverse10(Edge));
			}
		}
		switch(std::popcount(UniqueEdgeMask))
		{
			case 0:
			{
				// Interior piece
				for(const auto& Edge : CurTile.second.EdgeIDs)
				{
					InteriorEdges.insert(Edge);
					InteriorEdges.insert(Reverse10(Edge));
				}
				InteriorTiles.insert(CurTile.first);
				break;
			}
			case 1:
			{
				// Border piece
				BorderEdges.insert(UniqueEdges.cbegin(), UniqueEdges.cend());
				BorderTiles.insert(CurTile.first);
				break;
			}
			case 2:
			{
				// Corner piece
				Part1 *= CurTile.first;
				CornerEdges.insert(UniqueEdges.cbegin(), UniqueEdges.cend());
				CornerTiles.insert(CurTile.first);
				break;
			}
		}
	}
	std::cout << Part1 << std::endl;

	// Part2
	std::array<std::array<const TileT*, 12>, 12> SolvedTiles{};
	for( std::size_t Y = 0; Y < 12; ++Y )
	{
		for( std::size_t X = 0; X < 12; ++X )
		{
			if( X == 0 && Y == 0)
			{
				// Pick a Corner piece, and make it our 0,0 "pivot"
				TileT& PivotTile = Tiles.at(*CornerTiles.begin());
				// Rotate/flip it until its top and left edge are untouched
				{
					std::size_t PermutationID = 0;
					while(
						(EdgeHistogram.count(PivotTile.TopEdge()) > 1) &&
						(EdgeHistogram.count(PivotTile.LeftEdge()) > 1)
					)
					{
						PivotTile.RotateCW();
						if(PermutationID >= 4) PivotTile.FlipH();
						++PermutationID;
					}
					SolvedTiles[0][0] = &PivotTile;
					//std::cout << Y << ' ' << X << '|' << SolvedTiles[0][0] << std::endl;
				}
			}
			// Columns to the right of the pivot
			else if( Y == 0 )
			{
				// Match our "current left edge" to the "left's right edge"
				const auto TargetTile = SolvedTiles[0][X - 1];
				const auto Target = TargetTile->RightEdge();
				
				TileT* Self{};
				{
					auto CurTile = EdgeToTile.lower_bound(Target);
					const auto End = EdgeToTile.upper_bound(Target);
					for(;CurTile != End; ++CurTile)
					{
						if((*CurTile).second != TargetTile)
						{
							Self = (*CurTile).second; break;
						}
					}
				}
				//std::cout << Y << ' ' << X << '|' << TargetTile << std::endl;
				assert(Self->MatchLeft(Target));
				SolvedTiles[0][X] = Self;
				EdgeToTile.erase(TargetTile->TopEdge());
			}
			// All tiles below the top row
			else
			{
				//std::cout << Y << ' ' << X << '|' << SolvedTiles[Y - 1][X] << std::endl;
				const auto TargetAboveTile = SolvedTiles[Y - 1][X];
				const auto TargetAbove = TargetAboveTile->BottomEdge();
				TileT* Self{};
				{
					auto CurTile = EdgeToTile.lower_bound(TargetAbove);
					const auto End = EdgeToTile.upper_bound(TargetAbove);
					for(;CurTile != End; ++CurTile)
					{
						if((*CurTile).second != TargetAboveTile)
						{
							Self = (*CurTile).second; break;
						}
					}
				}
				assert(Self->MatchTop(TargetAbove));
				SolvedTiles[Y][X] = Self;
			}
		}
	}

	std::uintmax_t TotalRough{};
	std::array<std::array<bool, 96>, 96> FullMap{};
	for( std::size_t TileY = 0; TileY < 12; ++TileY )
	{
		for( std::size_t TileX = 0; TileX < 12; ++TileX )
		{
			const TileT* CurTile = SolvedTiles[TileY][TileX];
			for( std::size_t Y = 0; Y < 8; ++Y )
			{
				for( std::size_t X = 0; X < 8; ++X )
				{
					TotalRough += 
						(FullMap[TileY * 8 + Y][TileX * 8 + X] = CurTile->BitField[1 + Y][1 + X]);
				}
			}
		}
	}
	

	// Find the dragon
	//                  # 
	//#    ##    ##    ###
	// #  #  #  #  #  #   
	constexpr static std::array<std::array<bool, 20>, 3> Dragon{{
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
		{1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,1},
		{0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0}
	}};

	std::uintmax_t DragonSigma{};
	for(std::size_t PermFlip = 0; PermFlip < 2; ++PermFlip)
	{
		for(std::size_t PermRotate = 0; PermRotate < 4; ++PermRotate)
		{
			// Print current map permutation
			for( std::size_t Y = 0; Y < 96; ++Y )
			{
				for( std::size_t X = 0; X < 96; ++X )
				{
					std::cout << (FullMap[Y][X] ? '#':'.');
				}
				std::cout << std::endl;
			}

			for( std::size_t MapY = 0; MapY < 96 - 3; ++MapY )
			{
				for( std::size_t MapX = 0; MapX < 96 - 20; ++MapX )
				{
					std::uintmax_t CurDragonSum{};
					for( std::size_t DragonY = 0; DragonY < 3; ++DragonY )
					{
						for( std::size_t DragonX = 0; DragonX < 20; ++DragonX )
						{
							const auto DragonBit = Dragon[DragonY][DragonX];
							const auto MapBit = FullMap[MapY + DragonY][MapX + DragonX];
							// Opt: Sum of absolute difference comparison
							// Dragon and map both true
							if(DragonBit && MapBit) ++CurDragonSum;
						}
					}
					if(CurDragonSum == 15) DragonSigma += 15;
				}
			}
			if(DragonSigma) break;
			// Rotate clockwise
			{
				std::array<std::array<bool, 96>, 96> RotateFullMap{};
				for(std::size_t i = 0; i < 96; ++i)
				{
					for(std::size_t j = 0; j < 96; ++j)
					{
						RotateFullMap[i][j] = FullMap[96 - j - 1][i];
					}
				}
				FullMap.swap(RotateFullMap);
			}
		}
		// Flip Horizontally
		{
			for( auto& MapRow : FullMap )
			{
				std::reverse(MapRow.begin(), MapRow.end());
			}
		}
	}
	std::cout << (TotalRough - DragonSigma) << std::endl;
}