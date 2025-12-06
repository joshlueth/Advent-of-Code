#include "Day6.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cmath>

typedef long long ll;

std::vector<std::string> transpose(const std::vector<std::string>& v) {
  std::size_t rows{v.size()}, cols{v[0].size()};
  for (std::size_t sz{0}; sz<rows; sz++) {
    if (v[sz].size()!=cols) {
      std::cout << "Wrong number of columns!";
      std::exit(EXIT_FAILURE);
    }
  }
  // only conditionally transpose!
  // that is, specifically transpose the sub matrices
  // these will have indices (0,sz-1) (first_not_space,second_not_space-first_not_space)

  // first find max (second_not_space-first_not_space)
  std::string ops {v.back()};
  std::size_t pos{ops.find_first_not_of(" ",0)}, new_pos{0};
  std::size_t mx{0};
  do {
    new_pos = ops.find_first_not_of(" ",pos+1);
    mx = std::max(mx,new_pos-pos);
    pos = ops.find_first_not_of(" ",new_pos+1);
  } while (pos!=std::string::npos);

  // default before inversion
  std::vector<std::string> result(mx,std::string(cols-1,'\0'));

  for (std::size_t )

  for (std::size_t opi{0}; opi<result.back().size(); opi+=mx) {
    result.back()[opi] = 
  }

  // return result string!
  return result;
}

ll cephalopod_math(const std::vector<std::string>& inp) {
  std::vector<int> numbers {};
  std::vector<std::size_t> flatsizing;
  for (std::size_t i=0; i<inp.size()-1; i++) {
    std::string nb=inp[i];
    std::size_t pos{nb.find_first_not_of(" ",0)}, new_pos{0};
    flatsizing.push_back(0);
    do {
      new_pos = nb.find(" ",pos);
      numbers.push_back(std::stoi(nb.substr(pos,new_pos-pos)));
      flatsizing.back()++; 
      pos = nb.find_first_not_of(" ",new_pos);
    } while (pos!=std::string::npos); 
    if (flatsizing[i]!=flatsizing[0]) {
      std::cerr << "arrays are different lengths...\n";
    }
  }
  std::size_t flatsz{flatsizing[0]};

  // similarly, get each operator
  std::vector<char> op{};
  std::string ops=inp[inp.size()-1];
  std::size_t pos{ops.find_first_not_of(" ",0)};
  do {
    op.push_back(ops[pos]);
    pos = ops.find_first_not_of(" ",pos+1);
  } while (pos!=std::string::npos);

  // for each operator, go down flat array and compute the new number
  ll val{0};
  for (std::size_t p{0}; p<op.size(); p++) {
    ll prob{};
    if (op[p]=='*') {
      prob = 1;
    } else {
      prob = 0;
    }
    for (std::size_t getn{p}; getn<numbers.size(); getn+=flatsz) {
      int num{numbers[getn]};
      if (op[p]=='*') {
        prob *= num;
      } else {
        prob += num;
      }
    }
    val += prob;
  }
  return val;
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
  ll part1{0}, part2{0};

  std::vector<std::string> inp {};

	while (std::getline(inputFile,inputStr))
	{
    inp.push_back(inputStr);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  part1 = cephalopod_math(inp);
  std::cout << part1 << "\n";

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::vector<std::string> inp_t {transpose(inp)};
  part2 = cephalopod_math(inp_t);

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
