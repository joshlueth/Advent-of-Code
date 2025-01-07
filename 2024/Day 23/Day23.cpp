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
#include <map>

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
  // std::map<int,std::vector<int>> edges {};
  std::multimap<int,int> edges {};
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
    nodes.insert(n1);
    nodes.insert(n2);
    // edges[std::min(n1,n2)].push_back(std::max(n1,n2));

    int key {std::min(n1,n2)}, value {std::max(n1,n2)};

    bool emplaced {false};
    auto i1 = edges.equal_range(key);
    for (auto i2=i1.first; i2!=i1.second; ++i2) {
      if (i2->second > value) {
        edges.emplace_hint(i2,key,value);
        emplaced = true;
        break;
      }
    }

    if (!emplaced) edges.emplace(key,value);

	}

	inputFile.close();

  // for (auto ed : edges) {
  //   std::sort(ed.second.begin(),ed.second.end());
  // }

// Verification that input is being read correctly:
// for (auto st : nodes) {
//   std::cout << st << "\n";
// }
// for (auto ed : edges) {
//   std::cout << ed.first << " " << ed.second << "\n";
// }
// std::cout << "\n";
// for (auto ed : edges) {
//   for (auto e : ed.second) {
//     std::cout << ed.first << " " << e << "\n";
//   }
// }

	auto t2 {std::chrono::high_resolution_clock::now()};

// the question here is if we need nodes
// and what type of graph edges should be
// potentially we want a multimap where we read in each possible pair of edges for a given starting point
// and see if the third edge is in the graph

// for each vertex
// get list of corresponding edges
// have two iterators, with the second iterator starting immediately after the first iterator
// get the second value from each of the iterators, and see if the corresponding edge exists
// if so, we have a 3-clique: check if any of the nodes starts with a 't'

  std::set<std::vector<int>> threeClique {};
  for (int current_node : nodes) {
    auto range = edges.equal_range(current_node);
    for (auto i=range.first; i!=range.second; ++i) {
// std::cout << "i " << i->first << " " << i->second << "\n";
      auto j1 = i;
      j1++;
      for (auto j=j1; j!=range.second; ++j) {
// std::cout << "j " << j->first << " " << j->second << "\n";
        auto range2 = edges.equal_range(i->second);
        for (auto k=range2.first; k!=range2.second; ++k) {
// std::cout << "k " << k->first << " " << k->second << "\n";
          if (k->second==j->second) {
// std::cout << "3clique " << i->first << " " << i->second << " " << j->first << " " << j->second << " " << k->first << " " << k->second << "\n";
            threeClique.insert({i->first, i->second, j->second});
            break;
          }
        }
      }
    }
  }

  int t3cliques {0};
  for (auto tc : threeClique) {
    for (std::size_t it {0}; it<tc.size(); ++it) {
      if (startsWith_T(tc[it])) {
        t3cliques +=1;
        break;
      }
    }
  }


  auto t3 {std::chrono::high_resolution_clock::now()};

  std::string output {};
  output = BronKerbosch(nodes, edges);

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "3-Cliques with a computer's name starting with t: " << t3cliques << "\n";
  std::cout << "Sorted maximal clique in string form " << output << "\n";

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
  hsh += 26 * (first - 'a');
  hsh += second - 'a';
  return hsh;
}

std::string reverseComputerHash(int comp) {
  std::string hsh {};
  hsh += char(comp/26 + 'a');
  hsh += char(comp%26 + 'a');
  return hsh;
}

bool startsWith_T(int hsh) {
  if (hsh/26==19) return true;
  return false;
}

std::string BronKerbosch (std::set<int> nodes, std::multimap<int,int> edges) {

}