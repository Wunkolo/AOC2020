#include <iostream>
#include <cstdio>
#include <vector>
#include <string_view>
#include <unordered_map>
#include <bit>
#include <bitset>
#include <immintrin.h>

int main()
{
	std::unordered_map<std::uintmax_t, std::uintmax_t> Version1, Version2;
	std::uintmax_t Mask{}, MaskValue{};
	std::string CurLine;
	while(std::getline(std::cin, CurLine))
	{
		if(CurLine[1] == 'a') // mask
		{
			const auto MaskString = std::string_view(CurLine).substr(7);
			Mask = MaskValue = 0;
			for(std::intmax_t i = 0; i < 36; ++i)
			{
				const std::uint8_t CurBit = MaskString[i];
				if((CurBit == '0') || (CurBit == '1'))
				{
					Mask |= 1 << (35 - i);
					MaskValue |= (std::uintmax_t(CurBit - '0') << (35 - i));
				}
			}
		}
		else // mem
		{
			std::uintmax_t Address, Value;
			std::sscanf(CurLine.c_str(), "mem[%ju] = %ju", &Address, &Value);
			const auto WriteValue = (Value & ~Mask) | MaskValue;
			// Part 1
			Version1[Address] = WriteValue;
			// Part 2
			Address |= MaskValue; Address &= Mask;
			const std::uintmax_t Permutations = 1 << std::popcount(~Mask);
			for(std::size_t i = 0; i < Permutations; ++i)
			{
				const std::uintmax_t CurPermutation = _pdep_u64(i, ~Mask);
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