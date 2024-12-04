#include "Day3.h"
#include <fstream>
#include <iostream>
#include <string>
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
	int sum{0};
	while (std::getline(inputFile,inputStr))
	{
		std::regex multFormat {"mul\\([0-9]+,[0-9]+\\)"};
		for (std::smatch strMatch{}; std::regex_search(inputStr, strMatch, multFormat);)
		{
			std::regex multNumber {"[0-9]+"};
			std::string multStr {strMatch.str()};
			int mult{1};
			for (std::smatch numMatch{}; std::regex_search(multStr, numMatch, multNumber);)
			{
				mult *= std::stoi(numMatch.str());
			  multStr = numMatch.suffix();
			}
			sum += mult;
			inputStr = strMatch.suffix();

		}

	}

	inputFile.close();

	std::cout << "Total Multiplcation: " << sum << "\n";

	return 0;
}

