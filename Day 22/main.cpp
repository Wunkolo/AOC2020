#include <bits/stdc++.h> 

std::uintmax_t CalcScore(const std::list<std::uintmax_t>& Deck)
{
	std::uintmax_t Score{};
	std::uintmax_t Index = 1;
	for( auto CurCard = Deck.crbegin(); CurCard != Deck.crend(); ++CurCard)
	{
		Score += *CurCard * Index;
		++Index;
	}
	return Score;
}

template< bool Part2 = false>
std::pair<std::uintmax_t, std::list<std::uintmax_t>> Simulate(
	std::map<std::uintmax_t, std::list<std::uintmax_t>> PlayerDecks
)
{
	std::set<std::uintmax_t> PreviousDecks;
	std::uintmax_t WinningPlayer{};
	for( std::size_t i = 1;; ++i )
	{
		// Check for winner
		{
			std::uintmax_t HasCards{};
			for(auto& Player : PlayerDecks) if(Player.second.size()) ++HasCards;
			if(HasCards == 1) break;
		}
		// Add each deck to the previously seen list
		std::uintmax_t CurDeckHash{};
		for(auto& Player : PlayerDecks)
		{
			CurDeckHash |= CalcScore(Player.second) << (Player.first * 16);
		}
		if(Part2 && PreviousDecks.count(CurDeckHash))
		{
			// Player 1 wins
			return {1,PlayerDecks[1]};
		}
		PreviousDecks.insert(CurDeckHash);

		// Each player draws a card
		std::map<std::uintmax_t, std::uintmax_t> EachPlayerCard;
		for(auto& Player : PlayerDecks)
		{
			// This player lost already
			if(Player.second.size() == 0 ) continue;
			// This player draws their top card
			const auto PlayerCard = Player.second.front(); Player.second.erase(Player.second.begin());
			EachPlayerCard[Player.first] = PlayerCard;
		}

		std::uintmax_t RoundWinner{};
		if(
			Part2 &&
			std::all_of(
				EachPlayerCard.cbegin(), EachPlayerCard.cend(),
				[&PlayerDecks]( const auto& PlayerCard)
				{
					return PlayerDecks.at(PlayerCard.first).size() >= PlayerCard.second;
				}
			)
		)
		{
			// New recursive game
			auto RecursiveDeck = PlayerDecks;
			for( auto& Deck : RecursiveDeck )
			{;
				Deck.second.erase(
					std::next(Deck.second.begin(), EachPlayerCard.at(Deck.first)),
					Deck.second.end()
				);
			}
			RoundWinner = Simulate<Part2>(RecursiveDeck).first;
		}
		else
		{
			RoundWinner = std::max_element(
				EachPlayerCard.cbegin(), EachPlayerCard.cend(),
				[](auto& PlayerCardA, auto& PlayerCardB)
				{
					return PlayerCardA.second < PlayerCardB.second;
				}
			)->first;
		}
		// Winner gets their own card back.
		PlayerDecks[RoundWinner].push_back(EachPlayerCard.at(RoundWinner));
		EachPlayerCard.erase(RoundWinner);
		// Winner gets all other cards
		for(auto& PlayerCard : EachPlayerCard)
		{
			PlayerDecks[RoundWinner].push_back(PlayerCard.second);
		}
	}

	const auto Winner = (*std::max_element(
		PlayerDecks.cbegin(), PlayerDecks.cend(),
		[](auto& PlayerDeckA, auto& PlayerDeckB)
		{
			return PlayerDeckA.second.size() < PlayerDeckB.second.size();
		}
	)).first;

	return {Winner, PlayerDecks[Winner]};
}

int main()
{
	std::map<std::uintmax_t, std::list<std::uintmax_t>> PlayerDecks;
	while( std::cin )
	{
		std::string CurLine;
		std::uintmax_t PlayerNum{};
		if(std::scanf(" Player %ju: ", &PlayerNum) != 1) return 1;
		//std::cout << PlayerNum << std::endl;
		auto& CurDeck = PlayerDecks[PlayerNum];
		while( std::getline(std::cin, CurLine) && CurLine.size())
		{
			//std::cout << '\t' << CurLine << std::endl;
			CurDeck.push_back(std::stoull(CurLine));
		}
	}

	std::cout << CalcScore(Simulate<false>(PlayerDecks).second) << std::endl;
	std::cout << CalcScore(Simulate<true>(PlayerDecks).second) << std::endl;
}