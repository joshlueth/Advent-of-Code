// PROBLEM
// given two lists of integers, of the same length, in file input.txt:
// sort each of the lists in ascending order
// find the sum of the distance (absolute value of the differences) between each entry in the sorted lists

// SOLUTION
// Read in input file
// Into list1, list2
// Call sorting algorithm on list1, list2
// loop through the length, adding the distance to a sum variable
// print sum variable

#include "Day1.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

int main()
{
	std::string fileName {"input.txt"};

	std::ifstream input {fileName};

	if (!input)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::vector<int> list1 {};
	std::vector<int> list2 {};

	int val{};
	while (!input.eof())
	{
		if (!input.eof())
		{
			input >> val;
			list1.push_back(val);
			input >> val;
			list2.push_back(val);
		}
	}

	input.close();

	std::sort(list1.begin(),list1.end());
	std::sort(list2.begin(),list2.end());

	std::size_t len{};

	len = list1.size();
	if (len != list2.size())
	{
		std::cerr << "Lists are not of equal length\n";
		return 1;
	}

	int sum {0};

	std::size_t ii{0};
	for (; ii<len; ++ii)
	{
		sum += abs(list1[ii]-list2[ii]);
	}

	std::cout << "Sum of distances: " << sum;

	return 0;
}

