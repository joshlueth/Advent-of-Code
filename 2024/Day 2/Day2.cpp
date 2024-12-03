#include "Day2.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <bits/stdc++.h>

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
	int safe{0}, safeDamp{0};
	while (std::getline(inputFile,inputStr))
	{
		std::stringstream ss (inputStr);
		std::vector<int> list(std::istream_iterator<int>(ss),{});
		int vecDiffSign{}, increment{1}, incrementDamp{1};
		for (std::size_t i{0}; i<list.size()-1; ++i)
		{
			int diff {list[i+1]-list[i]};
			int diffSign {sign(diff)};
			if (i==0) 
			{
				vecDiffSign = diffSign;
				if (vecDiffSign==0) 
				{
					increment = 0;
					break;
				}
			}
			if (diffSign!=vecDiffSign || abs(diff)>3) 
			{
				increment = 0;
				break;
			}
		}
		safe += increment;
		safeDamp += incrementDamp;
	}

	inputFile.close();

	std::cout << "Number of Safe Reports: " << safe << "\n";
	std::cout << "Number of Safe Reports After Damping: " << safeDamp << "\n";

	return 0;
}

int sign(int number)
{
	if (number>0) return 1;
	if (number<0) return -1;
	return 0;
}

