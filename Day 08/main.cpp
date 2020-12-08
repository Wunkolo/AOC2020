#include <iostream>
#include <cstdio>
#include <vector>
#include <unordered_set>
#include <optional>

template<std::size_t N>
inline constexpr std::uint32_t Tag( const char (&TagString)[N])
{
	static_assert(N == 4, "Tag must be 3 characters");
	return (TagString[2] << 16) | (TagString[1] <<  8) | (TagString[0]);
}

struct Instruction
{
	enum OpCode : std::uint32_t
	{
		Accumulate	= Tag("acc"),
		Jump		= Tag("jmp"),
		NoOperation	= Tag("nop")
	} Operation;
	std::intmax_t Operand;
};

bool Execute(const std::vector<Instruction>& Program, std::intmax_t& Result)
{
	std::unordered_set<std::uintmax_t> Visited;
	std::intmax_t Accumulator = 0;
	std::uintmax_t ProgramCounter = 0;

	while(Visited.count(ProgramCounter) == 0 && ProgramCounter < Program.size())
	{
		const Instruction& CurInstruction = Program[ProgramCounter];
		Visited.insert(ProgramCounter);
		switch(CurInstruction.Operation)
		{
			case Instruction::OpCode::Accumulate:
			{
				Accumulator += CurInstruction.Operand;
				++ProgramCounter;
				break;
			}
			case Instruction::OpCode::Jump:
			{
				ProgramCounter += CurInstruction.Operand;
				break;
			}
			case Instruction::OpCode::NoOperation:
			{
				++ProgramCounter;
				break;
			}
		}
	}
	Result = Accumulator;
	return ProgramCounter >= Program.size() ? true : false;
}

std::intmax_t Part2(const std::vector<Instruction>& Program)
{
	std::intmax_t CurResult;
	for(std::size_t i = 0; i < Program.size(); ++i)
	{
		const Instruction& CurInstruction = Program[i];
		switch(CurInstruction.Operation)
		{
			case Instruction::OpCode::Jump:
			{
				std::vector<Instruction> CurProgram = Program;
				CurProgram[i].Operation = Instruction::OpCode::NoOperation;
				if(Execute(CurProgram, CurResult)) return CurResult;
				break;
			}
			case Instruction::OpCode::NoOperation:
			{
				std::vector<Instruction> CurProgram = Program;
				CurProgram[i].Operation = Instruction::OpCode::Jump;
				if(Execute(CurProgram, CurResult)) return CurResult;
				break;
			}
		}
	}
	return 0;
}

int main()
{
	char Operation[4]; std::intmax_t Operand;
	std::vector<Instruction> Program;
	while( std::scanf(" %4s %ju", Operation, &Operand) == 2 )
		Program.push_back({*(Instruction::OpCode*)Operation, Operand});
	std::intmax_t Part1; Execute(Program, Part1);
	std::cout << Part1 << '\n' << Part2(Program) << std::endl;
}
