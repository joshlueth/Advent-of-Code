#include "Day4.h"
#include <fstream>
#include <iostream>
#include <string>

int main()
{
	std::string fileName {"input.txt"}; // note that using sample.txt should give an answer of 18

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	// put data in 2d character vector v
	std::string inputStr {};
	std::vector<std::vector<char>> v {};
	while (std::getline(inputFile,inputStr))
	{

	}

	inputFile.close();	

	return 0;
}
