#include "Day10.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>

int main()
{
	auto t1 {std::chrono::high_resolution_clock::now()};

	std::string fileName {"input.txt"};

	std::ifstream inputFile {fileName};
	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::string inputStr {};
  std::vector<std::vector<int>> map {}, numT{}, isP{};
	while (std::getline(inputFile,inputStr))
	{
    std::vector<int> map_row {};
    for (auto ch : inputStr) {
      map_row.push_back(int(ch-'0'));
    }
    map.push_back(map_row);
	}
	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

// create overlapping topographical map with number of trailheads reachable, ini with -1
  isP = map;
  setArray_n1(isP);
  numT = map;
  setArray_n1(numT);

// for each 0, call recursive findPeaks, findTrails
  int peaks{0}, uniqueTrails{0};
  for (std::size_t i{0}; i<map.size(); ++i) {
    for (std::size_t j{0}; j<map.size(); ++j) {
      if (map[i][j]==0) {
        peaks += findPeaks(i,j,0,map,isP);
        uniqueTrails += findTrails(i,j,0,map,numT);
      }
      setArray_n1(isP);
    }
  }

	auto t3 {std::chrono::high_resolution_clock::now()};

  std::cout << "Total Trailhead Score: " << peaks << "\n";
  std::cout << "Total Trails Score:    " << uniqueTrails << "\n";

// Optional map visualizer - unequal spacing
// for (std::size_t i{0}; i<map.size(); ++i) {
//   for (std::size_t j{0}; j<map[0].size(); ++j) {
//     std::cout << numT[i][j];
//     if (map[i][j]==0) std::cout << "*";
//     std::cout << " ";
//   }
//   std::cout << "\n";
// }

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:      " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  File Reading:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
  std::cout << "  Problem Solving: " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";

	return 0;
}

int findPeaks(std::size_t i, std::size_t j, int cNum, const std::vector<std::vector<int>>& map, std::vector<std::vector<int>>& isP) {
  if (map[i][j]!=cNum) return 0;
  if (cNum==9 && isP[i][j]==-1) {
    isP[i][j] = 1;
    return 1;
  }
  int score {0};
  if (i+1<map.size())    score += findPeaks(i+1,j,cNum+1,map,isP);
  if (i>0)               score += findPeaks(i-1,j,cNum+1,map,isP);            
  if (j+1<map[0].size()) score += findPeaks(i,j+1,cNum+1,map,isP);
  if (j>0)               score += findPeaks(i,j-1,cNum+1,map,isP);
  return score;
}

int findTrails(std::size_t i, std::size_t j, int cNum, const std::vector<std::vector<int>>& map, std::vector<std::vector<int>>& numT) {
  if (map[i][j]!=cNum) return 0;
  if (numT[i][j]!=-1) return numT[i][j];
  if (cNum==9) {
    numT[i][j] = 1;
    return 1;
  }
  int score {0};
  if (i+1<map.size())    score += findTrails(i+1,j,cNum+1,map,numT);
  if (i>0)               score += findTrails(i-1,j,cNum+1,map,numT);            
  if (j+1<map[0].size()) score += findTrails(i,j+1,cNum+1,map,numT);
  if (j>0)               score += findTrails(i,j-1,cNum+1,map,numT);
  numT[i][j] = score;
  return score;
}

void setArray_n1(std::vector<std::vector<int>>& arr) {
  for (auto& i : arr) {
    for (auto& j : i) {
      j = -1;
    }
  }
}
