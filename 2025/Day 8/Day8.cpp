#include "Day8.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <map>
#include <cmath>
#include <algorithm>

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
  std::size_t part1{0};
  long long part2{0};

  std::vector<std::tuple<int,int,int>> junction{};
  std::vector<std::set<std::size_t>> circuit{};
  int num {0};
	while (std::getline(inputFile,inputStr))
	{
    std::vector<int> coords{};
    std::size_t pos=0, new_pos{inputStr.find_first_of(',',0)};
    while(new_pos!=std::string::npos) {
      coords.push_back(std::stoi(inputStr.substr(pos,new_pos-pos)));
      pos = new_pos + 1;
      new_pos = inputStr.find_first_of(',',pos);
    }
    coords.push_back(std::stoi(inputStr.substr(pos)));
    junction.push_back(std::make_tuple(coords[0],coords[1],coords[2]));
    circuit.push_back(std::set{static_cast<std::size_t>(num)});
    num++;
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  // create graph with distances
  std::map<int,std::pair<std::size_t,std::size_t>> distance {};
  for (std::size_t j1=0; j1<junction.size();j1++) {
    for (std::size_t j2=j1+1; j2<junction.size(); j2++) {
      std::pair<std::size_t,std::size_t> pair{j1,j2};
      int di = std::pow(std::get<0>(junction[j1])-std::get<0>(junction[j2]),2)+std::pow(std::get<1>(junction[j1])-std::get<1>(junction[j2]),2)+std::pow(std::get<2>(junction[j1])-std::get<2>(junction[j2]),2);
      distance[di] = pair;
    }
  }

  auto t25 {std::chrono::high_resolution_clock::now()};

  // go through the graph
  int connections;
  if (fileName=="input.txt") {
    connections = 1000;
  } else if (fileName=="sample.txt") {
    connections = 10;
  }
  for (const auto& pair : distance) {
    if (connections==0) break;
    std::size_t i1 {pair.second.first}, i2 {pair.second.second};
    connections--;
    if (circuit[i1]==circuit[i2]) continue;
    for (const auto c2 : circuit[i2]) {
      circuit[i1].insert(c2);
    }
    for (const auto c1 : circuit[i1]) {
      circuit[c1] = circuit[i1];
    }
  }

  // make a set of sets:
  std::set<std::set<std::size_t>> circuit_counts {};
  for (const auto& cnum : circuit) {
    circuit_counts.insert(cnum);
  }
  
  // finally, solve part 1 by getting the three largest circuit sizes and multiplying them together
  std::vector<std::size_t> sz{};
  for (const auto& cc : circuit_counts) {
    sz.push_back(cc.size());
  }
  std::sort(sz.begin(),sz.end());
  std::size_t v1 = *(sz.end()-1), v2 = *(sz.end()-2), v3 = *(sz.end()-3);
  part1 = v1 * v2 * v3;

  auto t3 {std::chrono::high_resolution_clock::now()};

  for (const auto& pair : distance) {
    std::size_t i1 {pair.second.first}, i2 {pair.second.second};
    if (circuit[i1]==circuit[i2]) continue;
    for (const auto c2 : circuit[i2]) {
      circuit[i1].insert(c2);
    }
    for (const auto c1 : circuit[i1]) {
      circuit[c1] = circuit[i1];
    }
    if (circuit[i1].size()==circuit.size()) {
      part2 = static_cast<long long>(std::get<0>(junction[i1])) * static_cast<long long>(std::get<0>(junction[i2]));
      break;
    }
  }

  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Dist. ): " << std::chrono::duration_cast<std::chrono::microseconds>(t25-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

  std::cout << "\nPart 1: " << part1 << "\n";
  std::cout << "Part 2: " << part2 << "\n";

	return 0;
}


// okay, so why is this slow?
// obviously it is because everytime we modify the sets, we modify all of the sets to include everything.
// this would be much faster if instead we had a pointer for each junction box to a circuit
// and then when two junction boxes on different circuits were detected, we simply a) combined the sets of junctions, and b) had the pointers of each entry in the higher set point to that circuit
// this actually seems like an interesting use for a class?