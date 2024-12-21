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
  std::vector<long long> abc {};
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
          abc.push_back(std::stoll(inputStr_nm));
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
  
  std::vector<long long> abc_cp {abc};

	auto t2 {std::chrono::high_resolution_clock::now()};

// literal operand: value itself
// combo operand: 0->0, 1->1, 2->2, 3->3, 4->A, 5->B, 6->C, 7->RESERVED
// instructions: 0: A/2^combo->A, 1: bitwise XOR B and combo->B, 2: combo%8 (lowest 3 bits)->B
// 3: if A!=0, instruction pointer->literal operand; don't increase instruction pointer
// 4: bitwise XOR B and C->output, comma separated, 5: combo%8->output, 6: A/2^combo->B, 7: A/2^combo->C

  std::vector<int>::iterator it {instructions.begin()};
  std::string output {};
  while (it!=instructions.end()) {
    output += instruct(it,abc,instructions,true);
  }
  if (output.back()==',') output.pop_back();

  auto t3 {std::chrono::high_resolution_clock::now()};

// instructions remains constant
// want to find the new value for register A such that the output is equal to the instructions!

// SO, we need to begin with some notes to try and figure out what is going on.
// there are 16 instructions that need to be printed, and we know exactly what they are
// splitting them into opcode and operand pairs, we have
// 2,4 1,5 7,5 1,6 0,3 4,1 5,5 3,0
// Immediately we notice that the last two sequences are print(B%8) and reset to the beginning unless A==0
// So the preceding 6 instructions must alter B to the point that the print outputs the correct answer
// They are
// A%8 -> B; B^(5=101) -> B; A/(2**B) -> C; B^(6=110) -> B; A/8 -> A; B^C -> B;
// every cycle we get a new B value, and then only perform bitwise operations on it.
// Therefore B has to be the actual value of the instructions at each point
// (B=fst^101)^110:
// if t=1, we get 0,0; if t=0, we get 1,1; = -t
// if s=1, we get 1,0; if s=0 ,we get 0,1; = -s
// if f=1, we get 0,1; if f=0 we get 1,0; = f
// so we clearly just flip the last two bits of B such that there is a bijective mapping 1-2, 0-3, 4-7, 5-6
// now since the inverse of XOR is XOR itself, we can get the necessary mod 8 of A by taking
// ((B^C)^6)^5 must be equal to A%8
// Notice that since A is continually divided by 8, we can simply construct A in a base eight fashion by:
// required value of B -> A mod 8 -> multiplied by 8
// now the ultimate required value of B after the mod 8 is given by ((O^C)^6)^5, where O is the outputted value
// We only care about the last three digits of C, which are given by some A!!
// SO clearly this is more intuitive in a bitwise sense. Let b refer to a bit. For A=bn b(n-1) b(n-2) ... b3 b2 b1 b0 (n is an arbitrary power)
// B = b2 b1 b0
// B = (b2^1) (b1^0) (b0^1)
// C = b(y+2) b(y+1) b(y), y is given by B
// B = b2 -b1 -b0
// A = bn b(n-1) b(n-2) ... b5 b4 b3
// B(=Output) = b2^c2 -b1^c1 -b0^c0

// so we start with A = 0, so C = 0

  // std::vector<int>::reverse_iterator itb {instructions.rbegin()};
  // long long int A{0}, B{0};
  // while (itb!=instructions.rend()) {
  //   B = *itb;
  // use an iterative method to find C
  // A%8 -> B; B^(5=101) -> B; A/(2**B) -> C; B^(6=110) -> B; A/8 -> A; B^C -> B;

  // We must have, given output B and initial value A
  // B = (((A%8)^5)^6)^C
  // C = A/(2**((A%8)^5)
  // A = A/8

  // so given A*, we have
  // A = A*/8
  // B = (((A*%8)^5)^6) ^ (A*/(2**((A*%8)^5))

  // for our problem, we are given B. Then it reduces to finding a 3 digit (in binary, so in base, from 0-7) number N such that
  // given value A* = A(8) + N
  // and output value B
  // find B = (((A*%8)^5)^6) ^ (A*/(2**((A*%8)^5)) = ((N^5)^6) ^ (A*/(2**(N^5))
  // since bitwise XOR is associative
  // (0^0)^0 = 0 : assoc
  // (0^0)^1 = 1 : assoc w/ 3
  // (0^1)^0 = 1 : assoc w/ 2
  // (0^1)^1 = 0 : assoc w/ 7
  // (1^0)^0 = 1 : comm w/ 3
  // (1^0)^1 = 0 : comm w/ 4
  // (1^1)^0 = 0 : assoc w/ 4
  // (1^1)^1 = 1 : assoc
  // we have B = (N^5) ^ 6 ^ (A*/(2**(N^5)), M=N^5
  // to B^6 = M ^ (A*/(2**(M))
  // to B^6 ^ (A*/(2**(M)) = M 
  // to N = M^5

  //   int M {0};
  //   while ( (B^6) != M^static_cast<long long>(A/(pow(2,M)))) {
  //     M += 1;
  //     if (M==8) {
  //       std::cout << "M should never be 8\n";
  //       std::cout << A << " " << (B^6) << "\n";
  //     }
  //   }
  //   int N {M^5};
  //   std::cout << "N " << N << "\n";
  //   A = 8 * A + N;
  //   ++itb;
  // }

  // frankly, if I want a fully generalizable method, I need to figure out inverses for each operation.
  // inverse of 0, adv, is shifting A bitwise to the left by combo operand, introducing combo operand unknowns
  // inverse of 1, bxl, is bitwise XOR of B and literal operand
  // inverse of 2, bst, B needs to be exactly three bits equal to the lowest three bits of the combo operand. Set B to unknown
  // inverse of 3, jnz, set A to 0.
    // this one we have to deal with very very carefully.
    // so in order to pass 
  // inverse of 4, bxc, is bitwise XOR with the current value of C
  // inverse of 5, out, sets the last 3 bits of the combo operand to the previous output value
  // inverse of 6, bdv, is shifting B bitwise to the left by combo operand, introducing combo operand unknowns
  // inverse of 7, cdv, is shifting B bitwise to the left by combo operand, introducing combo operand unknowns

  // to keep this generic and allow for the possibility of unknowns, the "best" method, or, more practically, the method that I plan to use is with strings
  // we will keep the representations in binary
  // for unknown bits, we will have a list of unused letters paired with their opposite at the other end of the alphabet:
  // az, by, cx, dw, ev, fu, gt, hs, ir, jq, kp, lo, mn
  // the first represents the bit, the second represents the flip of the bit. '+' will represent an unknown amount of leading digits

// Okay I give up on making something fully generalizable for a problem that doesn't matter. Instead I will just add powers of 8 to A

  long long A {1};




  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "The concatenated answer is: " << output << "\n";
  std::cout << "The new value for register A is: " << A << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

int combo(int operand, std::vector<long long> abc) {
  if (operand==4) operand = abc[0];
  else if (operand==5) operand = abc[1];
  else if (operand==6) operand = abc[2];
  else if (operand==7) std::cerr << "Combo Operand 7 is Reserved and SHOULD NOT APPEAR\n";
  return operand;
}

std::string instruct(std::vector<int>::iterator& it, std::vector<long long>& abc, std::vector<int>& instructions, bool comma) {
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
    output += std::to_string(combo(operand,abc)%8);
    if (comma) output += ",";
  } else if (opcode==6) {
    abc[1] = int(abc[0] / (pow(2,combo(operand,abc))));
  } else if (opcode==7) {
    abc[2] = int(abc[0] / (pow(2,combo(operand,abc))));
  }
  return output;
}

// pass in A, abc_cp, instructions, 
long long recursiveInstruct(long long& A, std::vector<long long>& abc_cp, std::vector<int> instructions) {
  long long B {abc_cp[1]}, C {abc_cp[2]};
  std::vector<long long> abc {};

  for (std::size_t nout {0}; nout < instructions.size(); ++nout) {
    bool matches_thus_far {true};
    std::vector<int>::iterator it {instructions.begin()};
    std::string output {};
    abc = {A, B, C};
std::cout << "A " << dec2octal(A) << "\n";
    while (it!=instructions.end()) {
      output += instruct(it,abc,instructions,false);
    }
    for (std::size_t ch{0}; ch<output.size(); ch++) {
std::cout << "comparing output and instructions ch: " << output[ch] << " " << instructions[ch] << "\n";
      if (output[ch] - '0'==instructions[ch]) {
std::cout << "success\n";
        continue;
      } else {
        if (A!=7) {
          A += 1;
          matches_thus_far = false;
        } else {
          A /= 8;
          return A;
        }
      }
      
    }
    if (!matches_thus_far) {
      continue;
    }
    A = 8*A;
    A = recursiveInstruct(A, abc_cp, instructions);
  }
}

std::string dec2octal (long long A) {
  std::string output {};
  while (A!=0) {
    long long coeff = {A%8};
    output = std::to_string(coeff) + output;
    A = A/8;
  }
  return output;
}
