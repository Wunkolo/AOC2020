#include <iostream>
#include <cstdio>
#include <cmath>
#include <vector>

struct Bus { std::uintmax_t Index, ID; };

std::uintmax_t Part1(
	const std::vector<Bus>& Busses, std::uintmax_t TimeStamp
)
{
	std::uintmax_t MinBus{}, MinWait{~0ull};
	for(const auto Bus : Busses)
	{
		const auto Soonest = (1 + (TimeStamp / Bus.ID)) * Bus.ID;
		const auto CurWait  = Soonest - TimeStamp;
		if( CurWait < MinWait )
		{
			MinBus = Bus.ID; MinWait = CurWait;
		}
	}
	return MinBus * MinWait;
}

// https://stackoverflow.com/a/8498251/531719
template <typename T>
T ModPow(T Base, T Exp, T Modulo)
{
	Base %= Modulo;
	T Result = T(1);
	while (Exp > 0)
	{
		if (Exp & 1) Result = (Result * Base) % Modulo;
		Base = (Base * Base) % Modulo;
		Exp >>= 1;
	}
	return Result;
}

std::uintmax_t Part2( const std::vector<Bus>& Busses )
{
	std::uintmax_t Product = 1;
	for(const auto& Bus : Busses) Product *= Bus.ID;
	std::uintmax_t Sum{};
	for(const auto& Bus : Busses)
	{
		const std::uintmax_t p = Product / Bus.ID;
		Sum += (Bus.ID - Bus.Index) * p * ModPow(p, Bus.ID - 2, Bus.ID);
	}
	return Sum % Product;
}

int main()
{
	std::uintmax_t TimeStamp{};
	if(!(std::cin >> TimeStamp)) return 0;
	std::vector<Bus> Busses;
	std::string CurLine;
	std::uintmax_t i = 0;
	while(std::getline(std::cin, CurLine, ','))
	{
		if(CurLine[0] != 'x') Busses.push_back({i, std::stoull(CurLine)});
		++i;
	}

	std::cout << Part1(Busses, TimeStamp) << std::endl;
	std::cout << Part2(Busses) << std::endl;
}