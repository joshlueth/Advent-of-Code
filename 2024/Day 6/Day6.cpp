#include "Day6.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>

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
	int count {1}, numO{0};
	map[row][col] = 'X';
	row -= 1;
	int nrows {map.size()}, ncols {map[0].size()};
	char dir {'u'};
	while (!isOut(row,nrows-1) && !isOut(col,ncols-1)) {
		char* current {&map[row][col]};
		switch (*current) {
			case '#': {
				rotate(row,col,dir);
				break;
			}
			case '.': {
				count += 1;
				*current = 'X';
				numO += checkObstruction(map,row,col,dir);	
				break;		
			}
			case 'X': {
				break;
			}

			default: {
				std::cerr << "Not a valid map character: " << *current << "\n";
			}
		}
		move(row,col,dir);
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

int checkObstruction (std::vector<std::vector<char>> map, int row, int col, char dir) {
	// add obstacle
	map[row][col] = '#';
	int nrows {map.size()}, ncols {map[0].size()};
	int rotateCount {0};
	while (!isOut(row,nrows-1) && !isOut(col,ncols-1)) {
		char* current {&map[row][col]};
		switch (*current) {
			case 'R':{
				return 1;
			}
			case '#': {
				rotate(row,col,dir);
				if (map[row][col]==dir) {
					return 1;
				}
				map[row][col] = dir;
				rotateCount += 1;
				break;
			}
			default: {
				rotateCount = 0;
			}
		}
		if (rotateCount==2) map[row][col] = 'R';
					// char store {map[row][col]};
					// map[row][col] = '^';
					// for (auto i : map) {
					// 	for (auto j : i) {
					// 		std::cout << j;
					// 	}
					// 	std::cout << "\n";
					// }
					// std::cout << "\n";
					// map[row][col] = store;
		move(row,col,dir);
	}
	return 0;
}