#include "myCPU.h"


vector<unsigned int> Retrieve_Inst(string inFile)
{
	// Vector to hold list of instructions
	vector<unsigned int> inst;

	// Temp values to hold instructions
	unsigned int temp;

	// Input path
	string path = inFile;
	ifstream in(path);
	if (in.is_open())
	{
		// Retrieving data from file to vector
		while (in >> temp)
		{
			inst.push_back(temp);
		}
	}

	return inst;
}

vector <string> Convert_Binary(vector <unsigned int> inst)
{
	vector <string> bin_instructions;
	for (int i = 0; i < inst.size(); i++)
	{
		// Converts integer to binary of 8 bits
		string bin_inst = bitset<8> (inst[i]).to_string();

		// Push binary value into vector
		bin_instructions.push_back(bin_inst);
	}

	for (int i = inst.size(); i < 400; i++)
	{
		string bin_inst = bitset<8>(0).to_string();
		bin_instructions.push_back(bin_inst);
	}

	return bin_instructions;
}

int main(int argc, char* argv[])
{
	vector <unsigned int> inst;
	vector <string> bin_inst;
	string full_inst = "";

	// Error management 
	try 
	{
		// Takes in the input of command line argument
		// as input for instruction memory list
		if (argc > 0)
		{
			// Reading instructions from file
			// then transfers to vector list line by line
			inst = Retrieve_Inst(argv[1]);
		}
	}
	catch (exception e)
	{
		cout << "Some error has occurred" << endl
			 << "Program will now terminate";
		return 0;
	}
	
	// Convert the list of instructions to 8 bit binary
	bin_inst = Convert_Binary(inst);

	// Initializing CPU PC counter to ZERO
	CPU myCPU(0);

	// Initializing CPU Stat Tracker
	CPUStat myStat(0);

	while (1)
	{
		// Initializing Instruction object
		Instruction in("");

		// Fetch
		full_inst = myCPU.Fetch(bin_inst);

		// Decode
		if (!myCPU.Decode(full_inst, in))
		{
			// Break if NOP
			break;
		}

		string ALU_result = myCPU.EX(myCPU.get_data1(), myCPU.get_data2(), myCPU.get_bitimm());
		if (myCPU.get_memRead() || myCPU.get_memWrite())
		{
			myCPU.Mem(ALU_result); 
		}

		myCPU.PCAddr();

		if (myCPU.get_regWrite())
		{
			myCPU.WB(ALU_result, in);
		}

		// Update myStat here
		myStat.Inc_TotalInst(myCPU, in);

		// Reset Memory
		myCPU.resetControls();
		myStat.resetFlags();
		in.resetMem();
	}

	// Check for -d options
	myStat.checkFlags(argv[2]);

	// print the stats
	myStat.printStats(myCPU);
	return 0;
}









	





