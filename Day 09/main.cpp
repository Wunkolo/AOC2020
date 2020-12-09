#include <iostream>
#include <cstdio>
#include <vector>
#include <numeric>
#include <algorithm>

constexpr std::size_t Scope = 25;

std::uintmax_t FindInvalid(const std::vector<std::uintmax_t>& Numbers, std::size_t Scope)
{
	for( std::size_t i = Scope; i < Numbers.size(); ++i )
	{
		bool Valid = false;
		for(std::size_t j = i - Scope; j < i && !Valid; ++j)
		{
			for(std::size_t k = i - Scope; k < i && !Valid; ++k)
			{
				if(j == k) continue;
				if(Numbers[j] + Numbers[k] == Numbers[i]) Valid = true;
			}
		}
		if(!Valid) return Numbers[i];
	}
	return 0;
}

std::uintmax_t FindWeakness(const std::vector<std::uintmax_t>& Numbers, std::uintmax_t Target)
{
	std::vector<std::uintmax_t> SumTable(Numbers.size());
	std::partial_sum(Numbers.cbegin(), Numbers.cend(), SumTable.begin());
	for( std::size_t i = 1; i < SumTable.size(); ++i)
	{
		for( std::size_t j = 0; j < i; ++j )
		{
			if( SumTable[i] - SumTable[j] == Target )
			{
				const auto MinMax = std::minmax_element(
					Numbers.cbegin() + j, Numbers.cbegin() + i
				);
				return *MinMax.second + *MinMax.first;
			}
		}
	}
	return 0;
}

int main()
{
	std::uintmax_t CurNumber;
	std::vector<std::uintmax_t> Numbers;
	while(std::cin >> CurNumber) Numbers.push_back(CurNumber);
	const std::uintmax_t Invalid = FindInvalid(Numbers, Scope);
	std::cout << Invalid << std::endl;
	std::cout << FindWeakness(Numbers, Invalid) << std::endl;
}
