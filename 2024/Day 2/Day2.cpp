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

#include "Day2.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

int main()
{
	std::string fileName {"input.txt"};

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::vector<int> list1 {};
	std::vector<int> list2 {};


	std::string inputStr {};
	while (std::getline(inputFile,inputStr))
	{
		int val1{}, val2{};
		std::stringstream ss (inputStr);
		ss >> val1 >> val2;
		list1.push_back(val1);
		list2.push_back(val2);
	}

	inputFile.close();

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

	std::cout << "Sum of distances: " << sum <<"\n";


// second problem:
// go through each entry in the first list and calculate how many times it appears in the second list
// the similarity score is the number multiplied by the amount of times it appears
// in order to avoid using brute force, we can use the sorted lists and 'pointer traversal' of some sort
// For each entry in the list:
//   check if the entry is equal to the prior one, in which case we simply add the similarity score again
//   if not, check the entry of the second list corresponding to the pointer
//   if it is the same, add one to counter and increment pointer, continue loop
//   if it is not the same, stop
	std::size_t pointer{0};
	int counter;
	int simScore{0};

	ii = 0;
	for (; ii<len; ++ii)
	{
		// check if the entry is the same as the preceding one
		// std::cout << "list 1: " << list1[ii] << "\n";
		if (ii!=0)
		{
			if (list1[ii]==list1[ii-1])
			{
				simScore += list1[ii]*counter;
				continue;
			}
		}
		// if not, use the pointer variable to calculate the similarity score
		while (list2[pointer]<list1[ii]) 
		{
			// std::cout << "list 2: " << list2[pointer] << "\n";
			pointer++;
		}

		counter = 0;
		while (list2[pointer]==list1[ii])
		{
			// std::cout << "list 2: " << list2[pointer] << "\n";
			counter++;
			pointer++;
		}
		// if (counter!=0) std::cout << "simScore: " << list1[ii] << " " << counter << "\n";
		simScore += list1[ii]*counter;
		// if (counter!=0) std::cout << "Updates: " << list1[ii]*counter << " " << simScore << "\n";

	}

	std::cout << "Similarity score: " << simScore << "\n";

	return 0;
}

