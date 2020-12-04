#include <iostream>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <charconv>

template<std::uintmax_t Min, std::uintmax_t Max>
bool Between(std::uintmax_t Number){return (Min <= Number) && (Number <= Max);}

template<std::uintmax_t Min, std::uintmax_t Max, std::uintmax_t Digits, std::uintmax_t Base = 10>
bool ValidNumber(std::string_view String)
{
	if(String.length() != Digits) return false;
	std::uintmax_t Number;
	const std::from_chars_result ParseResult = std::from_chars(
		String.data(), String.data() + Digits, Number, Base
	); if(ParseResult.ec != std::errc()) return false;
	return Between<Min, Max>(Number);
}

bool ValidHeight(std::string_view String)
{
	std::uintmax_t Number;
	const std::from_chars_result ParseResult = std::from_chars(
		String.data(), String.data() + String.length(), Number
	); if(ParseResult.ec != std::errc()) return false;
	const std::string_view Unit(ParseResult.ptr);
	if( Unit == "cm" )		return Between<150, 193>(Number);
	else if( Unit == "in")	return Between<59, 76>(Number);
	return false;
}

bool ValidHairColor(std::string_view String)
{
	return String[0] == '#' && ValidNumber<0, 0xFFFFFF, 6, 16>(String.substr(1));
}

template<std::size_t N>
inline constexpr std::uint32_t Tag( const char (&TagString)[N])
{
	static_assert(N == 4, "Tag must be 3 characters");
	return (TagString[2] << 16) | (TagString[1] <<  8) | (TagString[0]);
}

bool ValidEyeColor(std::string_view String)
{
	if( String.length() != 3 ) return false;
	switch( *(std::uint32_t*)String.data() )
	{
		case Tag("amb"): case Tag("blu"): case Tag("brn"): case Tag("gry"):
		case Tag("grn"): case Tag("hzl"): case Tag("oth"):
			return true;
	}
	return false;
}

const static std::unordered_map<std::uint32_t, bool(*)(std::string_view)> RequiredFields{
	{ Tag("byr"), ValidNumber<1920, 2002, 4>},
	{ Tag("iyr"), ValidNumber<2010, 2020, 4>},
	{ Tag("eyr"), ValidNumber<2020, 2030, 4>},
	{ Tag("hgt"), ValidHeight},
	{ Tag("hcl"), ValidHairColor},
	{ Tag("ecl"), ValidEyeColor },
	{ Tag("pid"), ValidNumber<0, 999999999, 9>}
};

int main()
{
	std::unordered_map<std::uint32_t, std::string> CurPassport;
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
			CurPassport[*(std::uint32_t*)CurAttribute.substr(0, Colon).data()] = CurAttribute.substr(Colon + 1);
		}
	}
	std::cout << Part1 << std::endl; std::cout << Part2 << std::endl;
}
