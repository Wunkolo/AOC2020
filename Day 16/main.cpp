#include <iostream>
#include <cstdio>
#include <sstream>
#include <algorithm>
#include <vector>
#include <array>
#include <map>
#include <set>

// struct Ring2{}

int main()
{
	// FieldStart->FieldEnd | FieldStart->Ticket
	std::map<std::uintmax_t, std::uintmax_t> FieldSpan, FieldName;
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
			FieldSpan[MinA] = MaxA; FieldSpan[MinB] = MaxB; ValidFields.insert({MinA, MinB});
			FieldName[MinA] = MinA; FieldName[MinB] = MinA;
			FieldIdxMap[FieldIdx] = MinA;
			if(CurLine.rfind("departure", 0) == 0)
			{
				DepartureField.insert({MinA, MinB});
			}
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
		std::set<std::uintmax_t> CurFields;
		for(auto& Num : CurTicket)
		{
			const auto Match = --FieldSpan.lower_bound(Num);
			//std::cout << Num << '|' << Match->first << '-' << Match->second << std::endl;
			if(Match->first <= Num && Num <= Match->second)
			{
				// Match
				CurFields.insert(Match->first);
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
			std::set_intersection(
				CurFields.cbegin(), CurFields.cend(),
				ValidFields.cbegin(), ValidFields.cend(),
				std::inserter(ValidFields, ValidFields.begin())
			);
		}
	}

	std::vector<std::uintmax_t> ValidFieldOrder;
	// For each column N
	for(std::size_t i = 0; i < 20; ++i)
	{
		// Get all the rules that match each number in the column
		std::vector<std::uintmax_t> CurColumn;
		for(const auto& CurTicket : ValidTickets)
		{
			const auto CurNum = CurTicket[i];
			const auto CurMatch = --ValidFields.lower_bound(CurNum);
			CurColumn.push_back(FieldName[*CurMatch]);
		}
		for(auto Field : CurColumn)
		{
			std::cout << Field << ',';
		}
		std::cout << std::endl;
		// All the rules are the same for this column
		if(std::adjacent_find( CurColumn.cbegin(), CurColumn.cend(), std::not_equal_to<>() ) == CurColumn.end() )
		{
			ValidFieldOrder.push_back(CurColumn[0]);
			ValidFields.erase(CurColumn[0]);
		}
	}
	for(auto Field : ValidFieldOrder)
	{
		std::cout << Field << ',';
	}
	std::cout << std::endl;

	std::uintmax_t Part2 = 1;
	std::cout << Part1 << std::endl;
	std::cout << Part2 << std::endl;
}