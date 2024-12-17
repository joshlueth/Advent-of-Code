#include "Day17.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <iterator>
#include <algorithm>
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
  std::vector<int> abc {};
  std::vector<int> instructions {};
  bool onRegisters {true};
	while (std::getline(inputFile,inputStr))
	{
    if (inputStr=="") {
      onRegisters = false;
      continue;
    }
    std::string numbers = "0123456789";
    std::size_t found {inputStr.find_first_of(numbers.c_str())};
    std::size_t found_tracker {0};
    if (found!=0) {
      found_tracker = found-1;
    }
    std::string inputStr_nm {};

    while (found != std::string::npos) {
      if (found==found_tracker+1 || found==0) {
        inputStr_nm += inputStr[found];
      } else {
        if (onRegisters) {
          abc.push_back(std::stoi(inputStr_nm));
        } else {
          instructions.push_back(std::stoi(inputStr_nm));
        }
        inputStr_nm = "";
        inputStr_nm += inputStr[found];
      }
      found_tracker = found;
      found = inputStr.find_first_of(numbers.c_str(), found+1);
    }
    if (onRegisters) {
      abc.push_back(std::stoi(inputStr_nm));
    } else {
      instructions.push_back(std::stoi(inputStr_nm));
    }
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

// literal operand: value itself
// combo operand: 0->0, 1->1, 2->2, 3->3, 4->A, 5->B, 6->C, 7->RESERVED
// instructions: 0: A/2^combo->A, 1: bitwise XOR B and combo->B, 2: combo%8 (lowest 3 bits)->B
// 3: if A!=0, instruction pointer->literal operand; don't increase instruction pointer
// 4: bitwise XOR B and C->output, comma separated, 5: combo%8->output, 6: A/2^combo->B, 7: A/2^combo->C

  std::vector<int>::iterator it {instructions.begin()};
  std::string output {};
  while (it!=instructions.end()) {
    output += instruct(it,abc,instructions);
  }
  if (output.back()==',') output.pop_back();

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "The concatenated answer is: " << output << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

int combo(int operand, std::vector<int> abc) {
  if (operand==4) operand = abc[0];
  else if (operand==5) operand = abc[1];
  else if (operand==6) operand = abc[2];
  else if (operand==7) std::cerr << "Combo Operand 7 is Reserved and SHOULD NOT APPEAR\n";
  return operand;
}

std::string instruct(std::vector<int>::iterator& it, std::vector<int>& abc, std::vector<int>& instructions) {
  std::string output {""};
  // std::cout << "Reading in two numbers\n";
  // std::cout << *it << " " << *(it+1) << "\n";
  int opcode {*it};
  ++it;
  int operand {*it};
  ++it;
  // std::cout << opcode << " " << operand << "\n";
  if (opcode==0) {
    abc[0] = int(abc[0] / (pow(2,combo(operand,abc))));
  } else if (opcode==1) {
    abc[1] = abc[1] ^ operand;
  } else if (opcode==2) {
    abc[1] = combo(operand,abc)%8;
  } else if (opcode==3) {
    if (abc[0]!=0) {
      it = instructions.begin() + operand;
    }
  } else if (opcode==4) {
    abc[1] = abc[1] ^ abc[2];
  } else if (opcode==5) {
    output += std::to_string(combo(operand,abc)%8) + ",";
  } else if (opcode==6) {
    abc[1] = int(abc[0] / (pow(2,combo(operand,abc))));
  } else if (opcode==7) {
    abc[2] = int(abc[0] / (pow(2,combo(operand,abc))));
  }
  return output;
}
