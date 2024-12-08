#include "Day8.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <chrono>

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
	std::vector<std::vector<char>> map {};
	while (std::getline(inputFile,inputStr))
	{
		std::vector<char> intoArray(inputStr.begin(),inputStr.end());
		map.push_back(intoArray);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

	std::set<char> keys {};
	std::unordered_multimap<char,std::pair<int,int>> nodes {};
	for (std::size_t rw {0}; rw<map.size(); rw++) {
		for (std::size_t cl{0}; cl<map[0].size(); cl++) {
			if (map[rw][cl]!='.') {
				keys.insert(map[rw][cl]);
				nodes.insert({map[rw][cl],std::make_pair(int(rw),int(cl))});
			}
		}
	}

	auto t3 {std::chrono::high_resolution_clock::now()};

	int szx{int(map.size())}, szy{int(map[0].size())};
	std::set<std::pair<int,int>> antinodes1{}, antinodes2{};
	for (const auto& k : keys) {
		auto range {(nodes.equal_range(k))};
		for (auto loc1{range.first}; loc1!=range.second; ++loc1) {
			for (auto loc2 {std::next(loc1)}; loc2!=range.second; ++loc2) {
				int x1{}, y1{}, x2{}, y2{}, dx{}, dy{};
				getInfo(x1, y1, x2, y2, dx, dy, loc1->second, loc2->second);
				an1(x1, y1, x2, y2, dx, dy, szx, szy, antinodes1);
				an2(x1, y1, x2, y2, dx, dy, szx, szy, antinodes2);
			}
		}
	}

  std::size_t antinodeNumber1 {std::size(antinodes1)};
	std::size_t antinodeNumber2 {std::size(antinodes2)};

	auto t4 {std::chrono::high_resolution_clock::now()};

 	std::cout << "Number of unique antinodes for part 1 is " << antinodeNumber1 << "\n";
 	std::cout << "Number of unique antinodes for part 2 is " << antinodeNumber2 << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Reading file         : " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Getting nodes, keys  : " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Antinode calculations: " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";
	std::cout << "For a total of " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";


	return 0;
}

bool inBounds(int val, int sz) {
	if (val>=0 && val <sz) return true;
	return false;
}

void an1(int x1, int y1, int x2, int y2, int dx, int dy, int szx, int szy, std::set<std::pair<int,int>>& ans) {
	if (inBounds(x1-dx,szx) && inBounds(y1-dy,szy)) ans.insert(std::make_pair(x1-dx,y1-dy));
	if (inBounds(x2+dx,szx) && inBounds(y2+dy,szy)) ans.insert(std::make_pair(x2+dx,y2+dy));
}

void an2(int x1, int y1, int x2, int y2, int dx, int dy, int szx, int szy, std::set<std::pair<int,int>>& ans) {
	int anx {}, any{};
	anx = x2;
	any = y2;
	// forward stepping
	while (inBounds(anx, szx) && inBounds(any,szy)) {
		ans.insert(std::make_pair(anx, any));
		anx += dx;
		any += dy;
	}
	anx = x1;
	any = y1;
	// backward stepping
	while (inBounds(anx, szx) && inBounds(any,szy)) {
		ans.insert(std::make_pair(anx, any));
		anx -= dx;
		any -= dy;
	}
} 

void getInfo(int& x1, int& y1, int& x2, int& y2, int& dx, int& dy, std::pair<int,int> loc1, std::pair<int,int> loc2) {
	x1 = loc1.first;
	y1 = loc1.second;
	x2 = loc2.first;
	y2 = loc2.second;
	dx = x2 - x1;
	dy = y2 - y1;
}
