#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Instruction
{
	public:
		Instruction(string str);
		bool Set_Opcode(string type);
		bool Set_Operands(string op_code);

		// Getter functions
		string get_opcode();
		string get_funct_3();
		string get_funct_7();
		string get_rd();
		string get_rs_1();
		string get_rs_2();
		string get_imm();
		string get_type();

		// Reset Memory
		void resetMem();


	private:
		vector<string> full_inst;
		string opcode; 
		string funct_3;
		string funct_7;
		
		string rd;
		string rs_1;
		string rs_2;
		string imm;

		string type;
};