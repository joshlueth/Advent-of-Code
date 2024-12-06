#include "Day6.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

int main()
{
	std::string fileName {"input.txt"};

	std::ifstream inputFile {fileName};
	std::vector<std::vector<char>> map {};
	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::string inputStr {};

	while (std::getline(inputFile,inputStr))
	{
		std::vector<char> intoArray (inputStr.begin(), inputStr.end());
		map.push_back(intoArray);
	}

	inputFile.close();

// get index of current position (^) in row, col
	int row {0}, col {0};
	for (auto r : map) {
		auto it {std::find(r.begin(),r.end(),'^')};
		if (it!=r.end()) {
			col = std::distance(r.begin(),it);
			break;
		}
		row++;
	}

// set count, change '^' so we start at the correct point
	int count {0}, numO {0};
	map[row][col] = '.';
	int nrows {map.size()}, ncols {map[0].size()};
	char dir {'u'};
	while (!isOut(row,nrows-1) && !isOut(col,ncols-1)) {
		char* current {&map[row][col]};
		// std::cout << *current << " " << row << " " << col << " " << dir << "\n";
		switch (*current) {
			case '#': {
				rotate(row,col,dir);
				break;
			}
			case '.': {
				count += 1;
				*current = dir;
				break;
			}
			default: {
				// std::cerr << "Not a valid map character: " << *current << "\n";
			}
		}
		move(row,col,dir);

		// std::cout << count << "\n";
	}

	std::cout << "Number of Distinct Positions visited: " << count << "\n";
	std::cout << "Number of Obstruction locations: " << numO << "\n";

	return 0;
}

bool isOut (int index, int upper) {
	if (index >=0 && index <=upper) return false;
	return true;
}

void move (int &row, int &col, char dir) {
	switch (dir) {
		case 'd': {
			++row;
			break;
		}
		case 'r': {
			++col;
			break;
		}
		case 'u': {
			--row;
			break;
		}
		case 'l': {
			--col;
			break;
		}
		default: {
			std::cerr << "Given direction was not valid: " << dir << "\n";
		}
	}
}

void rotate (int &row, int &col, char &dir) {
		switch (dir) {
		case 'u': {
			dir = 'r';
			move(row,col,'d');
			break;
		}
		case 'r': {
			dir = 'd';
			move(row,col,'l');
			break;
		}
		case 'd': {
			dir = 'l';
			move(row,col,'u');
			break;		
		}
		case 'l': {
			dir = 'u';
			move(row,col,'r');
			break;
		}
		default: {
			std::cerr << "Given direction was not valid: " << dir << "\n";
		}
	}
}

