#include <iostream>
#include <cstdio>
#include <vector>
#include <numeric>
#include <cctype>
#include <stack>
#include <queue>

template< bool Part2 = false >
std::uintmax_t Evaluate(const std::vector<char>& Expression)
{
	std::stack<char> OperatorStack;
	std::stack<std::uintmax_t> OperandStack;
	const auto EvalTerm = [&OperatorStack, &OperandStack]()
	{
		const auto OperandA = OperandStack.top(); OperandStack.pop();
		const auto OperandB = OperandStack.top(); OperandStack.pop();
		switch(OperatorStack.top())
		{
			case '+': OperandStack.push(OperandA + OperandB); break;
			case '*': OperandStack.push(OperandA * OperandB); break;
		}
		OperatorStack.pop();
	};
	for(const auto& Token : Expression)
	{
		switch(Token)
		{
			case '(': OperatorStack.push(Token); break;
			case ')':
			{
				while(OperatorStack.size() && OperatorStack.top() != '(') EvalTerm();
				if(OperatorStack.top() == '(') OperatorStack.pop();
				break;
			}
			case '+':
			case '*':
			{
				while(
					OperatorStack.size() && OperatorStack.top() != '('
					&& (!Part2 || (Part2 && Token == '*' && OperatorStack.top() == '+'))
				) EvalTerm();
				OperatorStack.push(Token);
				break;
			}
			default:
				if( std::isdigit(Token) ) OperandStack.push(Token - '0'); break;
		}
	}
	while(OperatorStack.size()) EvalTerm();
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

	std::cout << std::transform_reduce(
		Expressions.cbegin(), Expressions.cend(),
		0ull, std::plus<>(), Evaluate<false>
	) << std::endl;

	std::cout << std::transform_reduce(
		Expressions.cbegin(), Expressions.cend(),
		0ull, std::plus<>(), Evaluate<true>
	) << std::endl;
}