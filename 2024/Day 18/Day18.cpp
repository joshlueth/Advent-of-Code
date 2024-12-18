#include "Day18.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector> 
#include <sstream>
#include <set>
#include <map>
#include <utility>

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
  std::vector<std::pair<std::size_t,std::size_t>> bytes {};
	while (std::getline(inputFile,inputStr))
	{
    std::stringstream ss {inputStr};
    std::string num {};
    std::vector<int> c_pair {};
    while (getline(ss,num,',')) {
      c_pair.push_back(std::stoi(num));
    }
    bytes.push_back(std::make_pair(c_pair[0],c_pair[1]));
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::size_t sz {}, n_bytes {};
;
  if (fileName == "sample.txt") {
    sz = 7;
    n_bytes = 12;
  } else if (fileName == "input.txt") {
    sz = 71;
    n_bytes = 1024;
  }

  std::vector<std::vector<char>> map(sz, std::vector<char>(sz,'.'));
  for (std::size_t byte_it {0}; byte_it<n_bytes; ++byte_it) {
    std::pair<std::size_t,std::size_t> byte {bytes[byte_it]};
    map[byte.first][byte.second] = '#';
  }

  std::pair<std::size_t,std::size_t> start {0,0}, end{sz-1,sz-1};
  std::size_t numSteps {};

  numSteps = Dijkstra(map,start,end,'#','c');

  auto t3 {std::chrono::high_resolution_clock::now()};

// so, given the map, we want to continue dropping bytes (clearly starting with n_bytes) until there is no path
// the naive way would be to rerun Dijkstra's every single byte that blocks the path, maybe starting from the blocked node?
// the slightly-less-naive-but-still-naive way would be to copy Day 16 and get some sort of path tracker and then knock of the paths that are blocker for each tracker

// My initial thought is that the easier problem to solve is going to be the dual problem. 
// There is no maze if we can travel, including diagonals, from the left-bottom edge to the top-right edge

/*
  std::vector<std::pair<std::size_t,std::size_t>> start_sides {};

  std::size_t x_side {}, y_side {};
  
  x_side = map.size()-1;
  y_side = 0;

  for (std::size_t ys {1}; ys<map[0].size()-1; ++ys) {
    if (map[x_side][ys]=='#') start_sides.push_back(std::make_pair(x_side,ys));
  }

  for (std::size_t xs {1}; xs<map.size(); ++xs) {
    if (map[xs][y_side]=='#') start_sides.push_back(std::make_pair(xs,y_side));
  }

  bool blocked {false};
  std::size_t xcoord {}, ycoord {};
  while (!blocked) {
  // add in the byte
    std::pair<std::size_t,std::size_t> byte {bytes[n_bytes]};
    map[byte.first][byte.second] = '#';

  // if the dropped byte gives us another starting location along the side, add it the list of starting locations
    if (byte.first==x_side || byte.second==y_side) start_sides.push_back(std::make_pair(byte.first,byte.second));

  // check if there are any paths though the '#' (where '.' are the blockers and we are travelling 'd' - diagonally)
  // check starting along the  
    for (auto st : start_sides) {
      int tmp_steps {0};
      tmp_steps = Dijkstra(map,st,st,'.','d');

      if (tmp_steps!=0) {
        blocked = true;
        xcoord = byte.first;
        ycoord = byte.second;
        break;
      }
    }

  // increase the number of bytes to drop the next byte
    n_bytes += 1;
  }

*/


  std::size_t min_bytes {n_bytes+1}, max_bytes{bytes.size()};
  std::vector<std::vector<char>> map_cp {map};
  std::size_t searches {0};
  while (min_bytes!=max_bytes) {
    searches += 1;
    map = map_cp;
    // get binary search number
    // note that since min_bytes has an exit and max_bytes doesn't, we will take min_bytes+1. Since we haven't tried it yet, we want the division to floor (truncate)
    std::size_t try_bytes {(min_bytes+max_bytes)/2};

    // add the bytes to the map
    for (std::size_t byte_it {min_bytes-1}; byte_it<try_bytes; ++byte_it) {
      std::pair<std::size_t,std::size_t> byte {bytes[byte_it]};
      map[byte.first][byte.second] = '#';
    }

    // check if Dijkstra's can solve
    int steps {0};

    steps = Dijkstra(map,start,end,'#','c');
    if (steps==0) {
      max_bytes = try_bytes;
    } else {
      min_bytes = try_bytes+1;
      map_cp = map;
    }
  }

  std::size_t blocking_byte = min_bytes - 1;
  std::size_t xcoord {}, ycoord {};
  xcoord = bytes[blocking_byte].first;
  ycoord = bytes[blocking_byte].second;

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Minimum number of steps to exit: " << numSteps << "\n";
  std::cout << "Byte coordinates of blocking byte: " << xcoord << "," << ycoord << "\n";
  std::cout << "Took " << searches << " searches for Part 2" << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

std::size_t Dijkstra(const std::vector<std::vector<char>>& map, std::pair<std::size_t,std::size_t> start, std::pair<std::size_t,std::size_t> end,
  const char block, const char possible_dir) {
  std::vector<std::vector<bool>> visited (map.size(),std::vector<bool>(map[0].size(),false));
  std::multimap<std::size_t,std::pair<std::size_t,std::size_t>> unvisited {};
  std::vector<std::vector<std::pair<std::size_t,std::size_t>>> previous (map.size(), std::vector<std::pair<std::size_t,std::size_t>>(map[0].size(), std::make_pair(0,0)));
  std::size_t min_path {0};

  for (std::size_t rw {0}; rw<map.size(); ++rw) {
    for (std::size_t cl {0}; cl<map[0].size(); ++cl) {
      if (map[rw][cl]==block) visited[rw][cl] = true;
    }
  }
  unvisited.insert(std::make_pair(0,start));
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
    // if this is the end node, get the minimum score
    if (end==start) {
      bool reached_side {false};
      reached_side = check_righttop(map,node.second);
      if (reached_side) {
        min_path = node.first;
        return min_path;
      }
    } else if (node.second == end) {
      // pathTrace(map,previous,end);
      // return node.first;
      min_path = node.first;
      return min_path;
    }
    // otherwise, set node to visited
    visited[node.second.first][node.second.second] = true;
    // for each surrounding node
    for (std::size_t it {0}; it<xdir.size(); ++it) {
      int newx {int(node.second.first)+xdir[it]}, newy {int(node.second.second)+ydir[it]};
      // check if the node is in bounds
      if (inBounds(newx,newy,map.size())) {
        // if it is, check if it has been visited
        if (!visited[newx][newy]) {
        // if not, check if it is already in the queue
          std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>::iterator entry {it_inQueue(unvisited,std::make_pair(newx,newy))};
          if (entry != unvisited.end()) {
            // if it is, replace it iff it has a lower score
            if (entry->first > node.first+1) {
              unvisited.erase(entry);
              unvisited.insert(std::make_pair(node.first+1,std::make_pair(newx,newy)));
              previous[newx][newy] = std::make_pair(node.second.first, node.second.second);
            }
          } else {
          // if it is not in the queue, add it to the queue!  
            unvisited.insert(std::make_pair(node.first+1,std::make_pair(newx,newy)));
            previous[newx][newy] = std::make_pair(node.second.first, node.second.second);
          }
        }
      }
    }
    unvisited.erase(node_it);
  }

  // if (min_path==0) {
  //   std::cout << "There is no path to the desired end point\n";
  // }
  return min_path;

}

std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>::iterator
  it_inQueue(std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>& queue, std::pair<std::size_t,std::size_t> node) {
  
  for (auto entry {queue.begin()}; entry!=queue.end(); ++entry) {
    if (entry->second == node) return entry;
  }
  return queue.end();
}

bool inBounds (int c1, int c2, int bnd) {
  if (c1>=0 && c2>=0 && c1<bnd && c2<bnd) return true;
  return false;
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

bool check_righttop(const std::vector<std::vector<char>>& map, const std::pair<std::size_t,std::size_t> node) {
  if (node.first==0 || node.second==map[0].size()-1) return true;
  return false;
}