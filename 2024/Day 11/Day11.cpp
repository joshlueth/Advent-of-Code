#include "Day11.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <vector>
#include <iterator>
#include <unordered_map>

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
  std::getline(inputFile,inputStr);
  std::stringstream ss {inputStr};
  std::vector<std::string> line(std::istream_iterator<std::string>(ss),{});

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  long long stoneCount25 {0};
  stoneCount25 += numStones(line,25);

	auto t3 {std::chrono::high_resolution_clock::now()};

  long long stoneCount75 {0};
  stoneCount75 += numStones(line,75);

	auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Number of Stones after 25 blinks: " << stoneCount25 << "\n";
  std::cout << "Number of Stones after 75 blinks: " << stoneCount75 << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:      " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
  std::cout << "  File Reading:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (25): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
  std::cout << "  Problem Solving (75): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

	return 0;
}

// okay so all of this was maybe a good try, but it's clearly horribly inefficient *at best*
// what I believe we need to do are 1 of 2 things - maybe both?
// 1) instead of tracking each individiual stone, track a dictionary of them since only the amount of stones of each number matters
// 2) use memoization to keep track of the results so we aren't calculating the stone value twice.

// I believe that these are actually mutually exclusive since memoization is only useful is we are reusing the calculatiosn
// since we have a two parameter family, number && blinks, there is not necessarily a way to memoize that function
// what I can, and I think will do for learning purposes, is to use a dictionary for the stones, but use memoize for the input return of an individual stone

// Therefore the program outline is 
// READ in file
// CREATE stone dictionary (unordered multimap)
// CALL numStones(list,blinks)
//  FOR all blinks
//    FOR each stone in the dictionary, 
//      CALL blinkss(singlestone) - *memoized*
//      UPDATE/replace new dictionary (??)

long long numStones(std::vector<std::string> list, int blinks) {
  std::unordered_map<std::string,std::pair<std::string,std::string>> cache {};
  std::unordered_map<std::string,long long> stone_dict {};

  for (auto stone : list) { // valid since the default constructor for type int is 0!
    stone_dict[stone]++;
  }

  for (int bk {0}; bk<blinks; ++bk) {
    stone_dict = blink(stone_dict,cache);
  }

  long long sum {0};
  for (const auto& kv_pair : stone_dict) {
    sum += kv_pair.second;
  }
  return sum;
}

std::pair<std::string,std::string> blinkSS(std::string stone, std::unordered_map<std::string,std::pair<std::string,std::string>>& cache) {
  if (cache.find(stone)!=cache.end()) {
    return cache[stone];
  }
  std::pair<std::string,std::string> newStone {stoneValue(stone)};
  cache[stone] = newStone;
  return newStone;
}

std::pair<std::string,std::string> stoneValue(std::string stone) {
  std::pair<std::string,std::string> newStone {"-1","-1"};
  if (stone=="0") newStone.first = "1"; // 0 -> 1
  else if (stone.size()%2==0) { // split into two strings
    std::string ns1 {stone.substr(0,stone.size()/2)}, ns2{stone.substr(0+stone.size()/2)};
    newStone.first = std::to_string(std::stoll(ns1));
    newStone.second = std::to_string(std::stoll(ns2));
  } else {
    newStone.first = std::to_string( 2024LL * std::stoll(stone) ); // can't split, multiply by 2024
  }
  return newStone;
}

std::unordered_map<std::string,long long> blink (std::unordered_map<std::string,long long> stone_dict, std::unordered_map<std::string,std::pair<std::string,std::string>>& cache) {
  std::unordered_map<std::string,long long> new_stone_dict {};
  for (const auto& kv_pair : stone_dict) {
    std::pair<std::string,std::string> new_stones {blinkSS(kv_pair.first,cache)};
    new_stone_dict[new_stones.first] += kv_pair.second;
    if (new_stones.second!="-1") new_stone_dict[new_stones.second] += kv_pair.second;
  }
  return new_stone_dict;
}

