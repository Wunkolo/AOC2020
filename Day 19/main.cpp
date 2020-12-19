#include <iostream>
#include <cstdio>
#include <vector>
#include <cctype>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <unordered_map>

#define _GLIBCXX_REGEX_STATE_LIMIT 1000000

#include <regex>

template< bool Part2 = false >
std::string GenRegex(
	const std::unordered_map<std::uintmax_t, std::string>& Rules,
	std::uintmax_t Rule = 0
)
{
	if( Part2 )
	{
		switch(Rule)
		{
			// 8: 42 | 42 8
			// Match a continuous sequence of "42"s
			case 8:
			{
				return '(' + GenRegex<Part2>(Rules, 42) + "+)";
			}
			// 11: 42 31 | 42 11 31
			// match zero, or more 11s in between a 42 and a 31
			// An equal, even, amount of 42s and 31s next to each other
			case 11:
			{
				std::string CurLine;
				CurLine += '(';
				// Aw shit
				for(std::size_t i = 2; i < 10; ++i)
				{
					CurLine +=
						GenRegex<Part2>(Rules, 42) + '{' + std::to_string(i) + '}'
						+ GenRegex<Part2>(Rules, 31) + '{'+ std::to_string(i) + '}' + '|';
				}
				CurLine += GenRegex<Part2>(Rules, 42) + GenRegex<Part2>(Rules, 31) + ')';
				return CurLine;
			}
		}
	}
	std::stringstream ParseRule(Rules.at(Rule));
	std::string Result;
	std::string CurToken;
	while( ParseRule >> CurToken )
	{
		if(std::isdigit(CurToken[0]))
		{
			Result += GenRegex<Part2>(Rules, std::stoull(CurToken));
		}
		else Result += CurToken;
	}
	return '(' + Result + ')';
}

int main()
{
	std::unordered_map<std::uintmax_t, std::string> Rules;
	std::string CurLine;
	// Parse Rules
	while( std::getline(std::cin, CurLine) && CurLine.size() )
	{
		const auto ColonPos = CurLine.find_first_of(':');
		const std::uint8_t RuleNumber = std::stoull(CurLine.substr(0, ColonPos));
		auto Rule = CurLine.substr(ColonPos + 2);
		// Remove "
		Rule.erase(std::remove(Rule.begin(), Rule.end(), '\"'), Rule.end());
		Rules[RuleNumber] = Rule;
	}


	// Parse strings
	std::vector<std::string> Strings;
	while( std::getline(std::cin, CurLine) && CurLine.size() )
	{
		Strings.push_back(CurLine);
	}

	const std::regex Part1Regex(GenRegex<false>(Rules, 0));
	const std::regex Part2Regex(GenRegex<true>(Rules, 0));

	std::uintmax_t Part1{}, Part2{};
	for(const auto& String : Strings)
	{
		Part1 += std::regex_match(String, Part1Regex);
		Part2 += std::regex_match(String, Part2Regex);
	}
	std::cout << Part1 << std::endl;
	std::cout << Part2 << std::endl;
}