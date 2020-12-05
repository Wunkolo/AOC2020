#include <iostream>

// XOR of all numbers from [1,N]
// https://oeis.org/A003815
inline std::uintmax_t SigmaXor(std::uintmax_t N)
{
	return (N & (N << 1 & 0b11) - 1) ^ (N >> 1 & 0b1);
}

int main()
{
	std::uintmax_t Min{~0ull}, Max{}, Missing{}; std::string CurLine;
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	while( std::getline(std::cin, CurLine) )
	{
		std::uintmax_t SeatID{};
		for( std::size_t i = 0; i < CurLine.length(); ++i )
		{
			SeatID <<= 1;
			if(CurLine[i]=='B' || CurLine[i]=='R') SeatID |= 1;
		}
		Missing ^= SeatID;
		Max = std::max(Max, SeatID); Min = std::min(Min, SeatID);
	}
	// Missing is the XOR of all processed seats, except our own
	// So we XOR all the seats between Min and Max, leaving only one seat left
	Missing ^= SigmaXor(Min-1) ^ SigmaXor(Max);
	std::cout << Max << '\n' << Missing << std::endl;
}
