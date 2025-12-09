#include "Day9.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

int main(int argc, char* argv[])
{

// argc is the number of command line arguments
// argv is the value of command line arguments
// currently accept argc==1 (no additional) or argc==2 (1 additional, filename w/ no .txt)

  std::string fileName {"sample.txt"}, extensionDefault {".txt"};
  if (argc<=1) {
  } else if (argc==2) {
    fileName = argv[1];
    if (fileName.find('.')==std::string::npos) {
      fileName += extensionDefault;
    } 
  } else {
    std::cerr << "Received " << argc-1 << " additional command line arguments" << "\n";
    std::cerr << "Only accepts up to 1 additional argument:" << "\n";
    std::cerr << "  input filename (default \"" << fileName << "\", extension defaults to \"" << extensionDefault << "\")" << "\n";
  }

	auto t1 {std::chrono::high_resolution_clock::now()};

	std::ifstream inputFile {fileName};
	if (!inputFile)
	{
		std::cerr << "Input file \"" << fileName << "\" could not be opened\n";
		return 1;
	}

	std::string inputStr {};
  long long part1{0}, part2{0};

  std::vector<std::pair<int,int>> tiles{};

	while (std::getline(inputFile,inputStr))
	{
    std::size_t pos {inputStr.find_first_of(',')};
    tiles.push_back(std::make_pair(std::stoi(inputStr.substr(0,pos)),std::stoi(inputStr.substr(pos+1))));
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  for (const auto& tile1 : tiles) {
    for (const auto& tile2 : tiles) {
      if (tile1==tile2) continue;
      part1 = std::max(static_cast<long long>(std::abs(tile1.first-tile2.first)+1)*static_cast<long long>(std::abs(tile1.second-tile2.second)+1),part1);
    }
  } 

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

  std::cout << "\nPart 1: " << part1 << "\n";
  std::cout << "Part 2: " << part2 << "\n";

	return 0;
}
