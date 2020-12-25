#include <cstddef>
#include <cstdint>
#include <iostream>

// https://stackoverflow.com/a/8498251/531719
// Transform (a, b, m)
inline std::uintmax_t Transform(std::uintmax_t Base, std::uintmax_t Exp)
{
	Base %= 20201227ull;
	std::uintmax_t Result = 1;
	while (Exp > 0)
	{
		if (Exp & 1) Result = (Result * Base) % 20201227ull;
		Base = (Base * Base) % 20201227ull;
		Exp >>= 1;
	}
	return Result;
}

int main()
{
	std::uintmax_t PublicKeyCard, PublicKeyDoor;
	std::cin >> PublicKeyCard >> PublicKeyDoor;

	std::size_t CardLoop{}, DoorLoop{};
	for(std::size_t i = 1; !(CardLoop && DoorLoop); ++i)
	{
		const auto CurTransform = Transform(7ull, i);
		if(PublicKeyCard == CurTransform) CardLoop = i;
		if(PublicKeyDoor == CurTransform) DoorLoop = i;
	}
	std::cout << CardLoop << ' ' << DoorLoop << std::endl;

	std::cout << Transform(PublicKeyCard, DoorLoop) << std::endl;
	std::cout << Transform(PublicKeyDoor, CardLoop) << std::endl;
}