#include "Day21.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
#include <cmath>
#include <climits>
#include <algorithm>

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
  std::vector<std::string> codes{};
	while (std::getline(inputFile,inputStr))
	{
    codes.push_back(inputStr);
	}

	inputFile.close();

  

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::vector<int> codes_int {};
  for (std::size_t cd {0}; cd<codes.size(); ++cd) {
    codes_int.push_back(std::stoi(codes[cd].substr(0,codes.size()-1)));
  }

// check that the input is correct - it is parsed correctly
// for (std::size_t ts {0}; ts<codes.size(); ++ts) {
//   std::cout << codes[ts] << " " << codes_int[ts] << "\n";
// }

  long long complexity {0};

// solve for each code
  int bps {0};
  for (std::size_t cd {0}; cd < codes.size(); ++cd) {
    // get code
    std::string code {codes[cd]};
    // find number of button presses (bps)
    bps = buttonPresses(code);
// std::cout << bps << "\n";
    // complexity score is sum of integer product and number of button presses
    complexity += codes_int[cd] * bps;
  }

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Sum of code complexities: " << complexity << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

int buttonPresses(std::string code) {
  int bps {0};
  // we start at 'A'
  char from {'A'};
  for (auto ch : code) {
    char to {ch};
    int bp_mv {};
    bp_mv = firstKP(from, to);
    bps += bp_mv;

// std::cout << "move bps: " << bp_mv << "\n";
    from = to;
  }
  return bps;
}

int firstKP(char from, char to) {
  int bps {0};
  std::pair<int,int> fromP {}, toP {};
  fromP = buttonKPcoords(from);
  toP = buttonKPcoords(to);
// std::cout << "from " << from << " to " << to << "\n";
  bps += secondKP(fromP, toP);
  return bps;
}

int secondKP(std::pair<int,int> from, std::pair<int,int> to) {
  // so, here's where the loop actually matters. Everything before this point is fixed
  // we have a fixed beginning and ending on the button keypad, and three nested directional keypads

  // given the two coordinates, enumerate every single possible path between them, including the 'A' press
  // make sure that none of the paths cross the empty square

  int bps {INT_MAX};
  int distX {to.first - from.first}, distY {to.second - from.second};
  // X,Y euclidean distance -> (X+Y) C X
  int numPaths {factorial(std::abs(distX)+std::abs(distY))/(factorial(std::abs(distX))*factorial(std::abs(distY)))};
  // get each possible path, call subroutine to get required presses on third keypad
  for (int pth {0}; pth<numPaths; ++pth) {
    int numPresses{0};
    std::string keyPresses {};
    bool emptySquare {};
    emptySquare = getNumPath(keyPresses,from,to,pth,numPaths);
    if (emptySquare) continue;
    keyPresses.push_back('A');
// std::cout << "2KP " << keyPresses << "\n";
    numPresses = thirdKP(keyPresses, 'A');
    bps = std::min(bps,numPresses);
  }
// std::cout << "2KP kps " << bps << "\n";
  return bps;
}

int thirdKP(std::string keyPresses, char from) {
  // for keyPresses sequence, get the two characters
  int bps {INT_MAX};
  char to {keyPresses[0]};
  std::pair<int,int> fromP {directionalKPcoords(from)}, toP {directionalKPcoords(to)};
  int distX {toP.first - fromP.first}, distY {toP.second - fromP.second};
  int numPaths {factorial(std::abs(distX)+std::abs(distY))/(factorial(std::abs(distX))*factorial(std::abs(distY)))};
  for (int it {0}; it<numPaths; ++it) {
// std::cout << "from " << from << " to " << to << " num paths " << numPaths << "\n";
// std::cout << "it counter " << it << "\n";
    int bps_tmp {0};
    bool emptySquare {};
    std::string myKeyPresses {};
    emptySquare = getDirPath(myKeyPresses, fromP, toP, it, numPaths);
    // if (emptySquare) std::cout << "emptySquare " << myKeyPresses << "  " << it << "\n";
    if (emptySquare) continue;
    myKeyPresses.push_back('A');
    bps_tmp += myKP(myKeyPresses);
// std::cout << "trying recursion " << it << " " << myKeyPresses << " " << bps_tmp << "\n";
    if (keyPresses.size()!=1) {
      bps_tmp += thirdKP(keyPresses.substr(1), keyPresses[0]);
    }
    bps = std::min(bps,bps_tmp);
  }

  return bps;
}

// <v<A>>^AA<vA<A>>^AAvAA<^A>A<vA>^A<A>A<vA>^A<A>A<v<A>A>^AAvA<^A>A

int myKP(std::string keyPresses) {
  // for keyPresses sequence, get 1-norm distance between each sequence + the length of the sequence because we need to actually press A
  int bps {0};
  char from {'A'};
// std::cout << "myKeyPresses " << keyPresses << "\n";
  for (std::size_t ky {0}; ky<keyPresses.size(); ++ky) {
    char to {keyPresses[ky]};
    std::pair<int,int> fromP {directionalKPcoords(from)}, toP {directionalKPcoords(to)};
    int bps_tmp {};
    bps_tmp = L1_distance(fromP,toP) + 1;
    bps += bps_tmp;
// std::cout << "bps_tmp on myKP " << bps_tmp << "\n";
    from = to;
  }
  return bps;
}

// std::string directionalKP(char from, char to) {
//   std::pair<int,int> fromP {directionalKPcoords(from)}, toP {directionalKPcoords(to)};
//   std::string path {};


//   path += 'A';
//   return path;
// }

std::pair<int,int> buttonKPcoords(char loc) {
  std::pair<int,int> coords {};
  if (loc=='A') coords = std::make_pair(3,2);
  else if (loc=='0') coords = std::make_pair(3,1);
  else if (loc=='1') coords = std::make_pair(2,0);
  else if (loc=='2') coords = std::make_pair(2,1);
  else if (loc=='3') coords = std::make_pair(2,2);
  else if (loc=='4') coords = std::make_pair(1,0);
  else if (loc=='5') coords = std::make_pair(1,1);
  else if (loc=='6') coords = std::make_pair(1,2);
  else if (loc=='7') coords = std::make_pair(0,0);
  else if (loc=='8') coords = std::make_pair(0,1);
  else if (loc=='9') coords = std::make_pair(0,2);
  return coords;
}

char coordsKPbutton(std::pair<int,int> coords) {
  char loc {};
  if (coords == std::make_pair(3,2)) loc='A';
  else if (coords == std::make_pair(3,1)) loc='0';
  else if (coords == std::make_pair(2,0)) loc='1';
  else if (coords == std::make_pair(2,1)) loc='2';
  else if (coords == std::make_pair(2,2)) loc='3';
  else if (coords == std::make_pair(1,0)) loc='4';
  else if (coords == std::make_pair(1,1)) loc='5';
  else if (coords == std::make_pair(1,2)) loc='6';
  else if (coords == std::make_pair(0,0)) loc='7';
  else if (coords == std::make_pair(0,1)) loc='8';
  else if (coords == std::make_pair(0,2)) loc='9';
  return loc;
}

bool isbuttonKPavoid (std::pair<int,int> coord) {
  if (coord==std::make_pair(3,0)) return true;
  return false;
}

std::pair<int,int> directionalKPcoords(char loc) {
  std::pair<int,int> coords {};
  if (loc=='<') coords = std::make_pair(1,0);
  else if (loc=='>') coords = std::make_pair(1,2);
  else if (loc=='^') coords = std::make_pair(0,1);
  else if (loc=='v') coords = std::make_pair(1,1);
  else if (loc=='A') coords = std::make_pair(0,2);
  return coords;
}

char coordsKPdirectional(std::pair<int,int> coords) {
  char loc {};
  if (coords == std::make_pair(0,2)) loc='A';
  else if (coords == std::make_pair(1,0)) loc='<';
  else if (coords == std::make_pair(1,2)) loc='>';
  else if (coords == std::make_pair(0,1)) loc='^';
  else if (coords == std::make_pair(1,1)) loc='v';
  return loc;
}

bool isdirectionalKPavoid (std::pair<int,int> coord) {
  if (coord==std::make_pair(0,0)) return true;
  return false;
}

int factorial(int num) {
  if (num==0) return 1;
  return num * factorial(num-1);
}

char movement2key(int move, char dir) {
  if (dir=='X') {
    if (move>0) return 'v';
    if (move<0) return '^';
    std::cout << "move must be +-1, not " << move << "\n";
  } else if (dir=='Y') {
    if (move>0) return '>';
    if (move<0) return '<';
    std::cout << "move must be +-1, not " << move << "\n";
  } else {
    std::cout << "this direction is not allowed " << dir << "\n";
  }
  return ' ';
}

int L1_distance(std::pair<int,int> fromP, std::pair<int,int> toP) {
  return std::abs(fromP.first-toP.first) + std::abs(fromP.second-toP.second);
}

bool getNumPath(std::string& keyPresses, std::pair<int,int> from, std::pair<int,int> to, int pth, int numPaths) {
  int distX {to.first-from.first}, distY {to.second-from.second};
  char Xmove {}, Ymove {};
  if (distX!=0) {
    Xmove = movement2key(distX,'X');
  }
  if (distY!=0) {
    Ymove = movement2key(distY,'Y');
  }
  for (int it {0}; it<std::abs(distX); ++it) {
    keyPresses += Xmove;
  }
  for (int it{0}; it<std::abs(distY); ++it) {
    keyPresses += Ymove;
  }

  // get different combination based on switching algorithm for pth value
  int pth_it {pth};
  while (pth_it>0) {
    pth_it -= 1;
    auto xy_compare = [Xmove, Ymove](char a, char b) { 
      if (a==Xmove && b==Ymove) return true;
      return false;
    };
    std::next_permutation(keyPresses.begin(), keyPresses.end(), xy_compare);
    // std::cout << "permutation " << keyPresses << "\n";
  }


  // check if we cross the empty square
  bool emptySquare {false};
  if ((from.first==3 && to.second==0) && pth==numPaths-1) emptySquare = true;
  if ((to.first==3 && from.second==0) && pth==0) emptySquare = true;
  return emptySquare;
}

bool getDirPath(std::string& keyPresses, std::pair<int,int> from, std::pair<int,int> to, int pth, int numPaths) {
  int distX {to.first-from.first}, distY {to.second-from.second};
  char Xmove {}, Ymove {};
  if (distX!=0) {
    Xmove = movement2key(distX,'X');
  }
  if (distY!=0) {
    Ymove = movement2key(distY,'Y');
  }
  for (int it {0}; it<std::abs(distX); ++it) {
    keyPresses += Xmove;
  }
  for (int it{0}; it<std::abs(distY); ++it) {
    keyPresses += Ymove;
  }

  // get different combination based on switching algorithm for pth value
  int pth_it {pth};
  while (pth_it>0) {
    pth_it -= 1;
    auto xy_compare = [Xmove, Ymove](char a, char b) { 
      if (a==Xmove && b==Ymove) return true;
      return false;
    };
    std::next_permutation(keyPresses.begin(), keyPresses.end(), xy_compare);
    // std::cout << "permutation " << keyPresses << "\n";
  }

  // check if we cross the empty square
  bool emptySquare {false};
  if ((from.first==0 && to.second==0) && pth==numPaths-1) emptySquare = true;
  if ((to.first==0 && from.second==0) && pth==0) emptySquare = true;
  return emptySquare;
}



// Writing out exactly what I need to do:
// for each sequence of instructions, look at a specific code
// for each code, we need to navigate between each button on the numeric keypad in sequence
// take two characters: we want to go 'from'->'to'
// enumerate all possible paths for the first robot [ (X+Y)! / (X!*Y!) ]
// for each possible path, enumerate all possible paths for second robot
// looking for minimum length