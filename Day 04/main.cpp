#include <iostream>
#include <cstdint>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <charconv>

template<std::uintmax_t Min, std::uintmax_t Max>
bool Between(std::uintmax_t Number)
{
	return (Min <= Number) && (Number <= Max);
}

template<
	std::uintmax_t Min, std::uintmax_t Max,
	std::uintmax_t Digits, std::uintmax_t Base = 10
>
bool ValidNumber(std::string_view String)
{
	if(String.length() != Digits) return false;
	std::uintmax_t Number;
	const std::from_chars_result ParseResult = std::from_chars(
		String.data(), String.data() + Digits, Number, Base
	);
	if(ParseResult.ec != std::errc()) return false;
	return Between<Min, Max>(Number);
}

bool ValidHeight(std::string_view String)
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

int main(int argc, char *argv[])
{
	const std::unordered_set<std::string> EyeColors = {{
		"amb", "blu", "brn", "gry", "grn", "hzl", "oth"
	}};
	const std::unordered_set<const char*> RequiredFields = {{
		"byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"
	}};
	std::unordered_map<std::string, std::string> CurPassport;
	std::size_t Part1{}, Part2{}, Digits{};
	std::string CurLine;
	while( std::getline(std::cin, CurLine) )
	{
		if( CurLine.size() == 0 )
		{
			if(
				std::all_of(
					RequiredFields.cbegin(), RequiredFields.cend(),
					[&CurPassport](const char* Field){ return CurPassport.count(Field);}
				)
			)
			{
				++Part1;
				if(
					ValidNumber<1920, 2002, 4>(CurPassport["byr"]) &&
					ValidNumber<2010, 2020, 4>(CurPassport["iyr"]) &&
					ValidNumber<2020, 2030, 4>(CurPassport["eyr"]) &&
					ValidHeight(CurPassport["hgt"]) &&
					CurPassport["hcl"][0] == '#' && ValidNumber<0, 0xFFFFFF, 6, 16>(CurPassport["hcl"].substr(1)) &&
					EyeColors.count(CurPassport["ecl"]) &&
					ValidNumber<0, 999999999, 9>(CurPassport["pid"])
				) ++Part2;
			}
			CurPassport.clear();
		}
		std::stringstream LineStream(CurLine); std::string CurAttribute;
		while( std::getline(LineStream, CurAttribute, ' ' ))
		{
			const auto Colon = CurAttribute.find_first_of(':');
			CurPassport[CurAttribute.substr(0, Colon)] = CurAttribute.substr(Colon + 1);
		}
	}
	std::cout << Part1 << std::endl; std::cout << Part2 << std::endl;
}
