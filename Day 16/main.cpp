#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
#include <set>

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
	// FieldStart->FieldEnd | FieldStart->Ticket
	std::map<std::uintmax_t, Rule> Rules;
	std::map<std::uintmax_t, Rule*> RuleSpans;
	std::set<std::uintmax_t> ValidFields, DepartureField;
	char Name[256];
	std::uintmax_t MinA, MinB, MaxA, MaxB;

	std::string CurLine;
	std::uintmax_t RuleIdx = 0;
	while( std::getline(std::cin, CurLine) && CurLine.size() )
	{
		if(
			std::sscanf( CurLine.c_str(), " %[^:]: %ju-%ju or %ju-%ju", Name, &MinA, &MaxA, &MinB, &MaxB) == 5
		)
		{
			Rule CurRule;
			CurRule.MinA = MinA; CurRule.MaxA = MaxA;
			CurRule.MinB = MinB; CurRule.MaxB = MaxB;
			Rules[RuleIdx] = CurRule;
			RuleSpans[MinA] = &Rules[RuleIdx]; RuleSpans[MinB] = &Rules[RuleIdx];
			++RuleIdx;
		}
	}

	// Your Ticket
	std::vector<std::uintmax_t> YourTicket;
	std::getline(std::cin, CurLine); // "your ticket:"
	if(std::getline(std::cin, CurLine))
	{
		std::stringstream TicketStream(CurLine); std::string CurNum;
		while(std::getline(TicketStream, CurNum, ',')) YourTicket.push_back(std::stoull(CurNum));
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
	}

	std::uintmax_t Part1{};
	std::vector<std::vector<std::uintmax_t>> ValidTickets;
	for(const auto& CurTicket : NearbyTickets)
	{
		bool ValidTicket = true;
		for(auto& Num : CurTicket)
		{
			const auto Match = --RuleSpans.lower_bound(Num);
			if(!Match->second->Test(Num))
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
	std::vector<std::vector<std::uintmax_t>> Columns(ValidTickets[0].size());

	for( std::size_t Row = 0; Row < ValidTickets.size(); ++Row )
	{
		for( std::size_t Column = 0; Column < ValidTickets[Row].size(); ++Column )
		{
			Columns[Column].push_back(ValidTickets[Row][Column]);
		}
	}
	std::map<std::uintmax_t, std::set<std::uintmax_t>> Possibilities;

	for( std::size_t i = 0; i < Columns.size(); ++i )
	{
		for(const auto Span : Rules)
		{
			bool ValidCol = true;
			for(const auto Val : Columns[i]) ValidCol &= Span.second.Test(Val);
			if(ValidCol) Possibilities[i].insert(Span.first);
		}
	}

	std::vector<std::uintmax_t> OrderIndex(Possibilities.size());\
	// Gaussian elimination
	while(Possibilities.size())
	{
		// Find the column with the least amount of possibilities
		std::uintmax_t RuleIndex = ~0ull;
		std::uintmax_t MinColumnSize = ~0ull;
		for( auto Possibility : Possibilities )
		{
			if(Possibility.second.size() < MinColumnSize)
			{
				RuleIndex = Possibility.first;
				MinColumnSize = Possibility.second.size();
			}
		}
		// This _should_ be a set of just 1 element each time, otherwise this
		// is not a solvable system
		assert(Possibilities[RuleIndex].size() == 1);
		// We get this [ ... 0 0 0 1 0 0 0 ... ] vector and remove it from
		// from the possibilities
		const std::uintmax_t CurBasis = *Possibilities[RuleIndex].begin();
		Possibilities.erase(RuleIndex);
		// Add it to the order, now that we know exactly where this one should be
		OrderIndex[CurBasis] = RuleIndex;
		// Remove this possibility from all other possibility-sets
		for( auto& Possibility : Possibilities ) Possibility.second.erase(CurBasis);
	}
	std::uintmax_t Part2 = 1;
	for( std::size_t i = 0; i < 6; ++i )
	{
		Part2 *= YourTicket[OrderIndex[i]];
	}
	std::cout << Part1 << std::endl;
	std::cout << Part2 << std::endl;
}