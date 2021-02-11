#include "Stat.h"

// CPUStat class constructor
CPUStat::CPUStat(int set_CPUStat)
{
	total_inst = set_CPUStat;
	R_type = set_CPUStat;
	I_type = set_CPUStat;
	S_type = set_CPUStat;
	B_type = set_CPUStat;
	U_type = set_CPUStat;
	J_type = set_CPUStat;
	total_SW = set_CPUStat;
	total_LW = set_CPUStat;
	total_ADD = set_CPUStat;
	taken = set_CPUStat;

	d1 = false;
	d2 = false;
}

void CPUStat::checkFlags(string test)
{
	if (test == "-d1")
	{
		d1 = true;
	}

	if (test == "-d2")
	{
		d2 = true;
	}

	if (test == "-d3")
	{
		d1 = true;
		d2 = true;
	}
}

void CPUStat::resetFlags()
{
	d1 = false;
	d2 = false;
}

// Print stats function
void CPUStat::printStats(CPU& myCPU)
{
	cout << total_inst << endl;
	cout << total_SW << ", " << total_LW << ", " << total_ADD << ", " <<  B_type << endl;
	cout << taken << endl;

	if (d1)
	{
		int i = 0;
		while (i < 32)
		{
			cout << "x" << i << ": " <<  myCPU.get_int(myCPU.get_reg(i));
			cout << "		" << "x" << i + 1 << ": " << myCPU.get_int(myCPU.get_reg(i + 1));
			cout << endl;
			i++; i++;
		}
	}

	if (d2)
	{
		int i = 0;
		while (i < 32)
		{
			cout << "mem[" << i << "]: " << myCPU.get_int(myCPU.get_mem(i));
			cout << "		" << "mem[" << i + 1 << "]: " << myCPU.get_int(myCPU.get_mem(i + 1));
			cout << endl;
			i++; i++;
		}
	}
}

// Function to increment ADD instruction
void CPUStat::Inc_Add()
{
	total_ADD++;
}

void CPUStat::Inc_TotalInst(CPU &myCPU, Instruction &inst)
{
	// Opcode for R-type
	// ADD, SUB, OR, AND
	// Increments R-type counter
	if (inst.get_opcode() == "0110011")
	{
		R_type++;
		if (inst.get_funct_3() == "000" && inst.get_funct_7() == "0000000") 
		{
			Inc_Add();
		}
	}

	// Opcode for I-type
	// ADDI, ORI, ANDI
	// Increments I-type counter
	else if (inst.get_opcode() == "0010011")
	{
		I_type++;
	}

	// Opcode for B-type
	// BEQ
	// Increments B-type counter
	else if (inst.get_opcode() == "1100011")
	{
		B_type++;
		if (myCPU.get_zero())
		{
			taken++;
		}
	}

	// Opcode for Load
	// LW
	// Increments LW counter
	else if (inst.get_opcode() == "0000011")
	{
		total_LW++;
		I_type++;
	}

	// Opcode for S-Type
	// SW
	// Increments S-type counter
	else if (inst.get_opcode() == "0100011")
	{
		total_SW++;
		S_type++;
	}

	// Increments total instruction count
	total_inst++;
}