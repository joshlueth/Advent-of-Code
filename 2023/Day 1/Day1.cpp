#include "Day1.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <regex>
#include <unordered_map>

std::unordered_map<std::string, std::string> words = {
        {"one","1"},{"two","2"},{"three","3"},
        {"four","4"},{"five","5"},{"six","6"},
        {"seven","7"},{"eight","8"},{"nine","9"},
        {"eno","1"},{"owt","2"},{"eerht","3"},
        {"ruof","4"},{"evif","5"},{"xis","6"},
        {"neves","7"},{"thgie","8"},{"enin","9"},
        {"1","1"},{"2","2"},{"3","3"},
        {"4","4"},{"5","5"},{"6","6"},{"7","7"},
        {"8","8"},{"9","9"}
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

  std::regex pattern("[1-9]");
  std::regex apattern("([1-9]|one|two|three|four|five|six|seven|eight|nine)");
  std::regex bpattern("([1-9]|eno|owt|eerht|ruof|evif|xis|neves|thgie|enin)");
  std::smatch match1{}, match2{};
  int count {0}, acount {0};
	while (std::getline(inputFile,inputStr))
	{
    std::regex_search(inputStr,match1,pattern);
    std::string first_digit = match1[0].str();
    std::regex_search(inputStr,match1,apattern);
    std::string afirst_digit = match1[0].str();

    std::reverse(inputStr.begin(),inputStr.end());

    std::regex_search(inputStr,match2,pattern);
    std::string second_digit = match2[0].str();
    std::regex_search(inputStr,match2,bpattern);
    std::string asecond_digit = match2[0].str();
    // std::cout << match2[0] << "\n";

    count += std::stoi(first_digit+second_digit);
    acount += std::stoi(words[afirst_digit]+words[asecond_digit]);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::cout << count << "\n";

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::cout << acount << "\n";

  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
