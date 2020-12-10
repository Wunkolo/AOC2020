#include <iostream>
#include <cstdio>
#include <vector>
#include <numeric>
#include <algorithm>
#include <unordered_map>

std::uintmax_t Part1(const std::vector<std::uintmax_t>& Jolts)
{
	std::vector<std::uintmax_t> Differences(Jolts.size());
	std::adjacent_difference(Jolts.cbegin(), Jolts.cend(), Differences.begin());
	return std::count(Differences.cbegin(), Differences.cend(), 1)
		* std::count(Differences.cbegin(), Differences.cend(), 3);
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
	std::vector<std::uintmax_t> Jolts;
	while(std::cin >> CurJolt) Jolts.push_back(CurJolt);
	Jolts.push_back(0);
	Jolts.push_back(3 + *std::max_element(Jolts.cbegin(), Jolts.cend()));
	std::sort(Jolts.begin(), Jolts.end());
	std::cout << Part1(Jolts) << std::endl;
	std::cout << Part2(Jolts) << std::endl;
}
