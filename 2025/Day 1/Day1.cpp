#include "Day1.h"
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
  std::vector<std::string> inputs {};

	while (std::getline(inputFile,inputStr))
	{
    inputs.push_back(inputStr);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  int counter1 {0};
  int dial {50};
  for (std::size_t i=0; i<inputs.size(); i++) {
    char dir {inputs[i][0]};
    int rot {std::stoi(inputs[i].substr(1))};
    if (dir=='L') {
      rot = -rot;
    } else if (dir=='R') {} else {
      std::cerr << "Must rotate either left (L) or right (R)\n";
      return 0;
    }
    dial = (dial + rot);
    if (dial % 100==0) counter1 += 1;
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  int counter2 {0};
  int dial_new {0};
  dial = 50;
  for (std::size_t i=0; i<inputs.size(); i++) {
    char dir {inputs[i][0]};
    int rot {std::stoi(inputs[i].substr(1))};
    if (dir=='L') {
      rot = -rot;
    } else if (dir=='R') {} else {
      std::cerr << "Must rotate either left (L) or right (R)\n";
      return 0;
    }
    // we have initial value and subsequent value.
    // we want to determine the amount of hundreds crossed over
    // different buckets: -199:-101; -100; -99:-1; 0; 1:99; 100; 101:199; 200; 201:299; 300
    // if we rotate to the right, each number divisible by zero is associated with the bucket to the right
    // if we rotate to the left, each number divisible by zero is associated with the bucket to the left
    dial_new = dial + rot;

    if (rot>0) {
      int h_i = static_cast<int>(std::floor(dial/100.));
      int h_f = static_cast<int>(std::floor(dial_new/100.));
      counter2 += std::abs(h_f-h_i);
      // std::cout << dial << " " << dial_new << " " << h_i << " " << h_f << " " << abs(h_f-h_i) << " " << counter2 << "\n";
    } else {
      int h_i = static_cast<int>(std::ceil(dial/100.));
      int h_f = static_cast<int>(std::ceil(dial_new/100.));
      counter2 += std::abs(h_f-h_i);
      // std::cout << dial << " " << dial_new << " " << h_i << " " << h_f << " " << abs(h_f-h_i) << " " << counter2 << "\n";
    }

    dial = dial_new; 

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
