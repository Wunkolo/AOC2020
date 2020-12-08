#include <iostream>
#include <cstdio>
#include <vector>

template<std::size_t N>
inline constexpr std::uint32_t Opcode( const char (&OpString)[N])
{
	static_assert(N == 4, "Opcode must be 3 characters");
	return (OpString[2] << 16) | (OpString[1] <<  8) | (OpString[0]);
}

struct Instruction
{
	enum OpCode : std::uint32_t
	{
		Acc	= Opcode("acc"),
		Jmp	= Opcode("jmp"),
		Nop	= Opcode("nop")
	} Operation;
	std::intmax_t Operand;
};

bool Execute(const std::vector<Instruction>& Program, std::intmax_t& Result)
{
	std::vector<bool> Visited(Program.size(), false);
	std::intmax_t Accumulator = 0;
	std::uintmax_t ProgramCounter = 0;

	while(Visited[ProgramCounter] == false && ProgramCounter < Program.size())
	{
		const Instruction& CurInstruction = Program[ProgramCounter];
		Visited[ProgramCounter] = true;
		switch(CurInstruction.Operation)
		{
			case Instruction::OpCode::Acc:
			{
				Accumulator += CurInstruction.Operand;
				++ProgramCounter;
				break;
			}
			case Instruction::OpCode::Jmp:
			{
				ProgramCounter += CurInstruction.Operand;
				break;
			}
			case Instruction::OpCode::Nop:
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
	std::vector<Instruction> NewProgram = Program;
	for(std::size_t i = 0; i < Program.size(); ++i)
	{
		const Instruction& CurInstruction = Program[i];
		Instruction::OpCode NewOpCode = {};
		switch(CurInstruction.Operation)
		{
			case Instruction::OpCode::Jmp: NewOpCode = Instruction::OpCode::Nop; break;
			case Instruction::OpCode::Nop: NewOpCode = Instruction::OpCode::Jmp; break;
		}
		if(NewOpCode)
		{
			NewProgram[i].Operation = NewOpCode;
			if(Execute(NewProgram, CurResult)) return CurResult;
			NewProgram[i].Operation = CurInstruction.Operation;
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
