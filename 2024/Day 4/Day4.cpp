#include "Day4.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main()
{
	std::string fileName {"input.txt"}; // note that using sample.txt should give an answer of 18

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	// put data in 2d character vector v
	std::string inputStr {};
	std::vector<std::vector<char>> v {};
	while (std::getline(inputFile,inputStr))
	{
		std::vector<char> intoArray (inputStr.begin(), inputStr.end());
		v.push_back(intoArray);
	}

	inputFile.close();	

	int xmas {}, x_mas{};
	xmas = numXMAS(v);
	x_mas = numX_MAX(v);

	std::cout << "Number of XMAS in data set: " << xmas << "\n";
	std::cout << "Number of X-MAS in data set: " << x_mas << "\n";

	return 0;
}

int numXMAS(std::vector<std::vector<char>> v)
{
	int xmas {0};
	int num_rows {v.size()}, num_cols {v[0].size()};
// std::cout << "before ";
// std::cout << num_rows << " " << num_cols << "\n";
	for (int ii{0}; ii<num_rows; ++ii) {
// std::cout << "in 1 ";
		for (int jj{0}; jj<num_cols; ++jj) {
// std::cout << "in 2 " << "\n";
			if (v[ii][jj]=='X') {
// std::cout << ii << " " << jj << "\n";
				std::size_t numberDirections {8};
				std::vector<int> iDir {1,1,0,-1,-1,-1,0,1}, jDir {0,1,1,1,0,-1,-1,-1};
				for (std::size_t dir{0}; dir<numberDirections; ++dir) {
					if (inBounds(ii+3*iDir[dir],0,num_rows-1) && inBounds(jj+3*jDir[dir],0,num_cols-1)) {
						if (v[ii+iDir[dir]][jj+jDir[dir]] == 'M') {
// std::cout << ii+iDir[dir] << " " << jj+jDir[dir] << "\n";
							if (v[ii+2*iDir[dir]][jj+2*jDir[dir]] == 'A') {
// std::cout << ii+2*iDir[dir] << " " << jj+2*jDir[dir] << "\n";
								if (v[ii+3*iDir[dir]][jj+3*jDir[dir]] == 'S') {
// std::cout << ii+3*iDir[dir] << " " << jj+3*jDir[dir] << "\n";
									xmas += 1;
								}
							}
						}
					}
				}
			}
		}
	}

	return xmas;
}

bool inBounds(int val, int lower, int upper)
{
	if (val<=upper && val>=lower) return true;
	return false;
}