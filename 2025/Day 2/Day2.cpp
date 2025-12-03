#include "Day2.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>

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

	while (std::getline(inputFile,inputStr))
	{

	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  // parse input into vector of integer ids
  std::string delimiters {"-,"};
  std::vector<long long> ids_i {};
  std::vector<std::string> ids_s {};
  std::size_t current_pos, prev_pos, end_pos;
  prev_pos = 0;
  end_pos = inputStr.size();
  while(current_pos!=std::string::npos) {
    current_pos = inputStr.find_first_of(delimiters,prev_pos);
    std::size_t string_pos {std::min(current_pos,end_pos)};
    ids_s.push_back(inputStr.substr(prev_pos,string_pos-prev_pos));
    ids_i.push_back(std::stoll(ids_s.back()));
    prev_pos = current_pos+1;
  }
  
  // determine duplicate ids
  // if a number is a duplicate id, then it will a) have an even number of digits, N, and b) by divisible by 11 (2 digits); 0101 (4 digits); 001001 (6 digits) and so on
  // so our task is the following
  // 1) cycle through numbers in each range
  // 2) determine the number of digits. If odd, cycle
  // 3) create the number to check divisibility by
  // 4) check divisibility, add to counter if so

  long long counter1{0};
  for (std::size_t ind {0}; ind<ids_i.size(); ind+=2) {
    long long min {ids_i[ind]}, max {ids_i[ind+1]};
    for (long long num {min}; num<=max; num++) {
      std::size_t sz {std::to_string(num).size()};
      if (sz%2!=0) continue; // cycle if odd
      long long dividend = 1 + static_cast<long long>(std::pow(10.,sz/2)); // create divident to check divisibility
      if (num%dividend!=0) continue; // cycle if not divisible
      if (num/dividend==std::stoll(std::to_string(num).substr(0,sz/2))) { // verify that it satisfies the rule
        counter1 += num;
      }
    }
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  // to do this for an arbitrary number of digits, we now need
  // 1) a list of the factors of the number of digits less than the number itself
  // 2) iterate over this list instead of just over one number!
  long long counter2{0};
  for (std::size_t ind {0}; ind<ids_i.size(); ind+=2) {
    long long min {ids_i[ind]}, max {ids_i[ind+1]};
    for (long long num {min}; num<=max; num++) {
      std::size_t sz {std::to_string(num).size()}; // get size
      for (std::size_t factor {1}; factor<sz; factor++) {
        if (sz%factor!=0) continue; // not an actual factor!
        long long dividend {0};
        std::size_t spacing {factor};
        for (std::size_t power_add {0}; power_add<sz; power_add+=spacing) {
          dividend += static_cast<long long>(std::pow(10.,power_add)); // create dividend to check divisibility
        }
        if (num%dividend!=0) continue;
        if (num/dividend==std::stoll(std::to_string(num).substr(0,spacing))) {
          counter2 += num;
          // exit factor loop, go to num loop
          goto finish_num;
        }
      }
    finish_num:
      if (false) return 0;
    }
  }

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "\n";
  std::cout << "Part 1: " << counter1 << "\n";
  std::cout << "Part 2: " << counter2 << "\n\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

	return 0;
}
