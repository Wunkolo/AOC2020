#include <bits/stdc++.h> 

int main()
{
	std::map<std::string, std::set<std::string>> AllergensToIngredients;
	std::multiset<std::string> AllIngredients;
	{
		std::string CurLine;
		while( std::getline(std::cin, CurLine) )
		{
			std::stringstream LineStream(CurLine);
			std::set<std::string> CurIngredients;
			std::set<std::string> CurAllergens;
			std::string CurToken;
			// Parse ingredients
			while(LineStream >> CurToken)
			{
				if( CurToken[0] == '(') // "(contains"
				{
					break;
				}
				else CurIngredients.insert(CurToken);
			}
			AllIngredients.insert(CurIngredients.cbegin(), CurIngredients.cend());
			// Parse allergens
			while(LineStream >> CurToken)
			{
				// Ignore the last ',' or ')'
				const auto CurAllergen = CurToken.substr(0, CurToken.size() - 1);
				CurAllergens.insert(CurAllergen);
				if(AllergensToIngredients.count(CurAllergen))
				{
					// Intersect
					auto& OtherIngredients = AllergensToIngredients.at(CurAllergen);
					std::set<std::string> Intersection;
					std::set_intersection(
						OtherIngredients.cbegin(), OtherIngredients.cend(),
						CurIngredients.cbegin(), CurIngredients.cend(),
						std::inserter(Intersection, Intersection.begin())
					);
					OtherIngredients = std::move(Intersection);
				}
				else
				{
					// Assign
					AllergensToIngredients.insert({CurAllergen, CurIngredients});
				}
			}
		}
	}
	// We want to reduce all of AllergensToIngredients's sets to have
	// just one element. If there is at least one element identity-element
	// then this should take exactly AllergensToIngredients.size() steps
	// <Allergen, Ingredient>
	while( std::any_of(
		AllergensToIngredients.cbegin(), AllergensToIngredients.cend(),
		[](auto& It){return It.second.size() > 1;})
	)
	{
		for( const auto& AllergenToIngredientA : AllergensToIngredients )
		{
			// We found an identity-element
			if(AllergenToIngredientA.second.size() == 1)
			{
				const auto& ToRemove = *AllergenToIngredientA.second.begin();
				// Remove this element from all other elements
				for( auto& AllergenToIngredientB : AllergensToIngredients )
				{
					if(AllergenToIngredientA.first == AllergenToIngredientB.first)
					{
						// Dont remove it from ourselves
						continue;
					}
					AllergenToIngredientB.second.erase(ToRemove);
				}
			}
		}
	}
	// At this point, AllergensToIngredients is fully solved

	// Count how any times non-allergic ingredients show up
	std::uintmax_t Part1{};
	{
		std::multiset<std::string> SafeIngredients = AllIngredients;
		for(const auto& AllergicIngredient : AllergensToIngredients)
		{
			SafeIngredients.erase(*AllergicIngredient.second.begin());
		}
		Part1 = SafeIngredients.size();
	}
	std::cout << Part1 << std::endl;

	std::string Part2;
	{
		// std::map is already sorted!
		for( const auto& AllergenToIngredient : AllergensToIngredients )
		{
			Part2 += *AllergenToIngredient.second.begin() + ',';
		}
		Part2.pop_back();
	}
	std::cout << Part2 << std::endl;
}