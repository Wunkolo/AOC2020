#include <iostream>
#include <bit>

int main()
{
	std::uint32_t Part1{}, Part2{}, GroupMask{}, QuestionMask{~0u};
	std::string CurLine;
	while( std::cin )
	{
		if( CurLine.clear(), std::getline(std::cin, CurLine); CurLine.empty())
		{
			Part1 += std::popcount(GroupMask);
			Part2 += std::popcount(GroupMask & QuestionMask);
			GroupMask = 0; QuestionMask = ~0u;
		}
		else
		{
			std::uint32_t CurPerson{};
			for( char Char : CurLine ) CurPerson |= 1 << (Char - 'a');
			GroupMask |= CurPerson; QuestionMask &= CurPerson;
		}
	}
	std::cout << Part1 << '\n' << Part2 << std::endl;
}
