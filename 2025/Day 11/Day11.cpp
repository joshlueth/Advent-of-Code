#include "Day11.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <unordered_map>

bool key_exists(std::string key, std::unordered_map<std::string,long long>& map) {
  if (map.find(key)==map.end()) return false;
  return true;
}

long long track_path(std::string start, std::string end, std::unordered_map<std::string,std::vector<std::string>>& graph, std::unordered_map<std::string,long long>& memoize, bool dac, bool fft) {
  long long sum=0;
  for (std::size_t word=0; word<graph[start].size(); word++) {
    std::string next = graph[start][word];
    if (next==end) {
      return (dac&&fft);
    }
    std::string key = next;
    key += (dac ? "1" : "0");
    key += (fft ? "1" : "0");
    if (!key_exists(key,memoize)) {
      long long result = track_path(next,end,graph,memoize,dac||(next=="dac"),fft||(next=="fft"));
      memoize[key] = result;
    }
    sum += memoize[key];
  }
  return sum;
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
  long long part1{0}, part2{0};

  // we make the assumption that this is not a cyclic graph (otherwise the answer would be infinity!)
  // the solution we get will depend on the type of data structure we use for the graph
  // use of an adjacency matrix will mean matrix multiplication until everything other than 'out' is zero
  // alternatively, we could follow each possible path through the graph with memoization (=> this seems better, since it is more general)

  std::unordered_map<std::string,std::vector<std::string>> graph {};
	while (std::getline(inputFile,inputStr))
	{
    std::string key{inputStr.substr(0,3)};
    std::vector<std::string> nodes {};
    for (std::size_t ss=5; ss<inputStr.size(); ss+=4) {
      nodes.push_back(inputStr.substr(ss,3));
    }
    graph[key] = nodes;
	}

	inputFile.close();
  

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::unordered_map<std::string,long long> memoize1 {};
  part1 = track_path("you","out",graph,memoize1,true,true);

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::unordered_map<std::string,long long> memoize2 {};
  part2 = track_path("svr","out",graph,memoize2,false,false);

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
