#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string_view>
#include <algorithm>

int main(int argc, char *argv[])
{
	std::uintmax_t OpA, OpB;
	char Letter;
	char Password[256];
	std::uintmax_t Part1{}, Part2{};
	while( std::scanf(" %ju-%ju %c: %256s", &OpA, &OpB, &Letter, Password) == 4 )
	{
		const std::string_view PassPhrase(Password, std::strlen(Password));
		const std::size_t Part1Check = std::count(
			PassPhrase.cbegin(), PassPhrase.cend(), Letter
		);
		Part1 += (Part1Check >= OpA) && (Part1Check <= OpB);
		Part2 += 1 == 
			(PassPhrase[OpA - 1] == Letter)
			+ (PassPhrase[OpB - 1] == Letter);
	}
	std::printf("%ju\n%ju\n", Part1, Part2);
	return EXIT_SUCCESS;
}
