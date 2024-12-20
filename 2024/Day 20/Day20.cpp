#include "Day20.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector> 
#include <sstream>
#include <set>
#include <map>
#include <utility>
#include <cstdlib>

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

// Okay, so we have a 2D vector with 

// ISSUE 1: I don't know that my distance is correct. It is possible to have a map that gives an incorrect answer:
// ####### #######
// #.....# #23456#
// #.###.# #1###7#
// #S.#.E# #S1#9E#
// ####### #######
// clearly, it takes 8 second to reach the end, and 4 seconds with the cheat
// according to our algorithm for checking cheats, we would save 8 seconds though!
// clearly we need to do BFS/Dijkstra's from start-end and end-start:
// for the above maps, this gives 
// start-end  end-start
// ####### #######
// #23456# #65432#
// #1###7# #7###1#
// #S1#9E# #S9#1E#
// ####### #######
// then to get the amount of time saved by a cheat of length n
// it used to take startpath_score[end coordinates] time to get there
// now it takes startpath_score + endpath_score + n
// To get the time saved, we simply subtract these two.

// Now of course, we need to go through all possible start and ending points for the path
// this will roughly scale with n^2
// probably the easiest way to do this is to get a list of all the '.' coordinates
// then double for loop it
// check the 1-norm distance: if <= cheat distance allowed, check if time_saved > 0
// if so, add it to the map.

  std::vector<std::vector<int>> startpath {};
  std::vector<std::vector<int>> endpath {};

  startpath = Dijkstra(map, start, end, '#', 'c');
  endpath = Dijkstra(map, end, start, '#', 'c');

  int numSteps {startpath[end.first][end.second]};

// for efficiency purposes: get a list of all '.' coordinates including:
// startpath, endpath, xcoord, ycoord
// with startpath <= numSteps and endpath <= numSteps
  auto t3 {std::chrono::high_resolution_clock::now()};

  std::vector<std::vector<int>> path_info {};
  for (std::size_t rw {0}; rw<map.size(); ++rw) {
    for (std::size_t cl {0}; cl<map[0].size(); ++cl) {
      if (map[rw][cl]=='.' && (startpath[rw][cl]<=numSteps || endpath[rw][cl]<=numSteps)) {
        std::vector<int> into {startpath[rw][cl], endpath[rw][cl], int(rw), int(cl)};
        path_info.push_back(into);
      }
    }
  }

  auto t4 {std::chrono::high_resolution_clock::now()};

  int numCheats1 {}, numCheats2 {};
  int threshold {100};
  numCheats1 = findCheats(path_info, numSteps, threshold, 2);
  numCheats2 = findCheats(path_info, numSteps, threshold, 20);

  auto t5 {std::chrono::high_resolution_clock::now()};

  std::cout << "Minimum number of steps to exit: " << numSteps << "\n";
  std::cout << "Number of shortcuts that saved at least " << threshold << " picoseconds, cheat distance 2: " << numCheats1 << "\n";
  std::cout << "Number of shortcuts that saved at least " << threshold << " picoseconds, cheat distance 20: " << numCheats2 << "\n";

  std::cout << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:                     " << std::chrono::duration_cast<std::chrono::microseconds>(t5-t1).count() << "\n";
	std::cout << "  Reading in Input File:          " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (pathing):      " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (getting list): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";
	std::cout << "  Problem Solving (cheats):       " << std::chrono::duration_cast<std::chrono::microseconds>(t5-t4).count() << "\n";

	return 0;
}

std::vector<std::vector<int>> Dijkstra(const std::vector<std::vector<char>>& map, std::pair<std::size_t,std::size_t> start, std::pair<std::size_t,std::size_t> end,
              const char block, const char possible_dir) {
  std::vector<std::vector<bool>> visited (map.size(),std::vector<bool>(map[0].size(),false));
  std::multimap<std::size_t,std::pair<std::size_t,std::size_t>> unvisited {};
  std::vector<std::vector<int>> distance (map.size(),std::vector<int>(map[0].size(),-1));
  std::vector<std::vector<std::pair<std::size_t,std::size_t>>> previous (map.size(), std::vector<std::pair<std::size_t,std::size_t>>(map[0].size(), std::make_pair(0,0)));

  for (std::size_t rw {0}; rw<map.size(); ++rw) {
    for (std::size_t cl {0}; cl<map[0].size(); ++cl) {
      if (map[rw][cl]==block) visited[rw][cl] = true;
    }
  }
  unvisited.insert(std::make_pair(0,start));
  distance[start.first][start.second] = 0;
  std::vector<int> xdir {}, ydir {};
  if (possible_dir=='c') {
    xdir = {0,-1,0,1};
    ydir = {1,0,-1,0};
  } else if (possible_dir=='d') {
    xdir = {0,-1,0,1,1,-1,-1,1};
    ydir = {1,0,-1,0,1,1,-1,-1};
  }
  while (unvisited.size()>0) {
    // set the next node to visit to be the first one in the ordered multimap
    auto node_it = unvisited.begin();
    auto node = *unvisited.begin();
    // if this is the end node, print the path we found
    // if (node.second == end) {
    //   pathTrace(map,previous,end);
    // }
    // otherwise, set node to visited
    visited[node.second.first][node.second.second] = true;
    // for each surrounding node
    for (std::size_t it {0}; it<xdir.size(); ++it) {
      int newx {int(node.second.first)+xdir[it]}, newy {int(node.second.second)+ydir[it]};
      // check if it has been visited
      if (!visited[newx][newy]) {
      // if not, check if it is already in the queue
        std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>::iterator entry {it_inQueue(unvisited,std::make_pair(newx,newy))};
        if (entry != unvisited.end()) {
          // if it is, replace it iff it has a lower score
          if (entry->first > node.first+1) {
            unvisited.erase(entry);
            unvisited.insert(std::make_pair(node.first+1,std::make_pair(newx,newy)));
            distance[newx][newy] = node.first+1;
            previous[newx][newy] = std::make_pair(node.second.first, node.second.second);
          }
        } else {
        // if it is not in the queue, add it to the queue!  
          unvisited.insert(std::make_pair(node.first+1,std::make_pair(newx,newy)));
          distance[newx][newy] = node.first+1;
          previous[newx][newy] = std::make_pair(node.second.first, node.second.second);
        }
      }
    }
    unvisited.erase(node_it);
  }
  return distance;
}

std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>::iterator
  it_inQueue(std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>& queue, std::pair<std::size_t,std::size_t> node) {
  
  for (auto entry {queue.begin()}; entry!=queue.end(); ++entry) {
    if (entry->second == node) return entry;
  }
  return queue.end();
}

void pathTrace (std::vector<std::vector<char>> map, const std::vector<std::vector<std::pair<std::size_t,std::size_t>>>& previous, const std::pair<std::size_t,std::size_t> end) {

  for (auto rw:map) {
    for (auto cl:rw) {
      std::cout << cl;
    }
    std::cout << "\n";
  }
  std::cout << "\n";

  std::pair<std::size_t,std::size_t> prev_node {end};
  while (prev_node != previous[prev_node.first][prev_node.second]) {
    map[prev_node.first][prev_node.second] = 'O';
    prev_node = previous[prev_node.first][prev_node.second];
  }

  for (auto rw:map) {
    for (auto cl:rw) {
      std::cout << cl;
    }
    std::cout << "\n";
  }
  std::cout << "\n";

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

int findCheats(const std::vector<std::vector<int>>& path_info, const int numSteps, const int threshold, const int dist) {

  // std::map<int,int> saved {};
  int counter {0};
  for (std::size_t st {0}; st<path_info.size(); ++st) {
    for (std::size_t en {0}; en<path_info.size(); ++en) {
      int distance_norm {std::abs(path_info[en][3]-path_info[st][3]) + std::abs(path_info[en][2]-path_info[st][2])};
      if (distance_norm <= dist && distance_norm >= 2) {
        int score {path_info[st][0]+path_info[en][1]+distance_norm};
        // if (score < numSteps) {
          // saved[numSteps - score] += 1;
          if (numSteps-score>=threshold) counter += 1;
        // }
      }
    }
  }

  // for (auto pair : saved) {
  //   std::cout << "- There are " << pair.second << " cheats that save " << pair.first << " picoseconds.\n";
  // }

  return counter;
}