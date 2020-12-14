#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string_view>
#include <unordered_map>
#include <bit>
#include <bitset>
#include <immintrin.h>

int main()
{
	std::unordered_map<std::uintmax_t, std::uintmax_t> Version1, Version2;
	std::uintmax_t CurMask{}, CurMaskValue{};
	std::string CurLine;
	while( std::getline(std::cin, CurLine) )
	{
		if( CurLine[1] == 'a' ) // mask
		{
			const auto MaskInput = std::string_view(CurLine).substr(7);
			std::string MaskString(36, '0'), MaskValueString(36, '0');
			std::transform(MaskInput.cbegin(), MaskInput.cend(), MaskString.begin(),
				[](auto Char){return Char == 'X' ? '1':'0';}
			);
			std::transform(MaskInput.cbegin(), MaskInput.cend(), MaskValueString.begin(),
				[](auto Char){return Char == 'X' ? '0':Char;}
			);
			CurMask			= std::bitset<36>(MaskString).to_ullong();
			CurMaskValue	= std::bitset<36>(MaskValueString).to_ullong();
		}
		else // mem
		{
			std::uintmax_t Address, Value;
			std::sscanf(CurLine.c_str(), "mem[%ju] = %ju", &Address, &Value);
			// Part 1
			Version1[Address] = (Value & CurMask) | CurMaskValue;
			// Part 2
			Address |= CurMaskValue; Address &= ~CurMask;
			const std::uintmax_t Permutations = 1 << std::popcount(CurMask);
			for( std::size_t i = 0; i < Permutations; ++i )
			{
				const std::uintmax_t CurPermutation = _pdep_u64(i, CurMask);
				Version2[Address | CurPermutation] = Value;
			}
		}
	}
	std::uintmax_t Sum1{}, Sum2{};
	for(const auto Mem : Version1) Sum1 += Mem.second;
	for(const auto Mem : Version2) Sum2 += Mem.second;
	std::cout << Sum1 << std::endl;
	std::cout << Sum2 << std::endl;
}