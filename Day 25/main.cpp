#include <cstdint>
#include <cstdio>

// https://stackoverflow.com/a/8498251/531719
// Transform (a, b, m)
inline std::uint32_t Transform(std::uint64_t Base, std::uint32_t Exponent)
{
	Base %= 20201227ull;
	std::uint32_t Result = 1;
	while( Exponent )
	{
		if( Exponent & 1 ) Result = (Result * Base) % 20201227u;
		Base = (Base * Base) % 20201227u;
		Exponent >>= 1;
	}
	return Result;
}

int main()
{
	std::uint32_t PublicKeyCard{}, PublicKeyDoor{}, CardLoop{}, DoorLoop{};
	std::scanf(" %u %u", &PublicKeyCard, &PublicKeyDoor);
	std::uint64_t CurTransform = 1ull;
	for( std::uint32_t i = 1; !(CardLoop && DoorLoop); ++i )
	{
		CurTransform = (CurTransform * 7) % 20201227u;
		if(PublicKeyCard == CurTransform) CardLoop = i;
		if(PublicKeyDoor == CurTransform) DoorLoop = i;
	}
	std::printf(
		"%u %u\n%u %u\n",
		CardLoop, DoorLoop,
		Transform(PublicKeyCard, DoorLoop), Transform(PublicKeyDoor, CardLoop)
	);
}
