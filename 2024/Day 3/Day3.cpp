#include "Day2.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <bits/stdc++.h>
#include <regex>

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
	int mult{1};
	while (std::getline(inputFile,inputStr))
	{


		

	}

	inputFile.close();


	std::cout << "Total Multiplcation: " << mult << "\n";

	return 0;
}

