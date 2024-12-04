#include "Day4.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


int main()
{
	std::string fileName {"sample.txt"}; // note that using sample.txt should give an answer of 18

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	// put data in 2d character vector v
	std::string inputStr {};
	std::vector<std::vector<char>> v {{}};
	while (std::getline(inputFile,inputStr))
	{
		std::vector<char> intoArray (inputStr.begin(), inputStr.end());
		v.push_back(intoArray);
	}

	inputFile.close();

	int xmas {};
	xmas = numXMAS(v);

	std::cout << "Number of XMAS in data set: " << xmas << "\n";
	
	return 0;
}

int numXMAS(std::vector<std::vector<char>>)
{
	int xmas {0};


	return xmas;
}
