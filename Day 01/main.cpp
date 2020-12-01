#include <cstddef>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>

std::uintmax_t Part1( const std::vector<std::uintmax_t>& Numbers )
{
	for( std::size_t i = 0; i < Numbers.size(); ++i )
	{
		for( std::size_t j = 0; j < Numbers.size(); ++j )
		{
			if( Numbers[i] + Numbers[j] == 2020 )
				return Numbers[i] * Numbers[j];
		}
	}
	return {};
}

std::uintmax_t Part2( const std::vector<std::uintmax_t>& Numbers )
{
	for( std::size_t i = 0; i < Numbers.size(); ++i )
	{
		for( std::size_t j = 0; j < Numbers.size(); ++j )
		{
			for( std::size_t k = 0; k < Numbers.size(); ++k )
			{
				if( Numbers[i] + Numbers[j] + Numbers[k] == 2020 )
					return Numbers[i] * Numbers[j] * Numbers[k];
			}
		}
	}
	return {};
}

int main(int argc, char *argv[])
{
	std::vector<std::uintmax_t> Numbers;

	std::ifstream Input(argv[1]);
	std::uintmax_t CurNumber;
	while( Input >> CurNumber ) Numbers.push_back(CurNumber);

	std::cout << Part1(Numbers) << std::endl;
	std::cout << Part2(Numbers) << std::endl;
	return EXIT_SUCCESS;
}
