#include "Day24.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <utility>
#include <set>
#include <sstream>
#include <algorithm>
#include <tuple>
#include <cmath>

struct node {
  std::string id;
  bool value;
  struct node* parent1;
  struct node* parent2;
  std::string gate {};
};

int main(int argc, char* argv[]) {

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
  std::map<std::string,int> nodes {};
  std::set<std::string> S {};
  std::map<std::string,std::tuple<std::string,std::string,std::string>> gates {};
  std::set<std::pair<std::string,std::string>> graph {};
  bool start_gates {false};
	while (std::getline(inputFile,inputStr))
	{
    if (!start_gates) {
      if (inputStr=="") {
        start_gates = true;
        continue;
      } else {
        // get initial values into 'nodes'
        nodes[inputStr.substr(0,3)] = std::stoi(inputStr.substr(5,1));
        S.insert(inputStr.substr(0,3));
      }
    } else {
      // get logic gates into appropriate data structures
      // note: do not add to nodes, this will be dealt with when actually going through the graph.
      std::stringstream ss {inputStr};
      std::string str_tmp {};
      std::vector<std::string> inputVec {};
      while (getline(ss,str_tmp,' ')) {
        inputVec.push_back(str_tmp);
      }
      gates[inputVec[4]] = std::make_tuple(inputVec[0], inputVec[2], inputVec[1]);
      graph.insert(std::make_pair(inputVec[0],inputVec[4]));
      graph.insert(std::make_pair(inputVec[2],inputVec[4]));
      nodes[inputVec[4]] = -1;
    }
	}

	inputFile.close();

// check that data was read in correctly
  // for (auto en : nodes) {
  //   std::cout << en.first << " " << en.second << "\n";
  // }
  // for (auto en : gates) {
  //   std::cout << en.first << " " << std::get<0>(en.second) << " " << std::get<1>(en.second) << " " << std::get<2>(en.second) << "\n";
  // }

	auto t2 {std::chrono::high_resolution_clock::now()};

// hmmm this one is a little hard because we can clearly understand what is going on but it's not intuitive to algorithmically determine the strategy.
// I think the best way to do it is:
// read in input values, add their values to a map
// when reading in logic gates, 1) add any new keys to map with a value of -1, and 
// 2) create a dependency graph using a directed acyclic graph and topologically sort it
// to do this we need a total of three structures:

// map with nodes and values
// DAG with edge (u,v) meaning u is required by v for a logic gate
// map with v : u1, u2, TYPE that calls values u1 and u2 with gate type TYPE

// then to actually solve the problem, 
// 1) topological sort via vector
// 2) go through vector and run logic gates
// 3) get output

  std::vector<std::string> ordering {};
  for (auto en : nodes) {
    ordering.push_back(en.first);
  }

  ordering = topological_sort(graph,S);
  logic_gates(gates,ordering,nodes);

  std::vector<std::string> solution_ordering {};
  for (auto en : ordering) {
    if (en[0]=='z') {
      solution_ordering.push_back(en);
    }
  }
  std::sort(solution_ordering.begin(),solution_ordering.end());

  for (auto en : solution_ordering) {
    std::cout << en << " \n";
  }

  long long number {0};
  for (std::size_t it {0}; it<solution_ordering.size(); ++it) {
    std::cout << nodes[solution_ordering[it]] << " " << it << " " << (static_cast<long long>(nodes[solution_ordering[it]]) << it) << "\n";
    number += static_cast<long long>(nodes[solution_ordering[it]]) << it;
  }


  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Decimal Number Output: " << number << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

std::vector<std::string> topological_sort (std::set<std::pair<std::string,std::string>> graph, std::set<std::string> S) {
  std::vector<std::string> L {};
  while (S.size()>0) {
    auto nd = S.begin();
    for (auto ed = graph.begin(); ed != graph.end(); ) {
      if (ed->first==*nd) {
        std::string edgeTo {ed->second};
        graph.erase(ed++);
        bool insertion {true};
        for (auto ed2 : graph) {
          if (edgeTo==ed2.second) {
            insertion = false;
            break;
          }
        }
        if (insertion) S.insert(edgeTo);
      } else {
        ++ed;
      }
    }
    L.push_back(*nd);
    S.erase(nd);
  }
  std::cout << "L\n";
  for (auto en : L) std::cout << en << " ";
  std::cout << "\n";
  return L;
}

void logic_gates (std::map<std::string,std::tuple<std::string,std::string,std::string>> gates, std::vector<std::string> ordering, std::map<std::string,int>& values) {
  for (std::size_t it {0}; it<ordering.size(); ++it) {
    std::string updating_node {ordering[it]};
    if (values[updating_node]!=-1) continue;
    std::string gate_type {std::get<2>(gates[updating_node])};
    std::string parent1 {std::get<0>(gates[updating_node])}, parent2 {std::get<1>(gates[updating_node])};
    values[updating_node] = single_gate(values[parent1],values[parent2],gate_type);
  }
}

int single_gate(int p1, int p2, std::string gate_type) {
  int out {};
  if (gate_type=="OR") out = std::max(p1,p2); // or gate via max
  else if (gate_type=="AND") out = p1 * p2; // and gate via multiplication
  else if (gate_type=="XOR") out = (p1+p2) % 2; // xor gate via remainder (modulus)
std::cout << p1 << " " << p2 << " " << gate_type << " " << out << "\n";

  return out;
}