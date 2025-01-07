#include "Day23.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <utility>
#include <vector>
#include <algorithm>
#include <set>

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

// read in input file: add nodes, edges
  std::set<std::pair<int,int>> edges {};
  std::set<int> nodes {};
	while (std::getline(inputFile,inputStr))
	{
    std::stringstream ss {inputStr};
    std::string num1 {}, num2;
    int n1 {}, n2 {};
    getline(ss,num1,'-');
    getline(ss,num2,'-');
    n1 = computerHash(num1);
    n2 = computerHash(num2);
    edges.insert(std::make_pair(std::min(n1,n2),std::max(n1,n2)));
    nodes.insert(n1);
    nodes.insert(n2);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

// for (auto st : nodes) {
//   std::cout << st << "\n";
// }
// for (auto ed : edges) {
//   std::cout << ed.first << " " << ed.second << "\n";
// }

// the question here is if we need nodes
// and what type of graph edges should be
// potentially we want a multimap where we read in each possible pair of edges for a given starting point
// and see if the third edge is in the graph

  auto t3 {std::chrono::high_resolution_clock::now()};

// for each vertex
// get list of corresponding edges
// have two iterators, with the second iterator starting immediately after the first iterator
// get the second value from each of the iterators, and see if the corresponding edge exists
// if so, we have a 3-clique: check if any of the nodes starts with a 't'

  


  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

int computerHash(std::string comp) {
  char first {comp[0]}, second {comp[1]};
  int hsh {0};
  hsh += 26 * (first-'a');
  hsh += second-'a';
  return hsh;
}

bool startsWith_T(int hsh) {
  if (hsh/26==19) return true;
  return false;
}