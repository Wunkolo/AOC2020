#include <iostream>
#include <sstream>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <functional>
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

std::uint16_t ValidateField(std::uint32_t Field, std::string_view Value)
{
	std::uint16_t Result{};
	switch( Field )
	{
	case Tag("byr"): Result |= (ValidNumber<1920,2002,4>  (Value) << 8 | 1) << 0; break;
	case Tag("iyr"): Result |= (ValidNumber<2010,2020,4>  (Value) << 8 | 1) << 1; break;
	case Tag("eyr"): Result |= (ValidNumber<2020,2030,4>  (Value) << 8 | 1) << 2; break;
	case Tag("hgt"): Result |= (ValidHeight				  (Value) << 8 | 1) << 3; break;
	case Tag("hcl"): Result |= (ValidHairColor			  (Value) << 8 | 1) << 4; break;
	case Tag("ecl"): Result |= (ValidEyeColor			  (Value) << 8 | 1) << 5; break;
	case Tag("pid"): Result |= (ValidNumber<0,999999999,9>(Value) << 8 | 1) << 6; break;
	}
	return Result;
}
void Split(std::string_view String, std::function<void(std::string_view)> Proc, char Delim = '\n')
{
	for(
		auto Cur = String.data(), End = Cur + String.length();
		Cur != End; Cur += ((Cur==End) ? 0 : 1)
	)
	{
		const auto Beg = Cur; Cur = std::search_n(Beg, End, 1, Delim);
		if (Cur != Beg) Proc(std::string_view(Beg, Cur - Beg));
	}
}

int main()
{
	std::unordered_map<std::uint32_t, std::string> CurPassport;
	std::size_t Part1{}, Part2{}; std::string CurLine;
	while( std::getline(std::cin, CurLine) )
	{
		if( CurLine.size() == 0 )
		{
			const std::uint16_t Tests = std::transform_reduce(
				CurPassport.cbegin(), CurPassport.cend(), 0, std::bit_or<>(),
				[](auto Attr){return ValidateField(Attr.first, Attr.second);}
			); CurPassport.clear();
			Part1 += std::uint8_t(Tests) == 0x7F; Part2 += Tests == 0x7F'7F;
		}
		Split(
			CurLine,
			[&CurPassport](std::string_view CurAttribute)
			{
				const std::size_t Colon = CurAttribute.find_first_of(':');
				CurPassport[0xFFFFFF & *(std::uint32_t*)CurAttribute.substr(0, Colon).data()] = CurAttribute.substr(Colon + 1);
			},
			' '
		);
	}
	std::cout << Part1 << std::endl; std::cout << Part2 << std::endl;
}
