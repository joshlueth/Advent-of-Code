#include "Day10.h"
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
  int part1{0}, part2{0};

  std::vector<std::vector<bool>> on {};

  std::vector<std::vector<int>> joltage {};

	while (std::getline(inputFile,inputStr))
	{
    std::size_t bracketL {inputStr.find_first_of('[')};
    std::size_t bracketR {inputStr.find_first_of(']')};
    std::size_t braceL {inputStr.find_first_of('{')};
    std::size_t braceR {inputStr.find_first_of('}')};

    std::string bracket {inputStr.substr(bracketL+1,bracketR-bracketL-1)};
    std::string brace {inputStr.substr(braceL+1,braceR-braceL-1)};
    std::string parens {inputStr.substr(bracketR+2,braceL-bracketR-3)};

    std::vector<bool> on_sub {};
    for (auto ch : bracket) {
      if (ch=='.') {
        on_sub.push_back(false);
      } else {
        on_sub.push_back(true);
      }
    }
    on.push_back(on_sub);

    std::vector<int> jolt_sub {};
    std::string jolt {};
    for (auto ch : brace) {
      if (ch==',') {
        jolt_sub.push_back(std::stoi(jolt));
        jolt = "";
      } else {
        jolt += ch;
      }
    }
    jolt_sub.push_back(std::stoi(jolt));
    joltage.push_back(jolt_sub);
	}

  // for (auto j : joltage) {
  //   for (auto jj : j) {
  //     std::cout << " " << jj;
  //   }
  //   std::cout << "\n";
  // }
  // for (auto o : on) {
  //   for (auto oo : o) {
  //     std::cout << " " << oo;
  //   }
  //   std::cout << "\n";
  // }

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};



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
