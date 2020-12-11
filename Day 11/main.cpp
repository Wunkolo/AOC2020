#include <iostream>
#include <cstdio>
#include <array>
#include <algorithm>
#include <tuple>
#include <unordered_set>

std::uint32_t EncodePoint(std::uint16_t X, std::uint16_t Y)
{
	return (std::uint32_t(Y) << 16) | X;
}

std::tuple<std::uint16_t, std::uint16_t> DecodePoint(std::uint32_t Point)
{
	return {std::uint16_t(Point), std::uint16_t(Point >> 16)};
}
constexpr std::array<std::tuple<std::int8_t, std::int8_t>, 8> Offsets = {{
	{ 0,  1},{ 0, -1},{ 1,  0},{-1,  0},{ 1,  1},{-1, -1},{ 1, -1},{-1,  1}
}};

std::uintmax_t Simulate(
	std::unordered_set<std::uint32_t> EmptySeats,
	std::unordered_set<std::uint32_t> OccupiedSeats,
	std::uintmax_t SightRadius,
	std::uintmax_t EmptySeatTolerance
)
{
	
	std::uint16_t Changed{};
	std::unordered_set<std::uint32_t> NewEmptySeats, NewOccupiedSeats;
	do
	{
		Changed = 0;
		NewEmptySeats.clear(); NewOccupiedSeats.clear();
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
				if(OccupiedNeighbors) break; // Optimization
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
				if(OccupiedNeighbors >= EmptySeatTolerance)
				{
					NewEmptySeats.insert(OccupiedSeat); ++Changed;
					break;
				}
			}
		}
		for(const auto Seat : NewOccupiedSeats){    EmptySeats.erase(Seat); OccupiedSeats.insert(Seat);}
		for(const auto Seat : NewEmptySeats   ){ OccupiedSeats.erase(Seat);    EmptySeats.insert(Seat);}
	} while (Changed);
	return OccupiedSeats.size();
}

int main()
{
	std::unordered_set<std::uint32_t> EmptySeats;
	std::unordered_set<std::uint32_t> OccupiedSeats;
	std::string CurLine;

	std::uint16_t Y{}, MaxDim{};
	while(std::getline(std::cin, CurLine))
	{
		for(std::uint16_t X = 0; X < CurLine.size(); ++X)
		{
			if(CurLine[X] == 'L') EmptySeats.insert(EncodePoint(X,Y));
			MaxDim = std::max(MaxDim, X);
		}
		++Y;
		MaxDim = std::max(MaxDim, Y);
	}

	std::cout << Simulate(EmptySeats, OccupiedSeats,      1, 4) << std::endl;
	std::cout << Simulate(EmptySeats, OccupiedSeats, MaxDim, 5) << std::endl;
}
