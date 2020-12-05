#include <iostream>
#include <set>

int main()
{
	std::set<std::size_t> Seats;
	std::size_t Min{127*8+7}, Max{}, Part2{};
	std::string CurLine; std::size_t i;
	while( std::getline(std::cin, CurLine) )
	{
		std::uintmax_t Row = 0;
		std::uintmax_t Column = 0;
		for( i = 0; i < 7; ++i )
		{
			Row <<= 1; Row |= CurLine[i] == 'B';
		}
		for( i = 7; i < 10; ++i )
		{
			Column <<= 1; Column |= CurLine[i] == 'R';
		}
		const std::size_t CurrentID = Row * 8 + Column;
		Seats.insert(CurrentID);
		Max = std::max(Max, CurrentID); Min = std::min(Min, CurrentID);
	}
	for( i = Min; i <= Max; ++i )
	{
		if( Seats.count(i - 1) && Seats.count(i + 1) && !Seats.count(i) )
		{
			Part2 = i; break;
		}
	}
	std::cout << Max << std::endl; std::cout << Part2 << std::endl;
}
