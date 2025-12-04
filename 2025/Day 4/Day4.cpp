#include "Day4.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm>

// 0, ..., rows-1, rows, rows+1, ..., size-1
// begin                              end
// either move 0 from begin or move right rows-1
// either move 0 from end or move left (size-1)-(rows+1) = size-rows-2
int neighboring(const std::vector<std::string>& floor, int num_neighboring) {
  int include_center {num_neighboring+1};
  int counter {0};
  for (std::size_t rows=0; rows<floor.size(); rows++) {
    int start = std::max(0,static_cast<int>(rows)-1);
    int end = std::max(0,static_cast<int>(floor.size())-static_cast<int>(rows)-2);
    // std::cout << rows << " " << start << " " << end << " " << floor.size() << "\n";
    std::vector<std::string> sub_floor(floor.begin()+start,floor.end()-end);
    // for (std::size_t j=0; j<sub_floor.size(); j++) {
    //   std::cout << sub_floor[j] << "\n";
    // }
    // std::cout << "checking substrings\n";
    for (std::size_t cols=0; cols<floor[rows].size(); cols++) {
      if (floor[rows][cols]!='@') continue;
      // std::cout << rows << " " << cols << "\n";
      int st = std::max(0,static_cast<int>(cols)-1);
      int en = std::min(static_cast<int>(sub_floor[0].size())-1,static_cast<int>(cols)+1);
      // std::cout << st << " " << en << "\n";
      std::string countAt="";
      for (std::size_t i=0; i<sub_floor.size(); i++) {
        countAt += sub_floor[i].substr(st,en-st+1);
      }
      int neigh = std::count(countAt.begin(),countAt.end(),'@');
      // std::cout << countAt << " " << neigh << " " << counter+1 << "\n";
      if (neigh<include_center) {
        counter++;
      }
      // std::cout << countAt << "\n";
    }
  }
  return counter;
}

int neighboring_removal(std::vector<std::string>& floor, int num_neighboring) {
  int include_center {num_neighboring+1};
  int counter {0};
  for (std::size_t rows=0; rows<floor.size(); rows++) {
    int start = std::max(0,static_cast<int>(rows)-1);
    int end = std::max(0,static_cast<int>(floor.size())-static_cast<int>(rows)-2);
    // std::cout << rows << " " << start << " " << end << " " << floor.size() << "\n";
    std::vector<std::string> sub_floor(floor.begin()+start,floor.end()-end);
    // for (std::size_t j=0; j<sub_floor.size(); j++) {
    //   std::cout << sub_floor[j] << "\n";
    // }
    // std::cout << "checking substrings\n";
    for (std::size_t cols=0; cols<floor[rows].size(); cols++) {
      if (floor[rows][cols]!='@') continue;
      // std::cout << rows << " " << cols << "\n";
      int st = std::max(0,static_cast<int>(cols)-1);
      int en = std::min(static_cast<int>(sub_floor[0].size())-1,static_cast<int>(cols)+1);
      // std::cout << st << " " << en << "\n";
      std::string countAt="";
      for (std::size_t i=0; i<sub_floor.size(); i++) {
        countAt += sub_floor[i].substr(st,en-st+1);
      }
      int neigh = std::count(countAt.begin(),countAt.end(),'@');
      // std::cout << countAt << " " << neigh << " " << counter+1 << "\n";
      if (neigh<include_center) {
        floor[rows][cols] = '.';
        counter++;
      }
      // std::cout << countAt << "\n";
    }
  }
  return counter;
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
  int part1{0}, part2{0};
  std::vector<std::string> floor {};

	while (std::getline(inputFile,inputStr))
	{
    floor.push_back(inputStr);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  part1 += neighboring(floor,4);

  auto t3 {std::chrono::high_resolution_clock::now()};

  int count;
  do {
    count = neighboring_removal(floor,4);
    part2 += count; 
  } while (count>0);

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
