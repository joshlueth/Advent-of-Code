#include "Day19.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <unordered_map>

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

  std::getline(inputFile,inputStr);
  std::stringstream ss {inputStr};
  std::vector<std::string> towels {};
  std::string single_towel {};
  while (getline(ss, single_towel, ',')) {
    while (single_towel[0]==' ') single_towel.erase(0,1);
    towels.push_back(single_towel);
  }

  std::getline(inputFile,inputStr);
  if (inputStr!="") {
    std::cout << "Wrong input format" << "\n";
    return 1;
  }
  std::vector<std::string> patterns {};
	while (std::getline(inputFile,inputStr))
	{
    patterns.push_back(inputStr);
	}

	inputFile.close();

  // std::cout << "towels\n";
  // for (auto tw : towels) {
  //   std::cout << tw << "\n";
  // }
  // std::cout << "patterns\n";
  // for (auto pt : patterns) {
  //   std::cout << pt << "\n";
  // }

	auto t2 {std::chrono::high_resolution_clock::now()};

// it seems possible to preprocess the patterns into a data format such that I can read char by char and do a DFS
// but whatever let's just go through each string

  long long numPossible {0}, counter {0};
  std::unordered_map<std::string,long long> cache;
  for (auto pt : patterns) {
    long long num {0};
    num += patternCheck(pt,towels,cache);
    numPossible += num;
    if (num>0) counter += 1;
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::cout << "Number of Designs that can be made: " << counter << "\n";
  std::cout << "Number of total different combinations that give a design: " << numPossible << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving:          " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";


	return 0;
}

long long patternCheck(std::string pt, std::vector<std::string>& towels, std::unordered_map<std::string,long long>& cache) {
  if (cache.find(pt)!=cache.end()) {
    return cache[pt];
  } else cache[pt] = 0;
  for (auto t : towels) {
    std::size_t sz = t.size();
    if (pt.size()>=sz) {
      if (t == pt.substr(0,sz)) {
        if (pt.size()==sz) {
          cache[pt] += 1;
        }
        cache[pt] += patternCheck(pt.substr(sz), towels, cache);
      }
    }
  }
  return cache[pt];
}