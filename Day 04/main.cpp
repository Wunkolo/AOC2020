#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <charconv>

template<std::uintmax_t Min, std::uintmax_t Max>
bool Between(std::uintmax_t Number){return (Min <= Number) && (Number <= Max);}

template<std::uintmax_t Min, std::uintmax_t Max, std::uintmax_t Digits, std::uintmax_t Base = 10>
bool ValidNumber(const std::string& String)
{
	if(String.length() != Digits) return false;
	std::uintmax_t Number;
	const std::from_chars_result ParseResult = std::from_chars(
		String.data(), String.data() + Digits, Number, Base
	);
	if(ParseResult.ec != std::errc()) return false;
	return Between<Min, Max>(Number);
}

bool ValidHeight(const std::string& String)
{
	std::uintmax_t Number;
	const std::from_chars_result ParseResult = std::from_chars(
		String.data(), String.data() + String.length(), Number
	);
	if(ParseResult.ec != std::errc()) return false;
	const std::string_view Unit(ParseResult.ptr);
	if( Unit == "cm" )		return Between<150, 193>(Number);
	else if( Unit == "in")	return Between<59, 76>(Number);
	return false;
}

const std::unordered_set<std::string> EyeColors{{"amb", "blu", "brn", "gry", "grn", "hzl", "oth"}};
const std::unordered_map<const char*, bool(*)(const std::string&)> RequiredFields{
	{ "byr", ValidNumber<1920, 2002, 4>},
	{ "iyr", ValidNumber<2010, 2020, 4>},
	{ "eyr", ValidNumber<2020, 2030, 4>},
	{ "hgt", ValidHeight},
	{ "hcl", [](const std::string& String) -> bool{return String[0] == '#' && ValidNumber<0, 0xFFFFFF, 6, 16>(String.substr(1));}},
	{ "ecl", [](const std::string& String) -> bool{return EyeColors.count(String);} },
	{ "pid", ValidNumber<0, 999999999, 9>}
};

int main(int argc, char *argv[])
{
	std::unordered_map<std::string, std::string> CurPassport;
	std::size_t Part1{}, Part2{}; std::string CurLine;
	while( std::getline(std::cin, CurLine) )
	{
		if( CurLine.size() == 0 )
		{
			if(
				std::all_of(
					RequiredFields.cbegin(), RequiredFields.cend(),
					[&CurPassport](const auto& Field){ return CurPassport.count(Field.first);}
				)
			)
			{
				++Part1;
				if(
					std::all_of(
						RequiredFields.cbegin(), RequiredFields.cend(),
						[&CurPassport](const auto& Field){ return Field.second(CurPassport[Field.first]);}
					)
				) ++Part2;
			}
			CurPassport.clear();
		}
		std::stringstream LineStream(CurLine); std::string CurAttribute;
		while( LineStream >> CurAttribute)
		{
			const std::size_t Colon = CurAttribute.find_first_of(':');
			CurPassport[CurAttribute.substr(0, Colon)] = CurAttribute.substr(Colon + 1);
		}
	}
	std::cout << Part1 << std::endl; std::cout << Part2 << std::endl;
}