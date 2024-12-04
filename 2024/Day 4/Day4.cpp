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
	x_mas = numX_MAS(v);

	std::cout << "Number of XMAS in data set: " << xmas << "\n";
	std::cout << "Number of X-MAS in data set: " << x_mas << "\n";

	return 0;
}

int numX_MAS(std::vector<std::vector<char>> v)
{
	int xmas {0};
	int num_rows {v.size()}, num_cols {v[0].size()};
	for (int ii{1}; ii<num_rows-1; ++ii) {
		for (int jj{1}; jj<num_cols-1; ++jj) {
			if (v[ii][jj]=='A') {
				std::vector<int> iDir {1,-1,1,-1}, jDir {1,-1,-1,1};
				std::size_t dir {0};

				// dir0 is M or S, dir1 is opposite
				// dir2 is M or S, dir3 is opposite
				if ( v[ii+iDir[dir]][jj+jDir[dir]] == 'M' || v[ii+iDir[dir]][jj+jDir[dir]] == 'S' ) {
					dir = 1;
					if ( (v[ii+iDir[dir]][jj+jDir[dir]] == 'M' || v[ii+iDir[dir]][jj+jDir[dir]] == 'S') && (v[ii+iDir[dir]][jj+jDir[dir]] != v[ii+iDir[dir-1]][jj+jDir[dir-1]]) ) {
						dir = 2;
						if ( v[ii+iDir[dir]][jj+jDir[dir]] == 'M' || v[ii+iDir[dir]][jj+jDir[dir]] == 'S' ) {
							dir = 3;
							if ( (v[ii+iDir[dir]][jj+jDir[dir]] == 'M' || v[ii+iDir[dir]][jj+jDir[dir]] == 'S') && (v[ii+iDir[dir]][jj+jDir[dir]] != v[ii+iDir[dir-1]][jj+jDir[dir-1]]) ) {
								xmas += 1;
							}
						}
					}
				}

			}
		}
	}

	return xmas;
}

int numXMAS(std::vector<std::vector<char>> v)
{
	int xmas {0};
	int num_rows {v.size()}, num_cols {v[0].size()};
	for (int ii{0}; ii<num_rows; ++ii) {
		for (int jj{0}; jj<num_cols; ++jj) {
			if (v[ii][jj]=='X') {
				std::size_t numberDirections {8};
				std::vector<int> iDir {1,1,0,-1,-1,-1,0,1}, jDir {0,1,1,1,0,-1,-1,-1};
				for (std::size_t dir{0}; dir<numberDirections; ++dir) {
					if (inBounds(ii+3*iDir[dir],0,num_rows-1) && inBounds(jj+3*jDir[dir],0,num_cols-1)) {
						if (v[ii+iDir[dir]][jj+jDir[dir]] == 'M') {
							if (v[ii+2*iDir[dir]][jj+2*jDir[dir]] == 'A') {
								if (v[ii+3*iDir[dir]][jj+3*jDir[dir]] == 'S') {
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