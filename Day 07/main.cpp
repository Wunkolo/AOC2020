#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <stack>

using ContainedT = std::unordered_map<std::string, std::unordered_map<std::string, std::size_t>>;
using ContainedInT = std::unordered_map<std::string, std::unordered_set<std::string>>;

std::uintmax_t ParentCount( const ContainedInT& ContainedIn, const std::string& Color = "shinygold")
{
	std::unordered_set<std::string> ContainedColor;
	std::stack<std::string> Queue;
	Queue.push(Color);

	while( Queue.size() )
	{
		const std::string CurColor = Queue.top(); Queue.pop();
		if( ContainedIn.count(CurColor) )
		{
			for(auto Container : ContainedIn.at(CurColor))
			{
				ContainedColor.insert(Container);
				Queue.push(Container);
			}
		}
	}
	return ContainedColor.size();
}

std::uintmax_t BagCount( const ContainedT& Contained, const std::string& Color = "shinygold" )
{
	std::uintmax_t Total = 0;
	if( Contained.count(Color) )
	{
		for(auto Rule : Contained.at(Color))
		{
			Total += Rule.second * (BagCount(Contained, Rule.first) + 1);
		}
	}
	return Total;
}

int main()
{
	std::string CurLine{}, Cull{}; ContainedT Contained; ContainedInT ContainedIn;
	while( std::getline(std::cin, CurLine) )
	{
		std::stringstream LineStream(CurLine); std::string Adjective{}, Color{};
		LineStream >> Adjective >> Color >> Cull >> Cull;
		while( LineStream )
		{
			std::uintmax_t Count;
			std::string InnerAdjective{}, InnerColor{};
			if( LineStream >> Count >> InnerAdjective >> InnerColor >> Cull )
			{
				ContainedIn[InnerAdjective + InnerColor].insert(Adjective + Color);
				Contained[Adjective + Color][InnerAdjective + InnerColor] = Count;
			}
		}
	}

	std::cout << ParentCount(ContainedIn) << '\n' << BagCount(Contained) << std::endl;
}
