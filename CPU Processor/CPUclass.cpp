#include "CPU.h"

using namespace std;

// CPU class constructor
CPU::CPU(int set_PC)
{
	PC = set_PC;
	branch = false;
	memRead = false;
	memtoReg = false;
	ALUOp = "";
	memWrite = false;
	ALUSrc = false;
	regWrite = false;
	zero = false;
	PCSrc = false;
	regload = false;
	
	for (int i = 0; i < 32; i++)
	{
		reg[i] = "0";
		mem[i] = "0";
	}
}

// CPU Fetch function
string CPU::Fetch(vector <string> inst_bin)
{
	// Initialize return string
	string full_inst;

	// Read instruction in little endian
	PC = PC + 4;
	for (int i = 0; i < 4; i++)
	{
		PC--;
		// Sets the 32 bit full instruction
		full_inst += inst_bin[PC];
	}

	// Moves to next instruction
	PC_0 = PC + 4;

	// KEEP ONLY FOR TESTING
	// PC = PC + 4;
	// KEEP ONLY FOR TESTING

	return full_inst;
}

// CPU Decode function
bool CPU::Decode(string full_inst, Instruction &in)
{
	// Checks if the opcode in instruction is nop/valid
	if (in.Set_Opcode(full_inst))
	{
		// Sets Controls
		Controller(in.get_opcode(), in.get_funct_3(), in.get_funct_7()); 
		
		// Reads from RegFile
		RegFile(in.get_rd(), in.get_rs_1(), in.get_rs_2());

		// Bit extends the immediate values
		bit_imm = Bit_Extend_Imm(in.get_imm());

		return true;
	}

	return false;
}

string CPU::EX(string val1, string val2, string bit_imm)
{
	// Computes ALU
	if (ALUSrc)
	{
		return ALU(val1, bit_imm);
	}
	else
	{
		return ALU(val1, val2);
	}
}

void CPU::WB(string ALU_result, Instruction& in)
{
	// MemtoReg MUX
	if (memtoReg)
	{
		regload = true;
		RegFile(in.get_rd(), "0", "0");
	}
	else
	{
		regload = true;
		ld_value = ALU_result;
		RegFile(in.get_rd(), "0", "0");
	}
}

void CPU::Mem(string ALU_result)
{
	// Gets memory location of ALU_result
	int location = stoi(ALU_result, 0, 2);
	if (memRead)
	{	
		// Gets the integer value of the ALU result
		ld_value = mem[location];
	}

	if (memWrite)
	{
		// Sets the location in memory to rs_2's data
		mem[location] = data2;
	}
}

void CPU::PCAddr()
{
	// PCSrc MUX
	if (branch && zero)
	{
		PCSrc = true;
	}

	// PC Adders
	string shift_1 = "";
	if (branch)
	{
		// Bit shift left << 1
		for (int i = 0; i < 31; i++)
		{
			shift_1 += bit_imm[i + 1];
		}
		shift_1 += '0';

		// PC Adder
		if (shift_1[0] == '1')
		{
			bitset<32> neg(shift_1);
			neg.flip();
			string temp = addBinary(neg.to_string(), "1");
			PC_1 = PC - stoi(temp, 0, 2);
			//cout << "PC: " << PC << " ";
			//cout << "stoi: " << stoi(temp, 0, 2) << " " ;
			//cout << "(PC + stoi): " << PC_1 << endl;
		}
		else
		{
			PC_1 = PC + stoi(shift_1, 0, 2);
			//cout << "PC: " << PC << " ";
			//cout << "stoi: " << stoi(shift_1, 0, 2) << endl;
		}
	}

	if (PCSrc)
	{
		PC = PC_1;
	}
	else
	{
		PC = PC_0;
	}
}

string CPU::ALU(string val1, string val2)
{
	// Bit extend the values for ALU
	string num1 = Bit_Extend_Imm(val1);
	string num2 = Bit_Extend_Imm(val2);

	// ALU Result
	// Add
	if (ALUOp == "0010")
	{
		return addBinary(num1, num2);
	}

	// Subtract
	else if (ALUOp == "0110")
	{
		// Two's Complement
		bitset<32> value(num2);
		value.flip();
		string temp = addBinary(value.to_string(), "1");

		// Checks for zero for branching to toggle flag
		if (addBinary(num1, temp) == "00000000000000000000000000000000")
		{
			zero = true;
		}

		return addBinary(num1, temp);
	}

	// Or
	else if (ALUOp == "0001")
	{
		string result = "00000000000000000000000000000000";
		for (int i = 0; i < 32; i++)
		{
			if (num1[i] == '1' || num2[i] == '1')
			{
				result[i] = '1';
			}
			else
			{
				result[i] = '0';
			}
		}

		return result;
	}

	// And
	else if (ALUOp == "0000")
	{
		string result = "00000000000000000000000000000000";
		for (int i = 0; i < 32; i++)
		{
			if (num1[i] == '1' && num2[i] == '1')
			{
				result[i] = '1';
			}
			else
			{
				result[i] = '0';
			}
		}
		return result;
	}
	
}

string CPU::addBinary(string a, string b)
{
	string result = ""; // Initialize result 
	int s = 0;          // Initialize digit sum 

	// Traverse both strings starting from last 
	// characters 
	int i = a.size() - 1, j = b.size() - 1;
	while (i >= 0 || j >= 0 || s == 1)
	{
		// Comput sum of last digits and carry 
		s += ((i >= 0) ? a[i] - '0' : 0);
		s += ((j >= 0) ? b[j] - '0' : 0);

		// If current digit sum is 1 or 3, add 1 to result 
		result = char(s % 2 + '0') + result;

		// Compute carry 
		s /= 2;

		// Move to next digits 
		i--; j--;
	}

	// If result is more than 32 bits
	//		set binary value to 32 bits
	if (result.length() > 32)
	{
		int diff = result.length() - 32;
		string temp;
		for (int i = diff; i < result.length(); i++)
		{
			temp += result[i];
		}
		result = temp;
	}

	return result;
}

// Function for Bit Extension for immediates
string CPU::Bit_Extend_Imm(string immd)
{
	// Extends til 32 bits for immediate
	while (immd.length() != 32)
	{
		if (immd[0] == '0')
		{
			// Adds 0s if bit extend 0
			immd.insert(0, "0");
		}
		else
		{
			// Adds 1s if bit extend 1
			immd.insert(0, "1");
		}
	}
	return immd;
}

// CPU Register File
void CPU::RegFile(string regD, string reg1, string reg2)
{
	// Check for regWrite
	if (regload)
	{
		// Read destination register
		int rd = stoi(regD, 0, 2);
		reg[rd] = ld_value;
	}
	else
	{
		// Read register 1
		int rs_1 = stoi(reg1, 0, 2);

		// Read register 2
		int rs_2 = stoi(reg2, 0, 2);

		// Read data 1
		data1 = reg[rs_1];

		// Read data 2
		data2 = reg[rs_2];
	}
}

// CPU Controller
void CPU::Controller(string opcode, string function_1, string function_2)
{
	// R-type
	if (opcode == "0110011")
	{
		regWrite = true;
		if (function_1 == "000" && function_2 == "0000000")
		{
			ALUOp = "0010";
		}
		else if (function_1 == "000" && function_2 == "0100000")
		{
			ALUOp = "0110";
		}
		else if (function_1 == "110")
		{
			ALUOp = "0001";
		}
		else if (function_1 == "111")
		{
			ALUOp = "0000";
		}
	}
	// I-type
	else if (opcode == "0010011") 
	{
		regWrite = true;
		ALUSrc = true;
		if (function_1 == "000")
		{
			ALUOp = "0010";
		}
		else if (function_1 == "110")
		{
			ALUOp = "0001";
		}
		else if (function_1 == "111")
		{
			ALUOp = "0000";
		}
	}

	// Load
	else if (opcode == "0000011")
	{
		regWrite = true;
		ALUSrc = true;
		memRead = true;
		memtoReg = true;
		ALUOp = "0010";
	}

	// Store
	else if (opcode == "0100011")
	{
		ALUSrc = true;
		memWrite = true;
		ALUOp = "0010";
	}

	// Branch
	else if (opcode == "1100011")
	{
		branch = true;
		ALUOp = "0110";
	}
}

// Getter for control flags
bool CPU::get_regWrite()
{
	return regWrite;
}

bool CPU::get_memWrite()
{
	return memWrite;
}

bool CPU::get_memRead()
{
	return memRead;
}

bool CPU::get_ALUSrc()
{
	return ALUSrc;
}

bool CPU::get_zero()
{
	return zero;
}

// Standard getters
string CPU::get_data1()
{
	return data1;
}

string CPU::get_data2()
{
	return data2;
}

string CPU::get_bitimm()
{
	return bit_imm; 
}

// Getter for reg[index]
string CPU::get_reg(int index)
{
	return reg[index];
}

// Getter for mem[index]
string CPU::get_mem(int index)
{
	return mem[index];
}

// Getter for binary to int
int CPU::get_int(string bin)
{
	if (bin[0] == '1')
	{
		bitset<32> value(bin);
		value.flip();
		string temp = addBinary(value.to_string(), "1");
		return -1 * (stoi(temp, 0, 2));
	}
	
	return stoi(bin, 0, 2);
}

// Resets the controls
void CPU::resetControls()
{
	 branch = false;
	 memRead = false;
	 memtoReg = false;
	 memWrite = false;
	 ALUSrc = false;
	 PCSrc = false;
	 regWrite = false;
	 regload = false;
	 zero = false;
	 ALUOp = "";
	 bit_imm = "";
	 ld_value = "";
}