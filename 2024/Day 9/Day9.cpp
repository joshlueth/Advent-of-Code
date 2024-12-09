#include "Day9.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>


int main()
{
	auto t1 {std::chrono::high_resolution_clock::now()};

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

	auto t2 {std::chrono::high_resolution_clock::now()};

	
	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";



	return 0;
}
