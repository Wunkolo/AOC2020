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
		Acc = Opcode("acc"), Jmp = Opcode("jmp"), Nop = Opcode("nop")
	} Op;
	std::intmax_t Operand;
};

bool Execute(const std::vector<Instruction>& Program, std::intmax_t& Result)
{
	std::vector<bool> Visited(Program.size());
	Result = 0; std::uintmax_t ProgramCounter{};

	while( Visited[ProgramCounter] == false && ProgramCounter < Program.size() )
	{
		const Instruction& CurInstruction = Program[ProgramCounter];
		Visited[ProgramCounter] = true;
		switch(CurInstruction.Op)
		{
			case Instruction::OpCode::Acc: Result += CurInstruction.Operand;
			case Instruction::OpCode::Nop: ++ProgramCounter; break;
			case Instruction::OpCode::Jmp: ProgramCounter += CurInstruction.Operand; break;
		}
	}
	return ProgramCounter >= Program.size() ? true : false;
}

std::intmax_t Part2(const std::vector<Instruction>& Program)
{
	std::intmax_t CurResult; std::vector<Instruction> NewProgram = Program;
	for(std::size_t i = 0; i < Program.size(); ++i)
	{
		const Instruction& CurInstruction = Program[i];
		Instruction::OpCode NewOpCode = {};
		switch(CurInstruction.Op)
		{
			case Instruction::OpCode::Jmp: NewOpCode = Instruction::OpCode::Nop; break;
			case Instruction::OpCode::Nop: NewOpCode = Instruction::OpCode::Jmp; break;
		}
		if(NewOpCode)
		{
			NewProgram[i].Op = NewOpCode;
			if(Execute(NewProgram, CurResult)) return CurResult;
			NewProgram[i].Op = CurInstruction.Op;
		}
	}
	return 0;
}

int main()
{
	char Op[4]; std::intmax_t Operand, Part1;
	std::vector<Instruction> Program;
	while( std::scanf(" %3s %ju", Op, &Operand) == 2 )
		Program.push_back({*(Instruction::OpCode*)Op, Operand});
	Execute(Program, Part1);
	std::cout << Part1 << '\n' << Part2(Program) << std::endl;
}
