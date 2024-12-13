#include "Day13.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
#include <cstdlib>
#include <regex>

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


// the fastest way to read in this file is likely to use a regex to identify the numbers and place them in the appropriate data structures
	std::string inputStr {};
  std::vector<std::pair<int,int>> A {};
  std::vector<std::pair<int,int>> B {};
  std::vector<std::pair<int,int>> prizes {};

	while (std::getline(inputFile,inputStr))
	{
    if (inputStr=="") continue;
    // use regex to get the two numbers
    std::regex numbers {"[0-9]+"};
    std::smatch numbersMatch {};
    int n1 {-1}, n2 {-1};
    while(std::regex_search(inputStr, numbersMatch, numbers)){
      if (numbersMatch[0]=="") break;
      if (n1==-1) n1 = std::stoi(numbersMatch[0]);
      else n2 = std::stoi(numbersMatch[0]);
      inputStr = numbersMatch.suffix();
    }
    std::pair<int,int> intoVec {std::make_pair(n1,n2)};
    // depending on the sizes of the vectors, put in the appropriate vector
    if (A.size()>B.size()) {
      B.push_back(intoVec);
    } else if (B.size()>prizes.size()) {
      prizes.push_back(intoVec);
    } else {
      A.push_back(intoVec);
    }
	}

  if ((prizes.size() != A.size()) && (A.size() != B.size())) {
    std::cerr << "The number of prizes, A buttons, and B buttons are not the same\n";
    return 1;
  }

	inputFile.close();

// Coin counter's problem in 2 dimension
// if a solution exists, want to give the minimum solution subject to a weighting function
// find x and y such that xa1+yb1=p1 and xa2+by2+p2 minimizing 3x+y

	auto t2 {std::chrono::high_resolution_clock::now()};

  long long tokens{0}, tokens_offset{0};
  long long offset {10000000000000};
  for (std::size_t pzs {0}; pzs<prizes.size(); ++pzs) {
    tokens += ccProblem(A[pzs],B[pzs],prizes[pzs],0);
    tokens_offset += ccProblem(A[pzs],B[pzs],prizes[pzs],offset);
  }
  
  auto t3 {std::chrono::high_resolution_clock::now()};

  std::cout << "Total number of tokens to win all possible prizes: " << tokens << "\n";
  std::cout << "Total number of tokens to win all possible prizes (with offset): " << tokens_offset << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving: " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";


	return 0;
}

long long ccProblem(const std::pair<int,int>& A, const std::pair<int,int>& B, const std::pair<int,int>& prizes, long long offset) {
  std::pair<long long,long long> solution {std::make_pair(prizes.first+offset,prizes.second+offset)};
  int det {A.first*B.second - A.second*B.first};
  if (det!=0) {
    solution = std::make_pair(solution.first*B.second-solution.second*B.first, -solution.first*A.second+solution.second*A.first);
    if (isDivisible(solution.first,det) && isDivisible(solution.second,det)) {
      if (divSign(solution.first,det) && divSign(solution.second,det)) {
        solution.first /= det;
        solution.second /= det;
        return 3*solution.first+solution.second;
      } else return 0;
    } else return 0;
  }

  std::cout << "Singular Matrix, have to write more code :(" << "\n";
  return 0;

}

bool divSign(long long n1, int n2) {
  if (n2==0) {
    std::cerr << "Tried to divide by a negative number\n";
    exit(1);
  }
  if ( (n1>=0 && n2>0) || (n1<=0 && n2<0) ) return true;
  return false;
}

bool isDivisible(long long n1, int n2) {
  if (std::abs(n1)>std::abs(n2)) {
    if(n1%n2==0) return true;
  }
  return false;
}

// returns gcd, bezout coefficients
// int extendedEuclideanAlgorithm() {

// }

//NEVER MIND IT'S LITERALLY JUST LINEAR EQUATIONS SUBJECT TO A CONSTRAINT I WANT TO DIE

// [AX BX][a]=[px]
// [AY BY][b] [py]

// inverse of a 2x2 matrix gives
// [a]=      1/     *[ BY -BX][px]
// [b] (AX*BY-AY*BX) [-AY  AX][py]

// det = (AX*BY-AY*BX)
// if determinant!=0, we have rhs vector and det
// check that rhs%det==0 and that the signs are the same -> unique solution
// if determinant==0, rows are not linearly independent
// find offset factor with max(AX%AY,AY%AX), check that px*factor==py || py*factor==px
// if not, not solution
// if so, reduces to one equation
// use extended Euclidean algorithm to get gcd(AX,BX) and divide the first equation by it
// if px%gcd(AX,BX)!=0, no solution
// multiply coefficients by px/gcd(AX,BX) to get an integer solution
// want a natural number solution that minimizes the constraint 3a+b
// find the ratio AX/BX : greater than 3, maximize a; less than 3, maximize b
// we know from the extended euclidean algorithm that one of our value of a/b is negative
// we also know that the solution is still valid if we take a+-BX, b-+AX
// do so until both numbers are positive -> valid solution
// go into minimizer: find which one of a/b is smaller
// if it is the incorrect one, continue doing a+-BX, b-+AX until we have the correct solution





