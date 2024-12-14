#include "Day14.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cctype>
#include <sstream>
#include <iterator>

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
  std::vector<std::vector<int>> pv {};
	while (std::getline(inputFile,inputStr))
	{
    std::string nums {};
    for (auto ch : inputStr) {
      if (std::isdigit(ch) || ch=='-') nums += ch;
      else if (ch==',' || ch==' ') nums+= ' ';
    }
    std::stringstream ss {nums};
    std::string n {};
    std::vector<int> intoPV {};
    while(getline(ss,n,' ')) {
      intoPV.push_back(std::stoi(n));
    }
    pv.push_back(intoPV);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::vector<std::vector<int>> pv_copy {pv};


  std::vector<int> grid_sz {}, half{};
  if (fileName=="sample.txt"||fileName=="sample1.txt") grid_sz = {7,11};
  else if (fileName=="input.txt") grid_sz = {103,101};
  half.push_back((grid_sz[0]-1)/2);
  half.push_back((grid_sz[1]-1)/2);

  int seconds {100};
  int safety {};
  safety = nSeconds(seconds, pv, grid_sz, half);

  auto t3 {std::chrono::high_resolution_clock::now()};

  for (int sec {1}; sec<10000; sec++) {
    nSeconds(1,pv_copy,grid_sz,half);
    print2screen(pv_copy,grid_sz,sec);
  }

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Safety Factor: " << safety << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

	return 0;
}

int nSeconds(int seconds, std::vector<std::vector<int>>& pv, const std::vector<int>& grid_sz, const std::vector<int>& half) {
  std::vector<int> quadrants {0,0,0,0};
  for (std::size_t it {0}; it<pv.size(); ++it) {

    if (pv[it][3]<0) pv[it][3] += grid_sz[0];
    if (pv[it][2]<0) pv[it][2] += grid_sz[1];

    int new_x {}, new_y {};
    new_x = pv[it][1]+seconds*pv[it][3];
    new_y = pv[it][0]+seconds*pv[it][2];

    new_x %= grid_sz[0];
    new_y %= grid_sz[1];

    pv[it][0] = new_y;
    pv[it][1] = new_x;

    if (new_x<half[0] && new_y<half[1]) quadrants[0] += 1;
    if (new_x>half[0] && new_y<half[1]) quadrants[1] += 1;
    if (new_x>half[0] && new_y>half[1]) quadrants[2] += 1;
    if (new_x<half[0] && new_y>half[1]) quadrants[3] += 1;

  }

  int safety {1};
  for (auto robots : quadrants) {
    safety *= robots;
  }

  return safety;
}

void print2screen(const std::vector<std::vector<int>>& pv, const std::vector<int>& grid_sz, int seconds) {
  std::vector<std::vector<bool>> isRobot(unsigned(grid_sz[0]),std::vector<bool>(unsigned(grid_sz[1]),false));
  for (auto robot : pv) {
    isRobot[unsigned(robot[1])][unsigned(robot[0])] = true;
  }

  int score {};
  score = surrounding(isRobot);

  if (score<300) return;
  std::cout << "After " << seconds << " seconds: (score = " << score << ")" << "\n";

  for (auto x : isRobot) {
    for (auto y : x) {
      if (y) std::cout << "0";
      else std::cout << " ";
    }
    std::cout << "\n";
  }
  std::cout << "\n" << "\n";
}

int surrounding(const std::vector<std::vector<bool>>& isRobot) {
  int score{0};
  for (std::size_t rw {0}; rw<isRobot.size()-1; ++rw) {
    for (std::size_t cl {0}; cl<isRobot[0].size()-1; ++cl) {
      if (isRobot[rw][cl]) {
        if (isRobot[rw+1][cl]) score += 1;
        if (isRobot[rw][cl+1]) score += 1;
      }
    }
  }
  return score;
}
