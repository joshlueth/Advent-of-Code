#include "Day12.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <utility>
#include <tuple>

int main()
{
	auto t1 {std::chrono::high_resolution_clock::now()};

	std::string fileName {"sample1.txt"};

	std::ifstream inputFile {fileName};
	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::string inputStr {};
  std::vector<std::vector<char>> map{};
	while (std::getline(inputFile,inputStr))
	{
    std::vector<char> intoArray(inputStr.begin(),inputStr.end());
    map.push_back(intoArray);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

// okay so the plan of attack is simple
// create a boolean array of equal size checking if we have already gone through that cell
// then go through each element in map
// if already traversed, keep going
// if not, create regionA with 1 and regionP with 4 and enter the recursive traversal of all neighboring cells
// when recursively going into a neighboring cell
  // not of a neighboring type, return
  // of a neighboring type and traversed, decrease perimiter count by 1
  // of a neighboring type and not traversed, add to traverse queue (which is a set)

  std::pair<int,int> ap {std::make_pair(0,0)};
  std::vector<std::vector<bool>> traversed(map.size(),std::vector<bool>(map[0].size(),false));
  for (std::size_t rw {0}; rw<map.size(); ++rw) {
    for (std::size_t ch {0}; ch<map[0].size(); ++ch) {
      if (traversed[rw][ch]) continue;
// std::cout << rw << " " << ch << " " << map[rw][ch] << "\n";
      std::pair<int,int> ap_update {};
      ap_update = region(map,traversed,rw,ch);
      ap.first += ap_update.first;
      ap.second += ap_update.second;
    } 
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::cout << "Sum of Area multiplied by Perimeter for all regions: " << ap.first << "\n";
  std::cout << "Sum of Area multiplied by Number of Sides for all regions: " << ap.second << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time: " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in File: " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving: " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";


	return 0;
}

std::pair<int,int> region(const std::vector<std::vector<char>>& map, std::vector<std::vector<bool>>& traversed, std::size_t rw, std::size_t ch ) {
  std::tuple<int,int,int> ap {std::make_tuple(0,0,0)};
  std::set<std::pair<std::size_t,std::size_t>> queue={std::make_pair(rw,ch)};
  while (!queue.empty()){
    cell(map,traversed,queue,ap);
  }
// std::cout << "ap: " << ap.first << " " << ap.second << "\n";
// std::cout << "is queue empty? " << queue.empty() << " " << queue.size() << "\n";
// for (auto pr : queue) {
//   std::cout << "queue entries: " << pr.first << " " << pr.second << "\n";
// }
  return std::make_pair(std::get<0>(ap)*std::get<1>(ap),std::get<0>(ap)*std::get<2>(ap));
}

void cell(const std::vector<std::vector<char>>& map, std::vector<std::vector<bool>>& traversed,
          std::set<std::pair<std::size_t,std::size_t>>& queue, std::tuple<int,int,int>& ap) {
// by definition, have not traversed a cell in the queue, and it is of the same region
  auto cl = *queue.begin();
// for (auto i:traversed) {
//   for (auto j:i) {
//     std::cout << j;
//   }
//   std::cout << "\n";
// }
// std::cout << "location chosen: " << cl.first << " " << cl.second << " map size " << map.size() << " " << map[0].size() << "\n";
  std::get<0>(ap)+=1;
  std::get<1>(ap)+=4;
  if (cl.first>0) {
    if (map[cl.first-1][cl.second]==map[cl.first][cl.second]) {
      std::get<1>(ap)-=1;
      if (!traversed[cl.first-1][cl.second]) {
        queue.insert(std::make_pair(cl.first-1,cl.second));
      }
    }
  }             
  if (cl.first+1<map.size()) {
    if (map[cl.first+1][cl.second]==map[cl.first][cl.second]) {
      std::get<1>(ap)-=1;
      if (!traversed[cl.first+1][cl.second]) {
        queue.insert(std::make_pair(cl.first+1,cl.second));
      }
    }
  }
  if (cl.second>0) {
    if (map[cl.first][cl.second-1]==map[cl.first][cl.second]) {
      std::get<1>(ap)-=1;
      if (!traversed[cl.first][cl.second-1]) {
        queue.insert(std::make_pair(cl.first,cl.second-1));
      }
    }
  }
  if (cl.second+1<map[0].size()){
    if (map[cl.first][cl.second+1]==map[cl.first][cl.second]) {
      std::get<1>(ap)-=1;
      if (!traversed[cl.first][cl.second+1]) {
        queue.insert(std::make_pair(cl.first,cl.second+1));
      }
    }
  }
  traversed[cl.first][cl.second] = true;
// for (auto pr : queue) {
//   std::cout << "queue entries in cell: " << pr.first << " " << pr.second << "\n";
// }
  queue.erase(cl);
}

