#include "Day5.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>

int main()
{
	std::string fileName {"input.txt"}; // note that using sample.txt should give an answer of 18

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::string inputStr {};
	std::unordered_multimap<int,int> rules{};
	std::vector<std::vector<int>> pages{};
	bool inputType {true};
	while (std::getline(inputFile,inputStr))
	{
		if (inputType) {
			if (inputStr=="") {
				inputType = false;
				continue;
			}
// put rules in struct
			// get (value,key)
			std::stringstream ss {inputStr};
			char delimiter{'|'};
			std::string num {};
			std::vector<int> vkPair{};
			while (getline(ss,num,delimiter)) {
				vkPair.push_back(std::stoi(num));
			}
			rules.insert(std::pair<int,int> (vkPair[1],vkPair[0]));
			continue;
		}
// put pages in struct
		std::stringstream ss {inputStr};
		char delimiter{','};
		std::string num {};
		std::vector<int> oneLine {};
		while (getline(ss,num,delimiter)) {
			oneLine.push_back(std::stoi(num));
		}
		pages.push_back(oneLine);
	}

	inputFile.close();	

	int middleNum {0}, middleNumAdjust{0};
	// go through each list
	for (auto lst : pages) {
		bool correct {false};
		std::vector<int> sorted {};
		// for each entry in the list
		for (auto pg : lst) {
			// go through each entry in the sorted list and identify where the index should be inserted
			std::size_t index {0};
			auto range {rules.equal_range(pg)};
			for (auto pgS : sorted) {
				for (auto ii{range.first}; ii!=range.second; ++ii) {
					if (ii->second==pgS) {
						index += 1;
						break;
					}
				}
			}
			sorted.insert(sorted.begin()+index,pg);
		}
		if (std::equal(lst.begin(),lst.end(),sorted.begin())) correct = true;
		// get index and increment middle number
		std::size_t index {(lst.size())/2};
		if (correct) {
			middleNum += lst[index];
		}
		else {
			middleNumAdjust += sorted[index];
		}
	}

	std::cout << "Sum of Middle Numbers: " << middleNum << "\n";
	std::cout << "Sum of Middle Numbers (Adjusted): " << middleNumAdjust << "\n";

	return 0;
}

