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

bool dist_sort(std::pair<long long,std::pair<std::size_t,std::size_t>>& a, std::pair<long long,std::pair<std::size_t,std::size_t>>& b) {
  return a.first<b.first;
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
  std::size_t part1{0};
  long long part2{0};

  std::vector<long long> jx{}, jy{}, jz{};
  std::vector<std::set<std::size_t>> circuit{};
  std::vector<std::set<std::size_t>*> c_ptr{};
  int num {0};
	while (std::getline(inputFile,inputStr))
	{
    std::vector<long long> coords{};
    std::size_t pos=0, new_pos{inputStr.find_first_of(',',0)};
    while(new_pos!=std::string::npos) {
      coords.push_back(std::stoi(inputStr.substr(pos,new_pos-pos)));
      pos = new_pos + 1;
      new_pos = inputStr.find_first_of(',',pos);
    }
    coords.push_back(std::stoi(inputStr.substr(pos)));
    jx.push_back(coords[0]);
    jy.push_back(coords[1]);
    jz.push_back(coords[2]);
    circuit.push_back(std::set{static_cast<std::size_t>(num)});
    num++;
	}
  for (std::size_t c=0; c<circuit.size(); c++) {
    c_ptr.push_back(&circuit[c]);
  }

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  // create graph with distances
  std::vector<std::pair<long long,std::pair<std::size_t,std::size_t>>> distance(jx.size()*(jx.size()-1)/2);
  std::size_t counter{0};
  for (std::size_t j1=0; j1<jx.size();j1++) {
    for (std::size_t j2=j1+1; j2<jx.size(); j2++) {
      std::pair<std::size_t,std::size_t> pair{j1,j2};
      long long dx = jx[j1]-jx[j2], dy = jy[j1]-jy[j2], dz = jz[j1]-jz[j2];
      long long di2 = dx*dx+dy*dy+dz*dz;
      distance[counter++] = std::make_pair(di2,pair);
    }
  }

  std::sort(distance.begin(),distance.end(),dist_sort);

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
    if (*c_ptr[i1]==*c_ptr[i2]) continue;
    for (const auto c2 : *c_ptr[i2]) {
      (*c_ptr[i1]).insert(c2);
    }
    for (const auto c1 : *c_ptr[i1]) {
      c_ptr[c1] = c_ptr[i1];
    }
  }

  // make a set of sets:
  std::set<std::set<std::size_t>> circuit_counts {};
  for (const auto& cnum : c_ptr) {
    circuit_counts.insert(*cnum);
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
    if (*c_ptr[i1]==*c_ptr[i2]) continue;
    for (const auto c2 : *c_ptr[i2]) {
      (*c_ptr[i1]).insert(c2);
    }
    for (const auto c1 : *c_ptr[i1]) {
      c_ptr[c1] = c_ptr[i1];
    }
    if ((*c_ptr[i1]).size()==c_ptr.size()) {
      part2 = static_cast<long long>(jx[i1]) * static_cast<long long>(jx[i2]);
      break;
    }
  }

  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Dist. ): " << std::chrono::duration_cast<std::chrono::microseconds>(t25-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (P1-Dis): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t25).count() << "\n";
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

// TODO:
// implement union-find
// avoid using a set: instead pre-allocate a vector and then sort
// even better, figure out what kd-tree does