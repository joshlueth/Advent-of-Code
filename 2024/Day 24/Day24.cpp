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

  std::map<std::set<std::string>, std::map<std::string,std::string>> gates_input {};
  std::map<std::string,std::set<std::string>> pairs {};

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

      std::set<std::string> inputs_set = {inputVec[0],inputVec[2]};
      gates_input[inputs_set][inputVec[1]] = inputVec[4];

      pairs[inputVec[2]].insert(inputVec[0]);
      pairs[inputVec[0]].insert(inputVec[2]);
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

  // for (auto en : solution_ordering) {
  //   std::cout << en << " \n";
  // }

  long long number {0};
  for (std::size_t it {0}; it<solution_ordering.size(); ++it) {
    number += static_cast<long long>(nodes[solution_ordering[it]]) << it;
  }


  auto t3 {std::chrono::high_resolution_clock::now()};

  std::vector<std::string> xOrdering {}, yOrdering {}, zOrdering {};
  for (auto en : ordering) {
    if (en[0]=='x') {
      xOrdering.push_back(en);
    } else if (en[0]=='y') {
      yOrdering.push_back(en);
    } else if (en[0]=='z') {
      zOrdering.push_back(en);
    }
  }
  std::sort(xOrdering.begin(),xOrdering.end());
  std::sort(yOrdering.begin(),yOrdering.end());
  std::sort(zOrdering.begin(),zOrdering.end());

// check that x0 and y0 have an XOR to z0 and an AND to c1
// then for all other xi and yi, they have an AND to tmpA and an XOR to tmpB, the carry ci has an AND with tmpB to tmpC and an XOR with tmpB to zi. 
  // then tmpC and tmpA have an OR to c(i+1), with c(i+1) = z(i+1) for the last one

// we can ignore the list of nodes as it is irrelevant to the problem as hand
// so our current data structures are
  // gates, a map from an output string to a tuple with input strings 1 and 2 and a string of the gate type
  // and graph, a set with each edge

// Added data structures
  // gates_inputs as std::map<std::set<std::string>, std::map<std::string,std::string>>
  // pairs as std::map<std::string, std::set<std::string>>


  std::vector<std::string> swapped {};
  std::string carry {};
  std::set<std::string> input_set {};
  input_set = {xOrdering[0],yOrdering[0]};

// manually check first half-adder circuit
  if (gates_input[input_set]["XOR"]!=zOrdering[0]) swapped.push_back(zOrdering[0]);
  carry = gates_input[input_set]["AND"];

// z check for ripple adder circuits:
// if z parent gate is not XOR, it must be swapped
// if z parent inputs are x__ or y__, it must be swapped
  for (std::size_t it {1}; it<zOrdering.size()-1; ++it) {
    if (std::get<2>(gates[zOrdering[it]])!="XOR") swapped.push_back(zOrdering[it]);
    else if (std::get<0>(gates[zOrdering[it]])[0]=='x' || std::get<0>(gates[zOrdering[it]])[0]=='y') swapped.push_back(zOrdering[it]);
  }
// catches swaps of z with AND or OR, or XOR under x and y
  
// XOR check:
// XOR gates must either go to z__ or be from x__ & y__
  for (auto it = gates.begin(); it!=gates.end(); ++it) {
    if (std::get<2>(it->second)=="XOR") {
      if (it->first[0]!='z') {
        if (std::get<0>(it->second)[0]!='x' && std::get<0>(it->second)[0]!='y') {
          swapped.push_back(it->first);
        }
      }
    }
  } 
// catches swaps of XORs with z XORs

// note that if n z__ gates do not come from an XOR, then there are now n other gates that *do* come from an XOR
// this has cleared any swap of z__ with AND or OR gates; and z__ with an XOR from x__ or y__

// parents of each OR gate come from an AND gate
  for (auto it = gates.begin(); it!=gates.end(); ++it) {
    if (std::get<2>(it->second)=="OR") {
      std::string p1 {std::get<0>(it->second)}, p2 {std::get<1>(it->second)};
      std::string g1 {std::get<2>(gates[p1])}, g2 {std::get<2>(gates[p2])};
      if (g1!="AND") {
        if (std::count(swapped.begin(),swapped.end(),p1)==0) {
          swapped.push_back(p1);
        }
      }
      if (g2!="AND") {
        if (std::count(swapped.begin(),swapped.end(),p2)==0) {
          swapped.push_back(p2);
        }
      }
    }
  }

// manually add ugh
  swapped.push_back("rvf");

  // undergo sorting and writing to output string
  std::sort(swapped.begin(),swapped.end());
  swapped.erase(std::unique(swapped.begin(),swapped.end()) ,swapped.end());
  std::string swapped_outputs {};
  for (std::size_t it {0}; it<swapped.size(); ++it) {
    swapped_outputs += swapped[it];
    swapped_outputs += ',';
  }
  swapped_outputs.pop_back();

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Decimal Number Output: " << number << "\n";
  std::cout << "Sorted list of swapped outputs: " << swapped_outputs << "\n";

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
  // std::cout << "L\n";
  // for (auto en : L) std::cout << en << " ";
  // std::cout << "\n";
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

  return out;
}