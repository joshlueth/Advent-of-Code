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
		sum += doMultOnStr(inputStr);
	}

	inputFile.close();

	std::cout << "Total Multiplcation: " << sum << "\n";

	// with don't(), do() disabling/enabling the multiplication instructions 

	inputFile.open(fileName);
	sum = 0;
	bool multEnabled {true};
	while (std::getline(inputFile,inputStr))
	{
		std::regex enabled {".+?(don't\\(\\)|$)"};
		for (std::smatch enabledMatch{}; std::regex_search(inputStr, enabledMatch, enabled);)
		{
			std::string enabledStr {enabledMatch.str()};
			multEnabled = false;
			sum += doMultOnStr(enabledStr);

			std::regex disabled {"do\\(\\)"};
			std::smatch disabledMatch{};
			std::string disabledStr {enabledMatch.suffix()};
			std::regex_search(disabledStr,disabledMatch,disabled);

			inputStr = disabledMatch.suffix();

		}

	}

	inputFile.close();

	std::cout << "Total Multiplcation, refined: " << sum << "\n";

	return 0;
}

int doMultOnStr(std::string string)
{
	int sum {0};
	std::regex multFormat {"mul\\([0-9]+,[0-9]+\\)"};
	for (std::smatch strMatch{}; std::regex_search(string, strMatch, multFormat);)
	{
		std::regex multNumber {"[0-9]+"};
		std::string multStr {strMatch.str()};
		std::smatch numMatch{}; 
		int mult {1};
		while (std::regex_search(multStr, numMatch, multNumber))
		{

			mult *= std::stoi(numMatch.str());
			multStr = numMatch.suffix();

		}
		sum += mult;
		string = strMatch.suffix();

	}

	return sum;
}
