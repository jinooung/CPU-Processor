#include <iostream>
#include <vector>
#include <string>
#include "CPU.h"

using namespace std;

class CPUStat
{
	public:
		CPUStat(int set_CPUStat);

		void checkFlags(string test);
		void resetFlags();
		void Inc_TotalInst(CPU &myCPU, Instruction &inst);
		void Inc_Add();
		void printStats(CPU& myCPU);


	private:
		unsigned int total_inst;
		unsigned int R_type;
		unsigned int I_type;
		unsigned int S_type;
		unsigned int B_type;
		unsigned int U_type;
		unsigned int J_type;
		unsigned int total_SW;
		unsigned int total_LW;
		unsigned int total_ADD;
		unsigned int taken;

		bool d1;
		bool d2;
};



