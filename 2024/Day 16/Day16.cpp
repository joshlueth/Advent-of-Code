#include "Day16.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
#include <algorithm>
#include <map>
#include <tuple>
#include <iterator>
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
  std::vector<std::vector<char>> map {};
	while (std::getline(inputFile,inputStr))
	{
    std::vector<char> intoArray(inputStr.begin(),inputStr.end());
    map.push_back(intoArray);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

// find the start, end of the map
  std::pair<std::size_t,std::size_t> start{}, end{};
  start = findChar(map, 'S');
  end = findChar(map, 'E');
  map[start.first][start.second] = '.';
  map[end.first][end.second] = '.';
  
// create node map : row, col, direction (ENWS)
  std::vector<std::vector<std::vector<int>>> nodes{};
  std::vector<std::vector<std::vector<bool>>> visited {};
  for (auto rw:map) {
    std::vector<std::vector<int>> intoNodes1 {};
    std::vector<std::vector<bool>> intoVisited1 {};
    for (auto cl:rw) {
      std::vector<int> intoNodes2 {};
      intoNodes2={-1,-1,-1,-1};
      intoNodes1.push_back(intoNodes2);

      std::vector<bool> intoVisited2 {};
      if (cl=='#') intoVisited2 = {true,true,true,true};
      if (cl=='.') intoVisited2 = {false,false,false,false};
      intoVisited1.push_back(intoVisited2);
    }
    nodes.push_back(intoNodes1);
    visited.push_back(intoVisited1);
  }

// use Dijkstra's algorithm
  std::tuple<std::size_t,std::size_t,std::size_t> startTuple {std::make_tuple(start.first,start.second,0)};
  std::multimap<int,std::tuple<std::size_t,std::size_t,std::size_t>> unvisited {};
  unvisited.insert(std::make_pair(0,startTuple));
  std::vector<std::vector<std::vector<std::vector<std::tuple<std::size_t,std::size_t,std::size_t>>>>> pathing{};
  pathing = Dijkstra(nodes,visited,unvisited);

  int pathScore {};
  std::vector<int>::iterator end_it {std::min_element(nodes[end.first][end.second].begin(),nodes[end.first][end.second].end())};
  pathScore = *end_it;

  std::tuple<std::size_t,std::size_t,std::size_t> end_tuple {std::make_tuple(end.first,end.second,std::distance(nodes[end.first][end.second].begin(),end_it))};
  std::set<std::tuple<std::size_t,std::size_t,std::size_t>> tiles_dir{};
  tiles_dir.insert(end_tuple);
  pathTrace(pathing,end_tuple,tiles_dir);

  std::set<std::pair<std::size_t,std::size_t>> tiles {};
  for (std::tuple<std::size_t,std::size_t,std::size_t> tpl : tiles_dir) {
    tiles.insert(std::make_pair(std::get<0>(tpl),std::get<1>(tpl)));
  }

  std::size_t numTiles {};
  numTiles = tiles.size();


  auto t3 {std::chrono::high_resolution_clock::now()};

// for part 2, the best way is probably to create a data structure inside Dijkstra that keeps track of optionally many previous nodes
// then when we return this data structure, we begin at end and find add each new node to a set

// data structure is 'pathing': [rw][cl][dir][node#][tuple:rw,cl,dir of previous node]

  // for (auto rw : nodes) {
  //   for (auto cl : rw) {
  //     for (auto dir : cl) {
  //       std::cout << dir << " ";
  //     }
  //     std::cout << "  ";
  //   }
  //   std::cout << "\n\n";
  // }

  std::cout << "Lowest Path Score: " << pathScore << "\n";
  std::cout << "Number of Tiles in *a* Best Path: " << numTiles << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Parts 1+2): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";


	return 0;
}

std::pair<std::size_t,std::size_t> findChar(const std::vector<std::vector<char>>& map, const char lookingFor) {
  std::pair<std::size_t,std::size_t> loc{};
  for (std::size_t rw {1}; rw<map.size()-1; ++rw) {
    for (std::size_t cl{1}; cl<map[0].size()-1; ++cl) {
      if (map[rw][cl]==lookingFor) {
        loc = std::make_pair(rw,cl);
        return loc;
      }
    }
  }
  return loc;
}

// Dijkstra's algorithm explained:
// go to node with smallest distance (originally, this is starting node with distance 0)
// add/update all unvisited neighbors with their distances (in map); key[distance] : value[rw,cl,dir]
// remove current node (should be first entry for an ordered multimap)
// once again, go to node with smallest distance and repeat

std::vector<std::vector<std::vector<std::vector<std::tuple<std::size_t,std::size_t,std::size_t>>>>> Dijkstra(std::vector<std::vector<std::vector<int>>>& nodes, std::vector<std::vector<std::vector<bool>>>& visited, std::multimap<int,std::tuple<std::size_t,std::size_t,std::size_t>>& unvisited) {
// where there are still unreached nodes to be visited
  std::vector<std::vector<std::vector<std::vector<std::tuple<std::size_t,std::size_t,std::size_t>>>>> 
    pathing(nodes.size(),std::vector<std::vector<std::vector<std::tuple<std::size_t,std::size_t,std::size_t>>>>(nodes[0].size(),
      std::vector<std::vector<std::tuple<std::size_t,std::size_t,std::size_t>>>(4)));

  while (unvisited.size()!=0) {
// set current starting node
  auto cnode = *unvisited.begin();
// std::cout << "current central node " << cnode.first << " " << std::get<0>(cnode.second) << " " << std::get<1>(cnode.second) << " " << std::get<2>(cnode.second) << "\n";
// update the distance, set as visited
  nodes[std::get<0>(cnode.second)][std::get<1>(cnode.second)][std::get<2>(cnode.second)] = cnode.first;
  visited[std::get<0>(cnode.second)][std::get<1>(cnode.second)][std::get<2>(cnode.second)] = true;
// add/update all unvisited neighbors (not those with a value of -2)
  // options are: rotate 90 degrees CW, rotate 90 degrees CCW, go forward
  std::tuple<std::size_t,std::size_t,std::size_t> newNode {};
// if not visited
// check if in unvisited set
// if not: add. if so: check the distance
  newNode = CW(cnode.second);
// std::cout << "check surrounding nodes CW " << std::get<0>(newNode) << " " << std::get<1>(newNode) << " " << std::get<2>(newNode) << " " << visited[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)]<< "\n";
  if (!visited[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)]) {
    std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator it {currentValue(newNode, unvisited)};
    if (it!=unvisited.end() && cnode.first+1000<it->first) {
      unvisited.erase(it);
      unvisited.insert(std::make_pair(cnode.first+1000,newNode));
      // new path
      std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> path {};
      path.push_back(cnode.second);
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] = path;
    } else if (it!=unvisited.end() && cnode.first+1000==it->first) {
      // multiple paths
      std::tuple<std::size_t,std::size_t,std::size_t> path {cnode.second};
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)].push_back(path);
    } else if (it==unvisited.end()) {
      unvisited.insert(std::make_pair(cnode.first+1000,newNode));
      // new path
      std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> path {};
      path.push_back(cnode.second);
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] = path;
    }
  }
  newNode = CCW(cnode.second);
// std::cout << "check surrounding nodes CCW " << std::get<0>(newNode) << " " << std::get<1>(newNode) << " " << std::get<2>(newNode) << " " << visited[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] << "\n";
  if (!visited[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)]) {
    std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator it {currentValue(newNode, unvisited)};
    if (it!=unvisited.end() && cnode.first+1000<it->first) {
      unvisited.erase(it);
      unvisited.insert(std::make_pair(cnode.first+1000,newNode));
      // new path
      std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> path {};
      path.push_back(cnode.second);
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] = path;
    } else if (it!=unvisited.end() && cnode.first+1000==it->first) {
      // multiple paths
      std::tuple<std::size_t,std::size_t,std::size_t> path {cnode.second};
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)].push_back(path);
    } else if (it==unvisited.end()) {
      unvisited.insert(std::make_pair(cnode.first+1000,newNode));
      // new path
      std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> path {};
      path.push_back(cnode.second);
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] = path;
    }
  }
  newNode = forward(cnode.second);
// std::cout << "check surrounding nodes foward " << std::get<0>(newNode) << " " << std::get<1>(newNode) << " " << std::get<2>(newNode) << " " << visited[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)]<< "\n";
  if (!visited[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)]) {
    std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator it {currentValue(newNode, unvisited)};
    if (it!=unvisited.end() && cnode.first+1<it->first) {
      unvisited.erase(it);
      unvisited.insert(std::make_pair(cnode.first+1,newNode));
      // new path
      std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> path {};
      path.push_back(cnode.second);
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] = path;
    } else if (it!=unvisited.end() && cnode.first+1==it->first) {
      // multiple paths
      std::tuple<std::size_t,std::size_t,std::size_t> path {cnode.second};
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)].push_back(path);
    } else if (it==unvisited.end()) {
      unvisited.insert(std::make_pair(cnode.first+1,newNode));
      // new path
      std::vector<std::tuple<std::size_t,std::size_t,std::size_t>> path {};
      path.push_back(cnode.second);
      pathing[std::get<0>(newNode)][std::get<1>(newNode)][std::get<2>(newNode)] = path;
    }
  }

// remove current node
  unvisited.erase(unvisited.begin());
// repeat while loop: leads to going to node with smallest distance
  }
  return pathing;
}

// ENWS: CCW increases, CW decreases
std::tuple<std::size_t,std::size_t,std::size_t> CW (std::tuple<std::size_t,std::size_t,std::size_t> current) {
  std::tuple<std::size_t,std::size_t,std::size_t> newNodes {current};
  if (std::get<2>(current)==0) std::get<2>(newNodes) = 3;
  else std::get<2>(newNodes) -= 1;
  return newNodes;
}

std::tuple<std::size_t,std::size_t,std::size_t> CCW (std::tuple<std::size_t,std::size_t,std::size_t> current) {
  std::tuple<std::size_t,std::size_t,std::size_t> newNodes {current};
  if (std::get<2>(current)==3) std::get<2>(newNodes) = 0;
  else std::get<2>(newNodes) += 1;
  return newNodes;
}

// E is {0,1}, N is {-1,0}, W is {0,-1}, S is {1,0}
std::tuple<std::size_t,std::size_t,std::size_t> forward (std::tuple<std::size_t,std::size_t,std::size_t> current) {
  std::tuple<std::size_t,std::size_t,std::size_t> newNodes {current};
  if (std::get<2>(current)==0) std::get<1>(newNodes) += 1;
  if (std::get<2>(current)==1) std::get<0>(newNodes) -= 1; 
  if (std::get<2>(current)==2) std::get<1>(newNodes) -= 1;
  if (std::get<2>(current)==3) std::get<0>(newNodes) += 1; 
  return newNodes;
}

std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator 
  currentValue(std::tuple<std::size_t,std::size_t,std::size_t> node, std::multimap<int,std::tuple<std::size_t,std::size_t,std::size_t>>& unvisited) {
  for (std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator entry {unvisited.begin()}; entry != unvisited.end(); ++entry) {
    if (entry->second==node) {
      return entry;
    }
  }
  std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator ending {unvisited.end()};
  return ending;
}

void pathTrace(const std::vector<std::vector<std::vector<std::vector<std::tuple<std::size_t,std::size_t,std::size_t>>>>>& pathing, 
              const std::tuple<std::size_t,std::size_t,std::size_t>& end, std::set<std::tuple<std::size_t,std::size_t,std::size_t>>& tiles) {
// data structure is 'pathing': [rw][cl][dir][node#][tuple:rw,cl,dir of previous node]
  for (std::tuple<std::size_t,std::size_t,std::size_t> node_tuple : pathing[std::get<0>(end)][std::get<1>(end)][std::get<2>(end)]) {
    if (tiles.find(node_tuple)==tiles.end()) {
      tiles.insert(node_tuple);
      pathTrace(pathing, node_tuple, tiles);
    }
  }

  }