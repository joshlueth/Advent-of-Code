#include "Day5.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
#include <algorithm>

typedef long long ll;

// void modify_ranges(std::vector<std::pair<ll,ll>>& ranges,ll min,ll max) {
  
// }

bool sort_by_min(std::pair<ll,ll> a, std::pair<ll,ll> b) {
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

  // what to do:
  // first create data structure
  // have minimum and maximum for the range
  // then for every new range

	std::string inputStr {};
  int part1{0};
  ll part2{0};
  std::vector<std::pair<ll,ll>> range {};
  std::vector<ll> food {};

  bool ingredients {false};
	while (std::getline(inputFile,inputStr))
	{
    if (inputStr=="") {
      ingredients = true;
      continue;
    }
    if (ingredients) {
      food.push_back(std::stoll(inputStr));
    } else {
      std::size_t dash {inputStr.find('-')};
      range.push_back(std::make_pair(std::stoll(inputStr.substr(0,dash)),std::stoll(inputStr.substr(dash+1))));
    }
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  sort(range.begin(),range.end(),sort_by_min);
  std::size_t i=0;
  // a-b
  // c-d
  // orderings: we know a<=b, a<=c, c<=d (if a=b, then b<=d)
  // therefore we can have a<b<c<d, a<c<b<d, or a<c<d<b
  while (i<range.size()-1) {
    // take i and check if the current range can be merged with the range ahead
    std::pair<ll,ll> cur=range[i], next=range[i+1];
    if (next.second<=cur.second) {
      range.erase(range.begin()+static_cast<ll>(i)+1);
    } else if (next.first<=cur.second+1) {
      range[i].second = next.second;
      range.erase(range.begin()+static_cast<ll>(i)+1);
    } else {
      i++;
    }
  }
  
  sort(food.begin(),food.end());

  auto cr = range.begin();
  for (auto ing : food) {
    while (ing>(*cr).second) {
      if (cr==std::prev(range.end())) break;
      cr++;
    }
    if (ing<(*cr).first) continue;
    if (ing<=(*cr).second) {
      part1++; continue;
    }
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  for (auto rg : range) {
    part2 += 1+rg.second-rg.first;
  }

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
