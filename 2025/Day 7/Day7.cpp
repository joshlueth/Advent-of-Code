#include "Day7.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <unordered_map>

typedef long long ll;

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
  int part1{0};
  ll part2{0};

  std::vector<std::string> map {};
	while (std::getline(inputFile,inputStr))
	{
    map.push_back(inputStr);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  // find starting point
  std::size_t source {map[0].find_first_of('S')};
  // store in data structure with the location of the beam and the tachyonic possibility for getting there
  std::unordered_map<std::size_t,ll> current{{source,1}};
  for (std::size_t level=1; level<map.size(); level++) {
    std::unordered_map<std::size_t,ll> next{};
    for (auto beam : current) {
      // std::cout << "beam: " << beam << "\n";
      std::size_t map_lsz = map[level].size();
      if (map[level][beam.first]=='^') {
        // potentially unecessary checks for ensuring the split beams do not leave the manifold
        // add the tachyonic paths for getting there
        if (beam.first>0) next[beam.first-1] += beam.second;
        if (beam.first+1<map_lsz) next[beam.first+1] += beam.second;
        // occurs when a beam is split (part 1)
        part1++;
      } else {
        // otherwise the beam goes straight down
        // adding tachyonic paths for getting there
        next[beam.first] += beam.second;
      }
    }
    // std::cout << "current\n";
    // for (auto bm : current) {
    //   std::cout << bm.first << " " << bm.second << "\n";
    // }
    // std::cout << "next\n";
    // for (auto bm : next) {
    //   std::cout << bm.first << " " << bm.second << "\n";
    // }
    
    // advance to next level down
    current = next;
  }

  // for part two, sum the number of tachyonic possibilities
  for (auto beam : current) {
    part2 += beam.second;
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:                  " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in Input File:       " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Parts 1+2): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";

  std::cout << "\nPart 1: " << part1 << "\n";
  std::cout << "Part 2: " << part2 << "\n";

	return 0;
}
