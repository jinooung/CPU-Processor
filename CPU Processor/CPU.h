#include <iostream>
#include <vector>
#include <string>
#include <bitset>

#include "Instruction.h"

using namespace std;

class CPU
{
public:
	CPU(int set_PC);
	
	// Single Cycle Processor
	string Fetch(vector <string> inst);
	bool Decode(string inst, Instruction& in);
	void Controller(string opcode, string function_1, string function_2);
	string EX(string val1, string val2, string bit_imm);
	void Mem(string ALU_result);
	void WB(string ALU_result, Instruction& in);
	void PCAddr();

	// Utilities for Processor
	void RegFile(string regD, string reg1, string reg2);
	string Bit_Extend_Imm(string immd);

	// ALU
	string ALU(string val1, string val2);

	// ALU Utility
	string addBinary(string a, string b);

	// Gets control functions
	bool get_regWrite();
	bool get_ALUSrc();
	bool get_memWrite();
	bool get_memRead();
	bool get_zero();

	// Standard getter functions
	string get_data1();
	string get_data2();
	string get_bitimm();

	// Gets the register value at index
	string get_reg(int index);
	string get_mem(int index);

	// Resets controls
	void resetControls();

	// Gets integer value of binary string
	int get_int(string bin);

private:
	unsigned int PC;
	signed int PC_0;
	signed int PC_1;
	string reg[32];
	string mem[32];
	string sw_value;
	string ld_value;
	
	bool branch;
	bool memRead;
	bool memtoReg;
	string ALUOp;
	bool memWrite;
	bool ALUSrc;
	bool regWrite;
	bool regload;

	bool zero;
	bool PCSrc;

	string data1;
	string data2;
	string bit_imm;
};

