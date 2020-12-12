#include <iostream>
#include <cstdio>
#include <cmath>
#include <tuple>
#include <vector>
#include <array>

static constexpr std::array<std::tuple<std::int8_t, std::int8_t>, 4> Offsets = {{
	{ 1,  0}, { 0,  1}, {-1,  0}, { 0, -1},
}};

std::uintmax_t SimulateShip(
	const std::vector<std::tuple<char, std::uintmax_t>>& Instructions,
	std::int32_t PosX = 0, std::int32_t PosY = 0, std::int32_t Rot = 0
)
{
	for(const auto& Instruction : Instructions)
	{
		char Action = std::get<0>(Instruction);
		auto Value = std::get<1>(Instruction);
		switch(Action)
		{
			case 'N': {                PosY += Value; break;}
			case 'S': {                PosY -= Value; break;}
			case 'E': {                PosX += Value; break;}
			case 'W': {                PosX -= Value; break;}
			case 'L': {Rot +=     (Value/90); Rot%=4; break;}
			case 'R': {Rot += 4 - (Value/90); Rot%=4; break;}
			case 'F': {
				PosX += Value * std::get<0>(Offsets[Rot]);
				PosY += Value * std::get<1>(Offsets[Rot]);
				break;
			}
		}
	}
	return std::abs(PosX) + std::abs(PosY);
}

std::uintmax_t SimulateWaypoint(
	const std::vector<std::tuple<char, std::uintmax_t>>& Instructions,
	std::int32_t WayX = 0, std::int32_t WayY = 0, std::int32_t Rot = 0
)
{
	std::int32_t PosX{}, PosY{};
	for(const auto& Instruction : Instructions)
	{
		const char Action = std::get<0>(Instruction);
		auto Value = std::get<1>(Instruction);
		switch(Action)
		{
			case 'N': {                             WayY += Value; break;}
			case 'S': {                             WayY -= Value; break;}
			case 'E': {                             WayX += Value; break;}
			case 'W': {                             WayX -= Value; break;}
			case 'F': {PosX += WayX * Value; PosY += WayY * Value; break;}
			case 'L': {Value = (-Value + 360);                           }
			case 'R': {
				const auto Bin = (Value%360)/90; const auto Gray = Bin ^ (Bin >> 1);
				if( Bin&0b01)std::swap(WayX, WayY);
				if(Gray&0b10)WayX = -WayX; if(Gray&0b01)WayY = -WayY;
				break;
			}
		}
	}
	return std::abs(PosX) + std::abs(PosY);
}

int main()
{
	char Action; std::uintmax_t Value;
	std::vector<std::tuple<char, std::uintmax_t>> Instructions;
	while( std::scanf(" %c%ju", &Action, &Value) == 2 )
	{
		Instructions.emplace_back(Action, Value);
	}
	std::cout << SimulateShip(    Instructions,  0, 0) << std::endl;
	std::cout << SimulateWaypoint(Instructions, 10, 1) << std::endl;
}
