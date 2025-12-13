#include "Day12.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>

class pattern_t {
  public:
  int count, x, y;
  std::vector<std::vector<char>> pattern;

  pattern_t(int xsz, int ysz, std::vector<std::vector<char>> in_pattern)
    : count(count_hashes(in_pattern)), x(xsz), y(ysz), pattern(in_pattern)
  {}

  static int count_hashes(std::vector<std::vector<char>> pt) {
    int count=0;
    for (auto vch : pt) {
      for (char ch : vch) {
        if (ch=='#') count++;
      }
    }
    return count;
  }

  void print() {
    std::cout << "x: " << x << " y: " << y << " count: " << count << "\n";
    for (auto r : pattern) {
      for (char c : r) {
        std::cout << c;
      }
      std::cout << "\n";
    }
  }
};

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

  std::vector<pattern_t> pieces{};
  std::vector<std::pair<std::pair<int,int>,std::vector<int>>> problems {};
  std::vector<std::vector<char>> piece(3,std::vector<char>(3));
  std::size_t rw=0;
  bool b_pieces=true;
	while (std::getline(inputFile,inputStr))
	{
    if (b_pieces) {
      // std::cout << inputStr.size() << "\n";
      if (inputStr.size()==0) {
        pieces.push_back(pattern_t(3,3,piece));
        continue;
      }
      if (inputStr.find_first_of(':')!=std::string::npos) {
        // std::cout << *(inputStr.end()-1) << "\n";
        if (*(inputStr.end()-1)==':') {
          rw=0; 
          continue;
        } else {
          b_pieces = false;
        }
      }
      if (b_pieces) piece[rw++] = std::vector<char>(inputStr.begin(),inputStr.end());
    }
    // std::cout << b_pieces << "\n";
    if (!b_pieces) {
      // std::cout << "problems\n";
      std::size_t locx=inputStr.find_first_of('x'), locsc=inputStr.find_first_of(':');
      std::pair<int,int> sz {std::stoi(inputStr.substr(0,locx)),std::stoi(inputStr.substr(locx+1,locsc-locx-1))};
      std::istringstream probs (inputStr.substr(locsc+1));
      int x;
      std::vector<int> xp {};
      while (probs>>x) xp.push_back(x);
      problems.push_back(std::make_pair(sz,xp));
    }
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  for (auto pr : problems) {
    int trivial=0; int total=0;
    for (std::size_t pt=0; pt<pr.second.size(); pt++) {
      trivial+=pr.second[pt];
      total+=pieces[pt].count*pr.second[pt];
    }
    int space = pr.first.first/3 * pr.first.second/3;
    int markers = pr.first.first*pr.first.second;
    // std::cout << trivial << " " << space << " " << total << " " << markers << "\n";
    if (space>=trivial) part1++;
    else if (markers<total) continue;
    else {
      std::cout << "NP-hard case\n";
      std::cout << trivial << " " << space << " " << total << " " << markers << "\n";
    }
  }

  // std::cout << "parsed\n";
  // for (auto pt : pieces) {
  //   pt.print();
  // }
  // for (auto p : problems) {
  //   std::cout << p.first.first << " " << p.first.second;
  //   for (auto pp : p.second) {
  //     std::cout << " " << pp;
  //   }
  //   std::cout << "\n";
  // }


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
