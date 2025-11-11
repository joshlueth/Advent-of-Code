#include "Day2.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <unordered_map>

#define RED 0
#define GREEN 1
#define BLUE 2

std::unordered_map<std::string,int> colors { {"red",RED},{"green",GREEN},{"blue",BLUE}};

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
  std::string game{}, show{}, item{};
  std::tuple<int,int,int> max {12,13,14};
  int allowable {0}, counter{1}, minimum {0};
	while (std::getline(inputFile,inputStr))
	{
    bool possible=true;
    int minR=0, minG=0, minB=0;
    game = inputStr.substr(inputStr.find(':',0)+1);
    std::stringstream ss {game};
    while (std::getline(ss,show,';')) {
      std::stringstream ss2(show);
      while(std::getline(ss2,item,',')) {
        std::string col {}, num {};
        std::stringstream ss3(item.substr(1));
        std::getline(ss3,num,' ');
        std::getline(ss3,col,' ');
        switch (colors[col]) {
          case RED:
            minR = std::max(minR,std::stoi(num));
            if (std::stoi(num)>std::get<RED>(max)) possible = false;
            break;
          case GREEN:
            minG = std::max(minG,std::stoi(num));
            if (std::stoi(num)>std::get<GREEN>(max)) possible = false;
            break;
          case BLUE:
            minB = std::max(minB,std::stoi(num));
            if (std::stoi(num)>std::get<BLUE>(max)) possible = false;
            break;
        }
      }
    }
    if (possible) allowable+=counter;
    minimum += minR*minG*minB;
    counter++;
  }

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};


  auto t3 {std::chrono::high_resolution_clock::now()};


  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << allowable << "\n";
  std::cout << minimum << "\n";
	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}
