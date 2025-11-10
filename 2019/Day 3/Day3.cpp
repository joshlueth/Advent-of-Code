#include "Day3.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>
#include <limits>

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

  // read file
  std::string path1 {}, path2 {};
  std::getline(inputFile, path1);
  std::getline(inputFile, path2);
	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  // put into data structure
  std::stringstream ss1{}, ss2 {};
  ss1.str(path1);
  ss2.str(path2);
  std::vector<std::pair<char,int>> directions1 {}, directions2 {};
  std::string token {};
  while (std::getline(ss1,token,',')) {
    directions1.push_back(std::make_pair(token[0],std::stoi(token.substr(1))));
  }
  while (std::getline(ss2,token,',')) {
    directions2.push_back(std::make_pair(token[0],std::stoi(token.substr(1))));
  }

  // solve problem
  std::vector<std::pair<std::pair<std::pair<char,int>,std::pair<int,int>>,std::pair<int,std::pair<int,int>>>> range1{};
  std::pair<int,int> loc(0,0);
  int dist {0};
  for (std::size_t i=0; i<directions1.size(); i++) {
    char dir {};
    int stay {}, bot{}, top{};
    switch (directions1[i].first) {
      case 'L':
        dir = 'H';
        stay = loc.second;
        bot = loc.first - directions1[i].second;
        top = loc.first;
        loc.first = bot;
        break;
      case 'R':
        dir = 'H';
        stay = loc.second;
        bot = loc.first;
        top = loc.first + directions1[i].second;
        loc.first = top;
        break;
      case 'U':
        dir = 'V';
        stay = loc.first;
        bot = loc.second;
        top = loc.second + directions1[i].second;
        loc.second = top;
        break;
      case 'D':
        dir = 'V';
        stay = loc.first;
        bot = loc.second - directions1[i].second;
        top = loc.second;
        loc.second = bot;
        break;
      default:
        std::cerr << "Only cardinal directions allowed\n";
        return 0;
    }
    dist += directions1[i].second;
    // std::cout << stay << " " << loc.first << " " << loc.second << "\n";
    range1.push_back(std::make_pair(std::make_pair(std::make_pair(dir,stay),std::make_pair(bot,top)),std::make_pair(dist,loc)));
  }

  std::vector<std::pair<std::pair<int,int>,int>> intersect {};

  loc = std::make_pair(0,0);
  dist = 0;
  for (std::size_t i=0; i<directions2.size(); i++) {
    char dir {};
    int stay{}, bot{}, top{};
    switch (directions2[i].first) {
      case 'L':
        dir = 'H';
        stay = loc.second;
        bot = loc.first - directions2[i].second;
        top = loc.first;
        loc.first = bot;
        break;
      case 'R':
        dir = 'H';
        stay = loc.second;
        bot = loc.first;
        top = loc.first + directions2[i].second;
        loc.first = top; 
        break;
      case 'U':
        dir = 'V';
        stay = loc.first;
        bot = loc.second;
        top = loc.second + directions2[i].second;
        loc.second = top;
        break;
      case 'D':
        dir = 'V';
        stay = loc.first;
        bot = loc.second - directions2[i].second;
        top = loc.second;
        loc.second = bot;
        break;
      default:
      std::cerr << "Only cardinal directions allowed\n";
      return 0;
    }
    dist += directions2[i].second;
    // check if intersection occurs
    for (std::size_t j=0; j<range1.size(); j++) {
      if (range1[j].first.first.first == dir) continue;
      // if so, put in intersect variable
      if ((range1[j].first.first.second>=bot && range1[j].first.first.second<=top) && (stay>=range1[j].first.second.first && stay<=range1[j].first.second.second)) {
        // std::cout << "putting in intersection\n";
        // std::cout << range1[j].first.second << " " << stay << std::endl;
        // std::cout << dir << " " << bot << " " << top << std::endl;
        // std::cout << range1[j].first.first << " " << range1[j].second.first << " " << range1[j].second.second << std::endl;
        int total_dist{};
        total_dist = dist + range1[j].second.first;
        // std::cout << total_dist << std::endl;
        switch (dir) {
          case 'H':
            total_dist -= (std::abs(loc.first-range1[j].first.first.second)) + (std::abs(range1[j].second.second.second-stay));
            break;
          case 'V':
            total_dist -= (std::abs(loc.second-range1[j].first.first.second)) + (std::abs(range1[j].second.second.first-stay));
            break;
          default:
            std ::cout << "wrong direction..." << std::endl;
            return 0;
        }
        // std::cout << total_dist << std::endl;
        // std::cout << dist << " " << range1[j].second << " " << top << " " << range1[j].first.first.second << " " << range1[j].first.second.second << " " << stay << " " << total_dist << std::endl;
        intersect.push_back(std::make_pair(std::make_pair(range1[j].first.first.second,stay),total_dist));
      }
    }
  }

  int min_dist1 {std::numeric_limits<int>::max()};
  int min_dist2 {std::numeric_limits<int>::max()};
  for (std::size_t i=0; i<intersect.size(); i++) {
    if (intersect[i].first.first==0 && intersect[i].first.second==0) continue;
    min_dist1 = std::min(min_dist1,std::abs(intersect[i].first.first) + std::abs(intersect[i].first.second));
    min_dist2 = std::min(min_dist2,intersect[i].second);
  }

  std::cout << min_dist1 << std::endl;
  std::cout << min_dist2 << std::endl;

  auto t3 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:                  " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in Input File:       " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Parts 1,2): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";


	return 0;
}
