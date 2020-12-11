#include <iostream>
#include <cstdio>
#include <array>
#include <algorithm>
#include <tuple>
#include <unordered_set>

std::uint64_t EncodePoint(std::uint32_t X, std::uint32_t Y)
{
	return (std::uint64_t(Y) << 32) | X;
}

std::tuple<std::uint32_t, std::uint32_t> DecodePoint(std::uint64_t Point)
{
	return {std::uint32_t(Point), std::uint32_t(Point >> 32)};
}
constexpr std::array<std::tuple<std::int32_t, std::int32_t>, 8> Offsets = {{
	{ 0,  1},
	{ 0, -1},
	{ 1,  0},
	{-1,  0},
	{ 1,  1},
	{-1, -1},
	{ 1, -1},
	{-1,  1}
}};

std::uintmax_t Simulate(
	std::unordered_set<std::uint64_t> EmptySeats,
	std::unordered_set<std::uint64_t> OccupiedSeats,
	std::uintmax_t SightRadius,
	std::uintmax_t EmptySeatTolerance
)
{
	
	std::uint32_t Changed{};
	do
	{
		Changed = 0;
		std::unordered_set<std::uint64_t> NewEmptySeats;
		std::unordered_set<std::uint64_t> NewOccupiedSeats;
		for(const auto EmptySeat : EmptySeats)
		{
			const auto [SeatX, SeatY] = DecodePoint(EmptySeat);
			std::uintmax_t OccupiedNeighbors{};
			for(const auto Offset : Offsets)
			{
				// Scan along this direction until we see an occupied seat
				for(std::int32_t i = 1; i <= SightRadius; ++i)
				{
					const auto TestPoint = EncodePoint(
						SeatX + std::get<0>(Offset) * i,
						SeatY + std::get<1>(Offset) * i
					);
					bool SightHit = OccupiedSeats.count(TestPoint);
					OccupiedNeighbors += SightHit;
					SightHit |= EmptySeats.count(TestPoint);
					if(SightHit) break;
				}
			}

			if(OccupiedNeighbors == 0)
			{
				NewOccupiedSeats.insert(EmptySeat); ++Changed;
			}
		}
		for(const auto OccupiedSeat : OccupiedSeats)
		{
			const auto [SeatX, SeatY] = DecodePoint(OccupiedSeat);
			std::uintmax_t OccupiedNeighbors{};
			for(const auto Offset : Offsets)
			{
				for(std::int32_t i = 1; i <= SightRadius; ++i)
				{
					const auto TestPoint = EncodePoint(
						SeatX + std::get<0>(Offset) * i,
						SeatY + std::get<1>(Offset) * i
					);
					bool SightHit = OccupiedSeats.count(TestPoint);
					OccupiedNeighbors += SightHit;
					SightHit |= EmptySeats.count(TestPoint);
					if(SightHit) break;
				}
			}

			if(OccupiedNeighbors >= EmptySeatTolerance)
			{
				NewEmptySeats.insert(OccupiedSeat); ++Changed;
			}
		}
		for(const auto Seat : NewOccupiedSeats){    EmptySeats.erase(Seat); OccupiedSeats.insert(Seat);}
		for(const auto Seat : NewEmptySeats   ){ OccupiedSeats.erase(Seat);    EmptySeats.insert(Seat);}
	} while (Changed);
	return OccupiedSeats.size();
}

int main()
{
	std::unordered_set<std::uint64_t> EmptySeats;
	std::unordered_set<std::uint64_t> OccupiedSeats;
	std::string CurLine;

	std::uint32_t Y = 0;
	while(std::getline(std::cin, CurLine))
	{
		for(std::uint32_t X = 0; X < CurLine.size(); ++X)
		{
			switch(CurLine[X])
			{
				case 'L': EmptySeats.insert(EncodePoint(X,Y)); break;
			}
		}
		++Y;
	}

	std::cout << Simulate(EmptySeats, OccupiedSeats, 1, 4) << std::endl;
	std::cout << Simulate(EmptySeats, OccupiedSeats, Y, 5) << std::endl;
}
