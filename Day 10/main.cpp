#include <iostream>
#include <cstdio>
#include <vector>
#include <numeric>
#include <algorithm>
#include <unordered_map>

std::uintmax_t Part1(const std::vector<std::uintmax_t>& Jolts)
{
	std::uintmax_t Ones{}, Threes{};
	for( std::size_t i = 1; i < Jolts.size(); ++i )
	{
		switch( Jolts[i] - Jolts[i - 1] )
		{
			case 1:{ ++Ones; break;}
			case 3:{ ++Threes; break;}
		}
	}
	return Ones * Threes;
}

std::uintmax_t Part2(const std::vector<std::uintmax_t>& Jolts)
{
	std::unordered_map<std::uintmax_t, std::uintmax_t> Counts{{0,1}};
	for(auto Jolt : Jolts)
	{
		Counts[Jolt] += Counts[Jolt - 1] + Counts[Jolt - 2] + Counts[Jolt - 3];
	}
	return Counts[Jolts.back()];
}

int main()
{
	std::uintmax_t CurJolt;
	std::vector<std::uintmax_t> Jolts{0};
	while(std::cin >> CurJolt) Jolts.push_back(CurJolt);
	std::sort(Jolts.begin(), Jolts.end());
	Jolts.push_back(3 + Jolts.back());
	std::vector<std::uintmax_t> Differences{Jolts.size()};

	std::cout << Part1(Jolts) << std::endl;
	std::cout << Part2(Jolts) << std::endl;
}
