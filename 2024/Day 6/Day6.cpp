#include "Day6.h"
#include <fstream>
#include <iostream>
#include <string>


int main()
{
	std::string fileName {"input.txt"};

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::string inputStr {};

	while (std::getline(inputFile,inputStr))
	{
		
	}

	inputFile.close();	


	return 0;
}

