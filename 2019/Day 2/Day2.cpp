#include "Day2.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <sstream>
#include <cstdlib>

void IntCode(std::vector<int>& Code) {
  bool stop {false};
  for (std::size_t pos {0}; pos<Code.size(); pos+=4) {
    if (stop) break;
    switch (Code[pos]) {
      case 1:
        Code[(std::size_t)Code[pos+3]] = Code[(std::size_t)Code[pos+1]]+Code[(std::size_t)Code[pos+2]];
        break;
      case 2:
        Code[(std::size_t)Code[pos+3]] = Code[(std::size_t)Code[pos+1]]*Code[(std::size_t)Code[pos+2]];
        break;
      case 99:
        stop = true;
        break;
      default:
        std::cerr << "Not a valid code" << std::endl;
        std::exit(1);
    }
  }
  return;
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

  std::vector<int> codesIni{};
	while (std::getline(inputFile,inputStr))
	{
    std::stringstream ss(inputStr);
    std::string token{};
    while (std::getline(ss,token,',')) {
      if (!token.empty()) {
        codesIni.push_back(std::stoi(token));
      }
    }
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::vector<int> codes {codesIni};
  codes[1] = 12;
  codes[2] = 2;
  IntCode(codes);
  std::cout << codes[0] << std::endl;

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::size_t ans{};
  for (std::size_t noun {0}; noun<100; noun +=1) {
    for (std::size_t verb {0}; verb<100; verb +=1) {
      std::vector<int> codesHere {codesIni};
      codesHere[1] = (int)noun;
      codesHere[2] = (int)verb;
      IntCode(codesHere);
      // std::cout << noun << " " << verb << " " << codesHere[0] << std::endl;
      if (codesHere[0]==19690720) {
        // std::cout << noun << " " << verb << std::endl;
        ans = 100*noun + verb;
        goto found;
      }
    }
  }

found:
  std::cout << ans << std::endl;
  // 78, 70

  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
