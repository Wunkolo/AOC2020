#include <iostream>
#include <cstdio>
#include <vector>
#include <cctype>
#include <stack>
#include <queue>

std::uintmax_t Evaluate(const std::vector<char>& Expression, bool Part2 = false)
{
	std::stack<char> OperatorStack;
	std::stack<std::uintmax_t> OperandStack;
	for(const auto& Token : Expression)
	{
		if( std::isdigit(Token) ) OperandStack.push(Token - '0');
		else if( Token == '(') OperatorStack.push(Token);
		else if( Token == ')')
		{
			while(OperatorStack.size() && OperatorStack.top() != '(')
			{
				const auto OperandA = OperandStack.top(); OperandStack.pop();
				const auto OperandB = OperandStack.top(); OperandStack.pop();
				switch(OperatorStack.top())
				{
					case '+': OperandStack.push(OperandA + OperandB); break;
					case '*': OperandStack.push(OperandA * OperandB); break;
				}
				OperatorStack.pop();
			}
			if(OperatorStack.top() == '(') OperatorStack.pop();
		}
		else if( Token == '+' || Token == '*' )
		{
			while(
				OperatorStack.size() && OperatorStack.top() != '('
				&& (!Part2 || (Part2 && Token == '*' && OperatorStack.top() == '+'))
			)
			{
				const auto OperandA = OperandStack.top(); OperandStack.pop();
				const auto OperandB = OperandStack.top(); OperandStack.pop();
				switch(OperatorStack.top())
				{
					case '+': OperandStack.push(OperandA + OperandB); break;
					case '*': OperandStack.push(OperandA * OperandB); break;
				}
				OperatorStack.pop();
			}
			OperatorStack.push(Token);
		}
	}
	while(OperatorStack.size())
	{
		const auto OperandA = OperandStack.top(); OperandStack.pop();
		const auto OperandB = OperandStack.top(); OperandStack.pop();
		switch(OperatorStack.top())
		{
			case '+': OperandStack.push(OperandA + OperandB); break;
			case '*': OperandStack.push(OperandA * OperandB); break;
		}
		OperatorStack.pop();
	}
	return OperandStack.top();
}

int main()
{
	std::string CurLine;
	std::vector<std::vector<char>> Expressions;
	while( std::getline(std::cin, CurLine) )
	{
		std::vector<char> CurExpression;
		for(const auto& Char : CurLine)
		{
			if( Char != ' ') CurExpression.push_back(Char);
		}
		Expressions.push_back(std::move(CurExpression));
	}

	std::uintmax_t Part1{};
	for(const auto& CurExpression : Expressions) Part1 += Evaluate(CurExpression);
	std::cout << Part1 << std::endl;

	std::uintmax_t Part2{};
	for(const auto& CurExpression : Expressions) Part2 += Evaluate(CurExpression, true);
	std::cout << Part2 << std::endl;
}