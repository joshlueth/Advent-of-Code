#include "Day3.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>

long long joltage_ini(const std::string bat) {
  char max, max2{'0'};
  for (std::size_t i{0}; i<bat.size(); i++) {
    if (i==0) {
      max = bat[i];
      continue;
    }

    if (i==bat.size()-1) {
      if (bat[i]>max2) {
        max2 = bat[i];
      }
      continue;
    }

    if (bat[i]>max) {
      max = bat[i];
      max2 = '0';
    } else if (bat[i]>max2) {
      max2 = bat[i];
    }
  }
  return std::stoll(std::string(1,max)+max2);
}

void first_max(const std::string str, char& val, int& loc) {
  val = '0';
  int loc_store = loc;
  for (std::size_t i=0; i<str.size(); i++) {
    if (str[i]>val) {
      val = str[i];
      loc = i;
    }
  }
  loc += loc_store;
  return;
}

// include all but last i-1 characters
// ss_start+1 is the number of characters we have to exclude from the beginning
// i-1 is the amount we have to exlude from the end
// 

long long joltage(const std::string bat, const int num) {
  std::string max="";
  char next;
  int ss_start{0};
  for (std::size_t i=num; i>0; i--) {
    // we initially want to find the maximum number in the substr beginning at ss_start
    // we want to exclude the (i-1) characters at the end of the string and the (ss_start) characters at the beginning
    first_max(bat.substr(ss_start,bat.size()-ss_start-i+1),next,ss_start);
    ss_start += 1;
    max += next;
  }
  return std::stoll(max);
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
  long long part1 {0}, part2 {0};

	while (std::getline(inputFile,inputStr))
	{
    part1 += joltage_ini(inputStr); 
    // part1 += joltage(inputStr,2);
    part2 += joltage(inputStr,12);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};


  auto t3 {std::chrono::high_resolution_clock::now()};

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "\nPart 1: " << part1 << "\n";
  std::cout << "Part 2: " << part2 << "\n\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
