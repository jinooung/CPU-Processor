#include "Instruction.h"

// Instruction class constructor
Instruction::Instruction(string str) 
{
	 opcode  = str;
	 funct_3 = str;
	 funct_7 = str;
	 type    = str;
	 rd      = str;
	 rs_1    = str;
	 rs_2    = str; 
	 imm     = str;
}

// Instruction function to set opcode
bool Instruction::Set_Opcode(string full_inst)
{
	// Initialize string for opcode data
	string op_code;

	// Obtains the opcode from 32 bit instruction
	for (int i = 25; i < full_inst.size(); i++)
	{
		op_code = op_code + full_inst[i];
	}

	// Updates the opcode data in instruction
	opcode = op_code;

	// Checks if the opcode is valid/nop
	if (Set_Operands(full_inst))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Instruction::Set_Operands(string inst_op)
{
	// Opcode for R-type
	// ADD, SUB, OR, AND
	if(opcode == "0110011")
	{
		for (int i = 0; i < 7; i++)
		{
			funct_7 += inst_op[i];
		}

		for (int i = 7; i < 12; i++)
		{
			rs_2 += inst_op[i];
		}

		for (int i = 12; i < 17; i++)
		{
			rs_1 += inst_op[i];
		}

		for (int i = 17; i < 20; i++)
		{
			funct_3 += inst_op[i];
		}

		for (int i = 20; i < 25; i++)
		{
			rd += inst_op[i];
		}

		imm = "0";

		type = "R-type";

		return true;
	}
		
	// Opcode for I-type
	// ADDI, ORI, ANDI
	else if (opcode == "0010011")
	{
		for (int i = 0; i < 12; i++)
		{
			imm += inst_op[i];
		}
		
		for (int i = 12; i < 17; i++)
		{
			rs_1 += inst_op[i];
		}

		for (int i = 17; i < 20; i++)
		{
			funct_3 += inst_op[i];
		}

		for (int i = 20; i < 25; i++)
		{
			rd += inst_op[i];
		}

		rs_2 = "0";
		funct_7 = "0";

		type = "I-type";

		return true;
	}

	// Opcode for B-type
	// BEQ
	else if (opcode == "1100011")
	{	
		// I[12]
		imm[0] = inst_op[0];

		// I[11]
		imm += inst_op[24];

		// I[10~5]
		for (int i = 1; i < 7; i++)
		{
			imm += inst_op[i];
		}

		for (int i = 7; i < 12; i++)
		{
			rs_2 += inst_op[i];
		}

		for (int i = 12; i < 17; i++)
		{
			rs_1 += inst_op[i];
		}

		for (int i = 17; i < 20; i++)
		{
			funct_3 += inst_op[i];
		}

		for (int i = 20; i < 24; i++)
		{
			imm += inst_op[i];
		}

		rd = "0";
		funct_7 = "0";

		type = "B-type";

		return true;
	}
	
	// Opcode for Load
	// LW
	else if (opcode == "0000011")
	{
		for (int i = 0; i < 12; i++)
		{
			imm += inst_op[i];
		}

		for (int i = 12; i < 17; i++)
		{
			rs_1 += inst_op[i];
		}

		for (int i = 17; i < 20; i++)
		{
			funct_3 += inst_op[i];
		}

		for (int i = 20; i < 25; i++)
		{
			rd += inst_op[i];
		}

		rs_2 = "0";
		funct_7 = "0";

		type = "I-type";

		return true;
	}


	// Opcode for S-Type
	// SW
	else if (opcode == "0100011")
	{
		for (int i = 0; i < 7; i++)
		{
			imm += inst_op[i];
		}
		for (int i = 20; i < 25; i++)
		{
			imm += inst_op[i];
		} 

		for (int i = 7; i < 12; i++)
		{
			rs_2 += inst_op[i];
		}

		for (int i = 12; i < 17; i++)
		{
			rs_1 += inst_op[i];
		}

		for (int i = 17; i < 20; i++)
		{
			funct_3 += inst_op[i];
		}

		rd = "0";
		funct_7 = "0";

		type = "S-type";

		return true;
	}

	// Check for NOP instruction
	else
	{
		return false;
	}
}

// Getter functions
string Instruction::get_opcode()
{
	return opcode;
}

string Instruction::get_funct_3()
{
	return funct_3;
}

string Instruction::get_funct_7()
{
	return funct_7;
}

string Instruction::get_rd()
{
	return rd;
}

string Instruction::get_rs_1()
{
	return rs_1;
}

string Instruction::get_rs_2()
{
	return rs_2;
}

string Instruction::get_imm()
{
	return imm;
}

string Instruction::get_type()
{
	return type;
}

void Instruction::resetMem()
{
	 opcode = "0";
	 funct_3 = "0";
	 funct_7 = "0";
	 type = "0";
	 rd = "0";
	 rs_1 = "0";
	 rs_2 = "0";
	 imm = "0";
}