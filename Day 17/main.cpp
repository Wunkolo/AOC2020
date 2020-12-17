#include <iostream>
#include <cstdio>
#include <array>
#include <unordered_set>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

// import itertools
// for (x, y, z) in itertools.product(*[[-1, 0, 1] for _ in range(3)]):
// 	if x == y == z == 0: continue
// 	print(f'{x:+}, {y:+}, {z:+}')
constexpr static std::array<glm::ivec3, 26> Offsets3 = {{
	{ -1, -1, -1},
	{ -1, -1,  0},
	{ -1, -1, +1},
	{ -1,  0, -1},
	{ -1,  0,  0},
	{ -1,  0, +1},
	{ -1, +1, -1},
	{ -1, +1,  0},
	{ -1, +1, +1},
	{  0, -1, -1},
	{  0, -1,  0},
	{  0, -1, +1},
	{  0,  0, -1},
	{  0,  0, +1},
	{  0, +1, -1},
	{  0, +1,  0},
	{  0, +1, +1},
	{ +1, -1, -1},
	{ +1, -1,  0},
	{ +1, -1, +1},
	{ +1,  0, -1},
	{ +1,  0,  0},
	{ +1,  0, +1},
	{ +1, +1, -1},
	{ +1, +1,  0},
	{ +1, +1, +1},
}};

// import itertools
// for (x, y, z, w) in itertools.product(*[[-1, 0, 1] for _ in range(4)]):
// 	if x == y == z == w == 0: continue
// 	print(f'{x:+}, {y:+}, {z:+}, {w:+}')
constexpr static std::array<glm::ivec4, 80> Offsets4 = {{
	{-1, -1, -1, -1},
	{-1, -1, -1, +0},
	{-1, -1, -1, +1},
	{-1, -1, +0, -1},
	{-1, -1, +0, +0},
	{-1, -1, +0, +1},
	{-1, -1, +1, -1},
	{-1, -1, +1, +0},
	{-1, -1, +1, +1},
	{-1, +0, -1, -1},
	{-1, +0, -1, +0},
	{-1, +0, -1, +1},
	{-1, +0, +0, -1},
	{-1, +0, +0, +0},
	{-1, +0, +0, +1},
	{-1, +0, +1, -1},
	{-1, +0, +1, +0},
	{-1, +0, +1, +1},
	{-1, +1, -1, -1},
	{-1, +1, -1, +0},
	{-1, +1, -1, +1},
	{-1, +1, +0, -1},
	{-1, +1, +0, +0},
	{-1, +1, +0, +1},
	{-1, +1, +1, -1},
	{-1, +1, +1, +0},
	{-1, +1, +1, +1},
	{+0, -1, -1, -1},
	{+0, -1, -1, +0},
	{+0, -1, -1, +1},
	{+0, -1, +0, -1},
	{+0, -1, +0, +0},
	{+0, -1, +0, +1},
	{+0, -1, +1, -1},
	{+0, -1, +1, +0},
	{+0, -1, +1, +1},
	{+0, +0, -1, -1},
	{+0, +0, -1, +0},
	{+0, +0, -1, +1},
	{+0, +0, +0, -1},
	{+0, +0, +0, +1},
	{+0, +0, +1, -1},
	{+0, +0, +1, +0},
	{+0, +0, +1, +1},
	{+0, +1, -1, -1},
	{+0, +1, -1, +0},
	{+0, +1, -1, +1},
	{+0, +1, +0, -1},
	{+0, +1, +0, +0},
	{+0, +1, +0, +1},
	{+0, +1, +1, -1},
	{+0, +1, +1, +0},
	{+0, +1, +1, +1},
	{+1, -1, -1, -1},
	{+1, -1, -1, +0},
	{+1, -1, -1, +1},
	{+1, -1, +0, -1},
	{+1, -1, +0, +0},
	{+1, -1, +0, +1},
	{+1, -1, +1, -1},
	{+1, -1, +1, +0},
	{+1, -1, +1, +1},
	{+1, +0, -1, -1},
	{+1, +0, -1, +0},
	{+1, +0, -1, +1},
	{+1, +0, +0, -1},
	{+1, +0, +0, +0},
	{+1, +0, +0, +1},
	{+1, +0, +1, -1},
	{+1, +0, +1, +0},
	{+1, +0, +1, +1},
	{+1, +1, -1, -1},
	{+1, +1, -1, +0},
	{+1, +1, -1, +1},
	{+1, +1, +0, -1},
	{+1, +1, +0, +0},
	{+1, +1, +0, +1},
	{+1, +1, +1, -1},
	{+1, +1, +1, +0},
	{+1, +1, +1, +1},
}};

std::uintmax_t Part1(const std::unordered_set<glm::ivec4>& Field, std::size_t Cycles)
{
	std::unordered_set<glm::ivec4> CurField = Field;
	for(std::size_t CurCycle = 0; CurCycle < Cycles; ++CurCycle)
	{
		std::unordered_map<glm::ivec3, std::uintmax_t> SumField;
		for( const auto& CurActiveCube : CurField )
		{
			for(const auto& Offset : Offsets3)
			{
				++SumField[glm::ivec3(CurActiveCube) + Offset];
			}
		}
		std::unordered_set<glm::ivec4> NextField;
		for(const auto& Sum : SumField)
		{
			if(Sum.second == 2 && CurField.count(glm::vec4(Sum.first, 0)))
				NextField.insert(glm::ivec4(Sum.first, 0));
			if(Sum.second == 3)
				NextField.insert(glm::ivec4(Sum.first, 0));
		}
		CurField = NextField;
	}
	return CurField.size();
}

std::uintmax_t Part2(const std::unordered_set<glm::ivec4>& Field, std::size_t Cycles)
{
	std::unordered_set<glm::ivec4> CurField = Field;
	for(std::size_t CurCycle = 0; CurCycle < Cycles; ++CurCycle)
	{
		std::unordered_map<glm::ivec4, std::uintmax_t> SumField;
		for( const auto& CurActiveCube : CurField )
		{
			for(const auto& Offset : Offsets4) ++SumField[CurActiveCube + Offset];
		}
		std::unordered_set<glm::ivec4> NextField;
		for(const auto& Sum : SumField)
		{
			if(Sum.second == 2 && CurField.count(Sum.first))
				NextField.insert(Sum.first);
			if(Sum.second == 3)
				NextField.insert(Sum.first);
		}
		CurField = NextField;
	}
	return CurField.size();
}

int main()
{
	std::unordered_set<glm::ivec4> ActiveCubes;
	std::string CurLine;
	std::size_t CurRow;
	while(std::getline(std::cin, CurLine))
	{
		for(std::size_t CurCol = 0; CurCol < CurLine.size(); ++CurCol)
		{
			if(CurLine[CurCol] == '#') ActiveCubes.insert({CurCol, CurRow, 0, 0});
		}
		++CurRow;
	}

	std::cout << Part1(ActiveCubes, 6) << std::endl;
	std::cout << Part2(ActiveCubes, 6) << std::endl;
}