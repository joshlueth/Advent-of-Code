#include "Day15.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <utility>
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
  std::vector<std::vector<char>> map {};
  std::vector<char> movement {};
  bool getMap {true};

	while (std::getline(inputFile,inputStr))
	{
    if (inputStr=="") getMap = false;
    else if (getMap) {
      std::vector<char> intoArray (inputStr.begin(),inputStr.end());
      map.push_back(intoArray);
    } else {
      movement.insert(movement.end(),inputStr.begin(),inputStr.end());
    }
	}

	inputFile.close();

  std::vector<std::vector<char>> mapCopy {map};

// print map to verify input was read in correctly
  // for (auto i:map) {
  //   for (auto j:i) {
  //     std::cout << j;
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "\n";
  // for (auto i:movement) std::cout << i;
  // std::cout << "\n";

	auto t2 {std::chrono::high_resolution_clock::now()};

// find current position, replace @ with .
// for each move in movement
// if #, exit
// if ., change position
// if O, go into moveBoxes
// continue down the path until we hit
//  . (replace with O, replace original O with .)
//  # (exit)
// At the end, iterate through from 1 to map.size()-2, sum 100*dist_from_top + dist_from_bottom

  std::pair<std::size_t,std::size_t> position {};
  getPosition(position, map);
  for (auto move : movement) {
    moveRobot(position, move, map);
  }
  std::size_t sumGPS {0};
  sumGPS = trackScore(map);


  auto t3 {std::chrono::high_resolution_clock::now()};

// update a second, twice-as-wide map, map2
  std::vector<std::vector<char>> map2 {};
  for (auto rw : mapCopy) {
    std::vector<char> intoM2 {};
    for (auto cl : rw) {
      if (cl=='#') {
        intoM2.push_back(cl);
        intoM2.push_back(cl);
      } else if (cl=='.') {
        intoM2.push_back(cl);
        intoM2.push_back(cl);
      } else if (cl=='@') {
        intoM2.push_back(cl);
        intoM2.push_back('.');
      } else if (cl=='O') {
        intoM2.push_back('[');
        intoM2.push_back(']');
      }
    }
    map2.push_back(intoM2);
  }

// print to verify that map was read in correctly
  // for (auto i:map2) {
  //   for (auto j:i) {
  //     std::cout << j;
  //   }
  //   std::cout << "\n";
  // }
  // std::cout << "\n";
  // for (auto i:movement) std::cout << i;
  // std::cout << "\n";

  std::pair<std::size_t,std::size_t> position2 {};
  getPosition(position2, map2);
  for (auto move : movement) {
    moveRobot(position2, move, map2);
  }
  std::size_t sumGPS2 {0};
  sumGPS2 = trackScore(map2);

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Sum of all boxes' GPS coordinates (warehouse 1): " << sumGPS << "\n";
  std::cout << "Sum of all boxes' GPS coordinates (warehouse 2): " << sumGPS2 << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

	return 0;
}

  void getPosition(std::pair<std::size_t,std::size_t>& position, std::vector<std::vector<char>>& map) {
    for (std::size_t rw {1}; rw<map.size()-1; ++rw) {
      for (std::size_t cl {1}; cl<map[0].size()-1; ++cl) {
        if (map[rw][cl]=='@') {
          map[rw][cl] = '.';
          position = std::make_pair(rw,cl);
          return;
        }
      }
    }
  }

  void moveRobot(std::pair<std::size_t,std::size_t>& position, const char move, std::vector<std::vector<char>>& map) {
    std::pair<std::size_t,std::size_t> newPosition {position};

    adjustPosition(newPosition, move);

    char newSpot {map[newPosition.first][newPosition.second]};

    if (newSpot=='#') return;
    else if (newSpot=='.') {
      position = newPosition;
      return;
    } else if (newSpot=='O') {
      moveBoxes(newPosition, move, map);
      position = newPosition;
      return;
    } else if (newSpot=='[' || newSpot==']') {
      moveBoxes2(newPosition, move, map);
      position = newPosition;
      return;
    }
  }

  void adjustPosition(std::pair<std::size_t,std::size_t>& position, const char move) {
    if (move=='>') position.second += 1;
    else if (move=='^') position.first -= 1;
    else if (move=='<') position.second -= 1;
    else if (move=='v') position.first += 1;
  }

  void reversePosition(std::pair<std::size_t,std::size_t>& position, const char move) {
    if (move=='>') position.second -= 1;
    else if (move=='^') position.first += 1;
    else if (move=='<') position.second += 1;
    else if (move=='v') position.first -= 1;
  }

  void moveBoxes(std::pair<std::size_t,std::size_t>& newPosition, const char move, std::vector<std::vector<char>>& map) {
    std::pair<std::size_t,std::size_t> oldPosition {newPosition};
    while (map[newPosition.first][newPosition.second]=='O') {
      adjustPosition(newPosition,move);
    }
    if (map[newPosition.first][newPosition.second]=='#') {
      reversePosition(oldPosition,move);
      newPosition = oldPosition;
      return;
    } else if (map[newPosition.first][newPosition.second]=='.') {
      map[newPosition.first][newPosition.second] = 'O';
      map[oldPosition.first][oldPosition.second] = '.';
      newPosition = oldPosition;
      return;
    }
  }

  void moveBoxes2(std::pair<std::size_t,std::size_t>& newPosition, const char move, std::vector<std::vector<char>>& map) {
    std::pair<std::size_t,std::size_t> oldPosition {newPosition};

  // if we are moving up and down, we also need to keep track of the other half of the box: go into specialize subroutine
    if (move=='^' || move=='v') {
      moveBoxTree(newPosition, move, map);
      return;
    }

  // while either the change of the new position is still a box or the change of the old position is still a box
    while ( (map[newPosition.first][newPosition.second]=='[' || map[newPosition.first][newPosition.second]==']')) {
      adjustPosition(newPosition,move);
    }
  // if either one is a '#', then we have bumped into a wall and can't change anything
    if (map[newPosition.first][newPosition.second]=='#') {
      reversePosition(oldPosition,move);
      newPosition = oldPosition;
      return;
  // otherwise, shift the boxes (recursively this time, to keep order)
    } else if (map[newPosition.first][newPosition.second]=='.') {
      while (newPosition.first!=oldPosition.first || newPosition.second!=oldPosition.second) {
        std::pair<std::size_t,std::size_t> newpos_back {newPosition};
        reversePosition(newpos_back,move);
        map[newPosition.first][newPosition.second] = map[newpos_back.first][newpos_back.second];
        newPosition = newpos_back;
      }
      map[newPosition.first][newPosition.second] = '.';
      return;
    }
  }

// special moving function, only for moving 2-wide boxes up and down.
  void moveBoxTree(std::pair<std::size_t,std::size_t>& newPosition, const char move, std::vector<std::vector<char>>& map) {
// use breadth-first search
// start by adding both parts of the current box to the list and to the queue
// for each entry in the queue: check the square above/below
// if '#': hit a wall, return.
// if '.': erase from queue, continue.
// if '[' or ']': another box. Check if we have already added it (ie, in boxes).
//  if so, erase from queue and continue.
//  if not, find it's pair; add them both to the list (boxes) and the queue (boxesQ)
// once the queue is empty, get the location in the direction of the move for each entry in boxes in reverse order.
// set map[location] = map[boxes location] (that is, move the box part up)
// then set map[boxes location] = '.' (either it should be this because there is nothing below/above it, or another box will overwrite it when being pushed later
// return
    std::vector<std::pair<std::size_t,std::size_t>> boxes {}, boxesQ {};
    boxes.push_back(newPosition);
    boxesQ.push_back(newPosition);
    std::pair<std::size_t,std::size_t> newPositionPair {};
    newPositionPair = findPair(newPosition,map[newPosition.first][newPosition.second]);
    boxes.push_back(newPositionPair);
    boxesQ.push_back(newPositionPair);
    while (boxesQ.size()>0) {
      std::pair<std::size_t,std::size_t> checkUD {boxesQ[0]};
      adjustPosition(checkUD,move);
      if (map[checkUD.first][checkUD.second]=='#') {
        reversePosition(newPosition,move);
        return;
      } else if (map[checkUD.first][checkUD.second]=='.') {
        boxesQ.erase(boxesQ.begin());
      } else {
          if (std::count(boxes.begin(),boxes.end(),checkUD)==0) {
            std::pair<std::size_t,std::size_t> pairPosition {};
            pairPosition = findPair(checkUD,map[checkUD.first][checkUD.second]);
            boxes.push_back(checkUD);
            boxesQ.push_back(checkUD);
            boxes.push_back(pairPosition);
            boxesQ.push_back(pairPosition);
          }
        boxesQ.erase(boxesQ.begin());
      }
    }
    for (auto it = boxes.rbegin(); it!=boxes.rend(); ++it) {
      std::pair<std::size_t,std::size_t> swapPosition {std::make_pair(it->first, it->second)};
      adjustPosition(swapPosition,move);
      map[swapPosition.first][swapPosition.second] = map[it->first][it->second];
      map[it->first][it->second] = '.';
    }
    return;
  }

  std::pair<std::size_t,std::size_t> findPair(std::pair<std::size_t,std::size_t> position, const char mark) {
    if (mark=='[') adjustPosition(position,'>');
    else if (mark==']') adjustPosition(position,'<');
    return position;
  }

  std::size_t trackScore(const std::vector<std::vector<char>>& map) {
    std::size_t score{0};
    for (std::size_t rw {1}; rw<map.size()-1; ++rw) {
      for (std::size_t cl {1}; cl<map[0].size()-1; ++cl) {
        if (map[rw][cl]=='O' || map[rw][cl]=='[') {
          score += 100 * rw + cl;
        }
      }
    }
    return score;
  }
