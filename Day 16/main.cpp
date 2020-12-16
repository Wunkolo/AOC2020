#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <vector>
#include <array>
#include <map>
#include <set>

struct Rule
{
	std::uintmax_t MinA, MinB, MaxA, MaxB;
	bool Test(std::uintmax_t Number)
	{
		return
			(MinA <= Number && Number <= MaxA) ||
			(MinB <= Number && Number <= MaxB);
	}
};

int main()
{
	// FieldStart->FieldEnd | FieldStart->Ticket
	std::map<std::string, Rule> Rules;
	std::map<std::uintmax_t, Rule*> RuleSpans;
	std::set<std::uintmax_t> ValidFields, DepartureField;
	char Name[256];
	std::uintmax_t MinA, MinB, MaxA, MaxB;
	std::string CurLine;
	while( std::getline(std::cin, CurLine) && CurLine.size() )
	{
		if(
			std::sscanf( CurLine.c_str(), " %[^:]: %ju-%ju or %ju-%ju", Name, &MinA, &MaxA, &MinB, &MaxB) == 5
		)
		{
			Rule CurRule;
			CurRule.MinA = MinA; CurRule.MaxA = MaxA;
			CurRule.MinB = MinB; CurRule.MaxB = MaxB;
			Rules[Name] = CurRule;
			RuleSpans[MinA] = &Rules[Name]; RuleSpans[MinB] = &Rules[Name];
		}
		//std::cout << Name << ' ' << MinA << '_' << MaxA << ' ' << MinB << '_' << MaxB << std::endl;
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
			//std::cout << Num << '|' << Match->first << '-' << Match->second << std::endl;
			if(Match->second->Test(Num))
			{
				// Match
			}
			else
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
	// std::cout << ValidTickets.size() << std::endl;
	// std::vector<std::vector<std::uintmax_t>> Columns(ValidTickets.size());

	// for( std::size_t Row = 0; Row < ValidTickets.size(); ++Row )
	// {
	// 	for( std::size_t Column = 0; Column < ValidTickets[Row].size(); ++Column )
	// 	{
	// 		Columns[Column].push_back(ValidTickets[Row][Column]);
	// 	}
	// }
	// std::map<std::uintmax_t, std::set<std::uintmax_t>> Possible;

	// for( std::size_t i = 0; i < Columns.size(); ++i )
	// {
	// 	std::cout << i << '|';
	// 	for(const auto Span : Rules)
	// 	{
	// 		bool ValidCol = true;
	// 		for(const auto Val : Columns[i])
	// 		{
	// 			ValidCol &= (Span.first <= Val && Val <= Span.second);
	// 		}
	// 		if(ValidCol)
	// 		{
	// 			Possible[i].insert(Span.first);
	// 			std::cout << Span.first << '-' << Span.second << ',';
	// 		}
	// 	}
	// 	std::cout << std::endl;
	// }

	// std::vector<std::uintmax_t> Definite(Possible.size());
	// while(Possible.size())
	// {
	// 	std::uintmax_t FoundCol = 0;

	// }

	std::uintmax_t Part2 = 1;
	std::cout << Part1 << std::endl;
	std::cout << Part2 << std::endl;
}