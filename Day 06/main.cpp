#include <iostream>

int main()
{
	std::uint32_t Part1{}, Part2{}, GroupMask{}, QuestionMask{~0u};
	std::string CurLine;
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	while( std::cin )
	{
		std::getline(std::cin, CurLine);
		if( !CurLine.length() && GroupMask || !std::cin )
		{
			Part1 += __builtin_popcount(GroupMask);
			Part2 += __builtin_popcount(GroupMask & QuestionMask);
			GroupMask = 0; QuestionMask = ~0u;
			continue;
		}
		std::uint32_t CurPerson{};
		for( const auto& Char : CurLine ) CurPerson |= 1 << (Char - 'a');
		GroupMask |= CurPerson; QuestionMask &= CurPerson;
	}
	std::cout << Part1 << '\n' << Part2 << std::endl;
}
