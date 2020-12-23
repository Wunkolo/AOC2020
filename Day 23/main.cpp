#include <bits/stdc++.h> 

// void Part1(std::array<std::uint8_t, 9> Cups, std::size_t Rounds)
// {
// 	for( std::size_t i = 0; i < Rounds; ++i )
// 	{
// 		constexpr auto CupAIdx = 1;
// 		constexpr auto CupBIdx = 2;
// 		constexpr auto CupCIdx = 3;
// 		const std::set<std::uint8_t> PickedCups({Cups[CupAIdx], Cups[CupBIdx], Cups[CupCIdx]});
// 		std::uint8_t CurTarget = Cups[0] - 1; if(CurTarget == 0) CurTarget = 9;
// 		while(
// 			   (CurTarget == Cups[CupAIdx])
// 			|| (CurTarget == Cups[CupBIdx])
// 			|| (CurTarget == Cups[CupCIdx])
// 		)
// 		{
// 			--CurTarget; if(CurTarget == 0) CurTarget = 9;
// 		}
// 		auto NewCups = Cups;
// 		NewCups[0] = Cups[0];
// 		NewCups[1] = Cups[4];
// 		NewCups[2] = Cups[5];
// 		NewCups[3] = Cups[6];
// 		NewCups[4] = Cups[7];
// 		NewCups[5] = Cups[8];
// 		auto CupDestination = std::distance(
// 			NewCups.cbegin(), std::find(NewCups.cbegin(), NewCups.cbegin() + 6, CurTarget)
// 		);
// 		assert(CupDestination < 6);
// 		std::memmove(NewCups.data() + CupDestination + 3, NewCups.data() + CupDestination, 5 - CupDestination);
// 		NewCups[CupDestination + 1] = Cups[CupAIdx];
// 		NewCups[CupDestination + 2] = Cups[CupBIdx];
// 		NewCups[CupDestination + 3] = Cups[CupCIdx];
// 		std::rotate(NewCups.begin(), NewCups.begin() + 1, NewCups.end());
// 		Cups = NewCups;
// 	}
// 	const auto DigitOne = std::distance(
// 		Cups.cbegin(), std::find(Cups.cbegin(), Cups.cend(), 1)
// 	);
// 	for(std::size_t i = 0; i < 8; ++i) std::cout << +Cups[(DigitOne + 1 + i) % 9];
// 	std::cout << std::endl;
// }

template<bool Part2, std::size_t Range>
void Simulate(std::array<std::uintmax_t, 9> OrigCups, std::size_t Rounds)
{
	// A "Flat" singlely linked list
	// Each element CupAdj[i] is "i"'s next number
	// "CupAdj[n] = m" means m comes after n
	static std::array<std::uintmax_t, Range + 1> CupAdj;
	{
		// Write the adjacency of the original cup labels
		for(std::size_t i = 0; i < OrigCups.size() - 1; ++i)
		{
			// Digit[i] goes to Digit[i + 1] after it
			CupAdj[OrigCups[i]] = OrigCups[i + 1];
		}
		// Write the rest of the range
		if(Part2)
		{
			// "The last digit goes to digit 10"
			CupAdj[OrigCups.back()] = 10;
			for(std::size_t i = 10; i < Range; ++i)
			{
				// Digit i goes to digit i+1
				CupAdj[i] = i + 1;
			}
			// Close the "circle"
			// "The millionth digit goes back to the front"
			CupAdj.back() = OrigCups.front();
		}
		else
		{
			// Close the "circle"
			// "The last digit goes back to digit 1"
			CupAdj[OrigCups.back()] = OrigCups.front();
		}
	}

	std::size_t CurCup = OrigCups[0];
	for( std::size_t i = 0; i < Rounds; ++i )
	{
		// Pick up the next three adjacent
		const std::size_t CupA = CupAdj[CurCup];
		const std::size_t CupB = CupAdj[CupA];
		const std::size_t CupC = CupAdj[CupB];
		std::size_t CupDestination = CurCup;

		while(
			CupDestination == CurCup
			|| (CupDestination == CupA)
			|| (CupDestination == CupB)
			|| (CupDestination == CupC)
		)
		{
			CupDestination -= 1; if(!CupDestination) CupDestination = Range;
		}

		const std::size_t NextCupDestination = CupAdj[CupDestination];
		CupAdj[CurCup] = CupAdj[CupC];
		CupAdj[CupDestination] = CupA;
		CupAdj[CupC] = NextCupDestination;

		CurCup = CupAdj[CurCup];
	}
	if(Part2)
	{
		std::cout << (CupAdj[1] * CupAdj[CupAdj[1]]) << std::endl;
	}
	else
	{
		auto CurrentDigit = 1;
		for(std::size_t i = 0; i < 8; ++i)
		{
			std::cout << CupAdj[CurrentDigit];
			CurrentDigit = CupAdj[CurrentDigit];
		}
		std::cout << std::endl;
	}
}

int main()
{
	std::array<std::uintmax_t, 9> Cups;
	std::copy_n(std::istream_iterator<char>(std::cin), 9, Cups.begin());
	for(auto& Label : Cups) Label -= '0';

	//Part1(Cups, 100);
	Simulate<false,        9>(Cups, 100);
	Simulate< true,1'000'000>(Cups, 10'000'000);
}