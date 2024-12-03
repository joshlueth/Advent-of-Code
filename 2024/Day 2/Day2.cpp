#include "Day2.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <bits/stdc++.h>

int main()
{
	std::string fileName {"input.txt"};

	std::ifstream inputFile {fileName};

	if (!inputFile)
	{
		std::cerr << "Input file " << fileName << " could not be opened\n";
		return 1;
	}

	std::string inputStr {};
	int safe{0}, safeDamp{0}, edgeCase{0}, lineCounter{1};
	while (std::getline(inputFile,inputStr))
	{
		std::stringstream ss (inputStr);
		std::vector<int> list(std::istream_iterator<int>(ss),{});

		std::size_t length {list.size()};
		int increment{1}, incrementDamp{1}; 
		switch (length) {
			case 0: {
				edgeCase += 1;
				break;
			}

			case 1: {
				edgeCase += 1;
				break;
			}

			case 2: {
				int diff {list[1]-list[0]};
				if (abs(diff)>3)
				{
					increment = 0;
					incrementDamp = 0;
				}
				break;
			}

			case 3: {
				int diff1 {list[1]-list[0]}, diff2 {list[2]-list[1]}, diff3 {list[2]-list[0]};
				if (abs(diff1)<=3 && abs(diff2)<=3 && sign(diff1)==sign(diff2) && sign(diff1)!=0)
				{
					break;
				}
				else if ((abs(diff1)<=3 && sign(diff1)!=0) || (abs(diff2)<=3 && sign(diff2)!=0) || (abs(diff3)<=3 && sign(diff3)!=0))
				{
					increment = 0;
					break;
				}
				increment = 0;
				incrementDamp = 0;
				break;
			}

			default:
			{
				int vecDiffSign{0}, checking{0};
				std::size_t start{0};
				while (incrementDamp!=0)
				{
					checking = check(list[0],list[1],list[2]);
					if (checking!=0)
					{
						start = 2;
						vecDiffSign = checking;
						break;
					}
					else
					{
						increment = 0;
						checking = check(list[0],list[1],list[3]);
						if (checking != 0)
						{
							start = 3;
							vecDiffSign = checking;
							break;
						}
						checking = check(list[0],list[2],list[3]);
						if (checking != 0)
						{
							start = 3;
							vecDiffSign = checking;
							break;
						}
						checking = check(list[1],list[2],list[3]);
						if (checking != 0)
						{
							start = 3;
							vecDiffSign = checking;
							break;
						}
						incrementDamp = 0;
					}
				}
				// for (int i: list) std::cout << " " << i;
				// std::cout << " " << increment << incrementDamp << "\n";
				// if (incrementDamp==0) break;
				
				for (std::size_t i{start}; i<list.size()-1; ++i)
				{
					int diff {list[i+1]-list[i]};
					int diffSign {sign(diff)};
					if (diffSign!=vecDiffSign || abs(diff)>3)
					{
						if (increment==0)
						{
							incrementDamp = 0;
							break;
						}
						else{
							increment = 0;
							++i;
							if (i==list.size()-1)
							{
								break;
							}
							diff = list[i+1]-list[i-1];
							diffSign = sign(diff);
						if (diffSign!=vecDiffSign || abs(diff)>3)
						{
							incrementDamp = 0;
							break;
						}
						}
					}
				}
			}

		}
		// std::cout << lineCounter << " " << increment << incrementDamp;
		// for (int i: list) std::cout << " " << i;
		// std::cout << "\n";
		safe += increment;
		safeDamp += incrementDamp;
		lineCounter += 1;

	}

	inputFile.close();

	std::cout << "Number of Edge Cases with Length 0 or 1: " << edgeCase << "\n";
	std::cout << "Number of Safe Reports: " << safe << "\n";
	std::cout << "Number of Safe Reports After Damping: " << safeDamp << "\n";

	return 0;
}

int sign(int number)
{
	if (number>0) return 1;
	if (number<0) return -1;
	return 0;
}

int check(int x1, int x2, int x3)
{
	int diff1 {x2-x1}, diff2 {x3-x2};
	if (abs(diff1)<=3 && abs(diff2)<=3 && sign(diff1)==sign(diff2) && sign(diff1)!=0)
	{
		return sign(diff1);
	}
	return 0;
}

