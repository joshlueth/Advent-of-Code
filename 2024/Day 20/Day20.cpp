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

  std::vector<std::vector<int>> distance {};
  distance = Dijkstra(map,start,end,'#','c');

  int numSteps {distance[end.first][end.second]};

// print the map with '.' replaced by distances found through Dijkstra
  // for (std::size_t rw {0}; rw<map.size(); ++rw) {
  //   for (std::size_t cl{0}; cl<map[0].size(); ++cl) {
  //     if (map[rw][cl]=='#') {
  //       if (distance[rw][cl]!=-1) std::cout << "\ndistance not -1\n";
  //       std::cout << "  # ";
  //     } else if (map[rw][cl]=='.') {
  //       std::string dist_score {std::to_string(distance[rw][cl])};
  //       if (dist_score.size()<4) dist_score += " ";
  //       while (dist_score.size()<4) dist_score = " " + dist_score;
  //       std::cout << dist_score;
  //     }
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "\n";

// now with the distance, we can iterate through each '#' and find the maximum and minimum score of the four scores surrounding
// the time saved is the difference between those scores

  int threshold {100};
  int counter {0};
  // std::vector<std::vector<int>> saved (map.size(), std::vector<int>(map[0].size(),0));
  std::map<int,int> saved_counting {};
  for (std::size_t rw {1}; rw<map.size()-1; ++rw) {
    for (std::size_t cl{1}; cl<map[0].size()-1; ++cl) {
      if (map[rw][cl]=='.') continue;
      std::vector<int> dist_saved {};
      dist_saved = FindDistanceSaved(rw,cl,distance);
      // saved[rw][cl] = dist_saved;
      for (auto dist : dist_saved) {
        if (dist>=threshold) counter += 1;
        saved_counting[dist] += 1;
      }
    }
  }

// print out how many cheats there are for a given amount of time save
  // for (auto pair : saved_counting) {
  //   std::cout << "- There are " << pair.second << " cheat(s) that save(s) " << pair.first << " picoseconds\n";
  // }

// visualize the distance saved
  // for (std::size_t rw {0}; rw<saved.size(); ++rw) {
  //   for (std::size_t cl {0}; cl<saved[0].size(); ++cl) {
  //     if (rw==0 || cl==0 || rw==saved.size()-1 || cl==saved[0].size()-1) {
  //       std::cout << "  # ";
  //       continue;
  //     } else if (map[rw][cl]=='.') {
  //       std::cout << "  . ";
  //       continue;
  //     }
  //     std::string st {};
  //     st = std::to_string(saved[rw][cl]);
  //     while (st.size()<4) st  = " " + st;
  //     std::cout << st;
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "\n";

  auto t3 {std::chrono::high_resolution_clock::now()};

// Okay, so we have a 2D vector with 

// ISSUE 1: I don't know that my distance is correct. It is possible to have a map that gives an incorrect answer:

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Minimum number of steps to exit: " << numSteps << "\n";
  std::cout << "Number of shortcuts that saved at least " << threshold << " picoseconds: " << counter << "\n";


	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

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

std::vector<int> FindDistanceSaved(const std::size_t rw, const std::size_t cl, const std::vector<std::vector<int>>& distance) {
  std::vector<int> xdir {0,1,0,-1}, ydir {1,0,-1,0};
  std::vector<int> scores_saved {};
  std::vector<int> scores {};
  for (std::size_t dir {0}; dir<4; ++dir) {
    int score {};
    score = distance[rw+xdir[dir]][cl+ydir[dir]];
    scores.push_back(score);
  }
  for (std::size_t i {0}; i<scores.size()-1; ++i) {
    for (std::size_t j {i+1}; j<scores.size(); ++j) {
      if (scores[i]>=0 && scores[j]>=0) {
        scores_saved.push_back(std::abs(scores[i]-scores[j])-2);
      }
    }
  }

  return scores_saved;

}
