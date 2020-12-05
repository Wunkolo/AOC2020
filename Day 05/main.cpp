#include <iostream>

// XOR of all number s from [1,N]
// https://oeis.org/A003815
inline std::uint16_t SigmaXor(std::uint16_t N)
{
	return (N & (N << 1 & 2) - 1) ^ (N >> 1 & 1);
}

int main()
{
	std::uint16_t Min{127*8+7}, Max{}, Missing{}; std::string CurLine;
	while( std::getline(std::cin, CurLine) )
	{
		std::uint16_t SeatID{};
		for( std::size_t i = 0; i < 10; ++i )
		{
			SeatID <<=1;
			switch(CurLine[i])
			{
				case 'B': case 'R':
				{
					SeatID |= 1;
					break;
				}
			}
		}
		Missing ^= SeatID;
		Max = std::max(Max, SeatID); Min = std::min(Min, SeatID);
	}
	// Missing is the XOR of all processed seats, except our own
	// So we XOR all the seats between Min and Max, leaving only one seat left
	Missing ^= SigmaXor(Min-1) ^ SigmaXor(Max);
	std::cout << Max << '\n' << Missing << std::endl;
}
