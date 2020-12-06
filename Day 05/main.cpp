#include <iostream>

inline std::uintmax_t XorTo(std::uintmax_t N)
{
	return (N & (N << 1 & 0b11) - 1) ^ (N >> 1 & 0b1);
}

int main()
{
	std::uintmax_t Min{~0ull}, Max{}, Missing{}; std::string CurLine;
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	while(std::getline(std::cin, CurLine))
	{
		std::uintmax_t SeatID{};
		for( auto Digit : CurLine ) (SeatID <<= 1) |= (Digit=='B' || Digit=='R');
		Missing ^= SeatID;
		Max = std::max(Max, SeatID); Min = std::min(Min, SeatID);
	}
	Missing ^= XorTo(Min-1) ^ XorTo(Max);
	std::cout << Max << '\n' << Missing << std::endl;
}
