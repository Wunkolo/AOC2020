#include <iostream>
#include <cstdint>
#include <string>
#include <string_view>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <charconv>
constexpr static char Decimal[] = "0123456789";
constexpr static char Hexadecimal[] = "abcdef0123456789";
template<std::uintmax_t Min, std::uintmax_t Max>
bool Between(std::uintmax_t Number)
{
	return (Min <= Number) && (Number <= Max);
}

bool Height(std::string_view HeightString)
{
	const auto UnitIdx = HeightString.find_first_not_of(Decimal);
	if(UnitIdx == std::string::npos) return false;
	std::uintmax_t Number;
	std::from_chars(
		HeightString.cbegin(), HeightString.cbegin() + UnitIdx, Number
	);
	const std::string_view Unit = HeightString.substr(UnitIdx);
	if( Unit == "cm" )		return Between<150, 193>(Number);
	else if( Unit == "in")	return Between<59, 76>(Number);
	return false;
}

int main(int argc, char *argv[])
{
	const std::unordered_set<std::string> EyeColors = {{
		"amb","blu","brn","gry","grn","hzl","oth"
	}};
	std::unordered_map<std::string, std::string> CurPassport;
	std::size_t Part1{}, Part2{};
	std::string CurLine;
	while( std::getline(std::cin, CurLine) )
	{
		if( CurLine.size() == 0 )
		{
			if( 
				CurPassport.count("byr") && CurPassport.count("iyr") &&
				CurPassport.count("eyr") && CurPassport.count("hgt") &&
				CurPassport.count("hcl") && CurPassport.count("ecl") &&
				CurPassport.count("pid")
			)
			{
				++Part1;
				if(
					CurPassport["byr"].size() == 4 && Between<1920, 2002>(std::stoull(CurPassport["byr"])) &&
					CurPassport["iyr"].size() == 4 && Between<2010, 2020>(std::stoull(CurPassport["iyr"])) &&
					CurPassport["eyr"].size() == 4 && Between<2020, 2030>(std::stoull(CurPassport["eyr"])) &&
					Height(CurPassport["hgt"]) &&
					CurPassport["hcl"].size() == 7 && CurPassport["hcl"][0] == '#' && (CurPassport["hcl"].find_first_not_of(Hexadecimal, 1) == std::string::npos) &&
					EyeColors.count(CurPassport["ecl"]) &&
					CurPassport["pid"].size() == 9 && (CurPassport["pid"].find_first_not_of(Decimal) == std::string::npos)
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
	std::cout << Part1 << std::endl;
	std::cout << Part2 << std::endl;
}
