#include "Day1.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

int getFuel(int mass) {
  return (mass/3-2);
}

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
  std::vector<int> masses {};

	while (std::getline(inputFile,inputStr))
	{
    masses.push_back(std::stoi(inputStr));
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  int totalFuel {0};
  for (std::size_t i{0}; i<masses.size(); ++i)
    totalFuel += getFuel(masses[i]);

  std::cout << totalFuel << std::endl;

  auto t3 {std::chrono::high_resolution_clock::now()};

  int totalFuel2 {0}, currentMass {};
  for (std::size_t i{0}; i<masses.size(); ++i) {
    currentMass = masses[i];
    int currentFuel {};
    do {
      currentFuel = getFuel(currentMass);
      totalFuel2 += std::max(currentFuel,0);
      currentMass = currentFuel;
    } while (currentMass > 0);
  }
  std::cout << totalFuel2 << std::endl;

  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
