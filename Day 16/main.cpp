#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
#include <set>
#include <bit>

struct Rule
{
	std::uintmax_t MinA, MinB, MaxA, MaxB;
	bool Test(std::uintmax_t Number) const
	{
		return
			(MinA <= Number && Number <= MaxA) ||
			(MinB <= Number && Number <= MaxB);
	}
};

int main()
{
	// Rule Index -> Rule
	std::map<std::uintmax_t, Rule> Rules;
	// RuleMinA -> Rule*, RuleMinB -> Rule*
	// Used for quickly looking up a number and the rule it lands on via lower_bound
	std::map<std::uintmax_t, Rule*> RuleSpans;

	// Interpret rules
	char Name[256];
	std::uintmax_t MinA, MinB, MaxA, MaxB;
	std::string CurLine;
	std::uintmax_t RuleIdx = 0;
	while( std::getline(std::cin, CurLine) && CurLine.size() )
	{
		if( std::sscanf( CurLine.c_str(), " %[^:]: %ju-%ju or %ju-%ju", Name, &MinA, &MaxA, &MinB, &MaxB) == 5 )
		{
			Rule CurRule;
			CurRule.MinA = MinA; CurRule.MaxA = MaxA;
			CurRule.MinB = MinB; CurRule.MaxB = MaxB;
			Rules[RuleIdx] = CurRule;
			RuleSpans[MinA] = &Rules[RuleIdx]; RuleSpans[MinB] = &Rules[RuleIdx];
			++RuleIdx;
		}
	}
	assert(Rules.size() == 20);

	// Your Ticket
	std::vector<std::uintmax_t> YourTicket;
	std::getline(std::cin, CurLine); // "your ticket:"
	if(std::getline(std::cin, CurLine))
	{
		std::stringstream TicketStream(CurLine); std::string CurNum;
		while(std::getline(TicketStream, CurNum, ',')) YourTicket.push_back(std::stoull(CurNum));
		assert(YourTicket.size() == 20);
	}

	// Nearby tickets
	std::vector<std::vector<std::uintmax_t>> NearbyTickets;
	std::getline(std::cin, CurLine); // ""
	std::getline(std::cin, CurLine); // "nearby tickets:"
	while(std::getline(std::cin, CurLine) && CurLine.size())
	{
		NearbyTickets.push_back({});
		auto& CurTicket = NearbyTickets.back();
		std::stringstream TicketStream(CurLine); std::string CurNum;
		while(std::getline(TicketStream, CurNum, ',')) CurTicket.push_back(std::stoull(CurNum));
		assert(CurTicket.size() == 20);
	}

	//// Part 1
	std::uintmax_t Part1{};
	std::vector<std::vector<std::uintmax_t>> ValidTickets;
	for(const auto& CurTicket : NearbyTickets)
	{
		bool ValidTicket = true;
		for(auto& Num : CurTicket)
		{
			auto Match = RuleSpans.lower_bound(Num);
			if(Match == RuleSpans.begin() || !(--Match)->second->Test(Num))
			{
				// Not matched
				Part1 += Num;
				ValidTicket = false;
			}
		}
		if( ValidTicket )
		{
			ValidTickets.push_back(CurTicket);
		}
	}
	std::cout << Part1 << std::endl;

	//// Part 2
	// Create a transposed matrix of the valid tickets
	// Each column-vector in columns is all of the other ticket's first number
	std::vector<std::vector<std::uintmax_t>> Columns(20);

	for( std::size_t Row = 0; Row < ValidTickets.size(); ++Row )
	{
		for( std::size_t Column = 0; Column < ValidTickets[Row].size(); ++Column )
		{
			Columns[Column].push_back(ValidTickets[Row][Column]);
		}
	}
	// Test each input column against each rule. If a whole column passes a rule
	// then add it to the list of possibilities
	std::vector<std::uint32_t> Possibilities(Columns.size());
	for( std::size_t i = 0; i < Columns.size(); ++i )
	{
		for(const auto Span : Rules)
		{
			bool ValidCol = true;
			for(const auto Val : Columns[i]) ValidCol &= Span.second.Test(Val);
			if(ValidCol) Possibilities[i] |= (1u << Span.first);
		}
	}

	std::vector<std::uintmax_t> OrderIndex(Possibilities.size());
	// Gaussian elimination
	while(Possibilities.size())
	{
		// Find the column with the least amount of possibilities(just 1 element)
		const auto MinRule = std::find_if(
			Possibilities.cbegin(), Possibilities.cend(),
			std::has_single_bit<std::uint32_t>
		);
		if(MinRule == Possibilities.cend()) break;
		const auto MinRuleIndex = std::distance(Possibilities.cbegin(), MinRule);
		const std::uint32_t CurBasis = *MinRule;
		// We get this [ ... 0 0 0 1 0 0 0 ... ] vector and remove it from
		// from the possibilities
		// Remove this column from further testing, we know what it is
		//Possibilities.erase(Possibilities.begin() + MinRuleIndex);
		// Add it to the order, now that we know exactly where this one should be
		OrderIndex[std::countr_zero(CurBasis)] = MinRuleIndex;
		// Remove this possibility from all other possibility-sets
		for( auto& Possibility : Possibilities ) Possibility &= ~CurBasis;
	}
	std::uintmax_t Part2 = 1;
	for( std::size_t i = 0; i < 6; ++i )
	{
		Part2 *= YourTicket[OrderIndex[i]];
	}
	std::cout << Part2 << std::endl;
}