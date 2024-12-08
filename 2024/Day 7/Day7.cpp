#include "Day7.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector> 
#include <chrono>
#include <cmath>
#include <algorithm>

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

	std::vector<std::vector<long long>> nums {};
	std::vector<long long> end {};
	while (std::getline(inputFile,inputStr))
	{
		std::stringstream ss {inputStr};
		char delimiter {':'};
		std::string oNum {};
		std::getline(ss,oNum,delimiter);
// std::cout << oNum << " " << std::stoull(oNum) << "\n";
		end.push_back(std::stoull(oNum));
		std::vector<long long> tmp {};
		while(std::getline(ss,oNum,' ')) {
			if (oNum=="") continue;
// std::cout << oNum << " " << std::stoi(oNum) << "\n";
			tmp.push_back(std::stoull(oNum));
		}
		nums.push_back(tmp);
	}

	inputFile.close();

	auto t1 = std::chrono::high_resolution_clock::now();
	long long sum {0};
	for (std::size_t ii {0}; ii<end.size(); ++ii) {
		long long result {end[ii]};
		if(recursion(result,nums[ii])) {
			sum+=result;
		}
	}
	auto t2 = std::chrono::high_resolution_clock::now();
	long long sum2p2{0};	
	int maxt{0};
	for (std::size_t ii {0}; ii<end.size(); ++ii) {
		long long result {end[ii]};
		long long tmp{0};
		auto tb = std::chrono::high_resolution_clock::now();
		if(bruteForce(result,tmp,nums[ii])) {
			sum2p2+=result;
		}
		auto ta = std::chrono::high_resolution_clock::now();
		auto t_int = std::chrono::duration_cast<std::chrono::microseconds>(ta-tb);
		maxt = std::max(maxt,int(t_int.count()));
	}
	auto t3 = std::chrono::high_resolution_clock::now();
	std::cout << "The sum of the correct test values is: " << sum << "\n";
	std::cout << "This goes to: " << sum2p2 << " when we include ||" << "\n";
	std::cout << "In " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()
						<< " and " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << " microseconds\n";
	std::cout << "The maximum time it took for a single line for part 2 was " << maxt << " microseconds\n";
	return 0;
}

bool bruteForce(long long result, long long tmp, std::vector<long long>lst) {
	if (lst.size()==0) {
		if (result==tmp) return 2;
		return 0;
	}
	bool success{};
	std::vector<long long> nlst(lst.begin()+1,lst.end());
	if (tmp==0) {
		success = bruteForce(result,lst[0],nlst);
		return success;
	}
	tmp += lst[0];

	success = bruteForce(result,tmp,nlst);
	if (!success) {
		tmp -= lst[0];
		tmp *= lst[0];
		success = bruteForce(result,tmp,nlst);
	}
	if (!success) {
		tmp /= lst[0];
		tmp = concatenate(tmp,lst[0]);
		success = bruteForce(result,tmp,nlst);
	}
	return success;
}

// no point in doing backward recursion for part 2 because you'd have to evaluate the entire left half...
bool recursion(long long result, std::vector<long long> lst) {
	if (lst.size()==1) {
		if (result==lst[0]) return true;
		return false;
	}
	int indicator {0};
	indicator = checkMultiply(result,lst.back());
	if (indicator==0) {
		subtract(result,lst.back());
	}
	bool success {};
	std::vector<long long> nlst (lst.begin(),lst.end()-1);
	success = recursion(result,nlst);
	if(success==0 && indicator!=0) {
		result *= lst.back();
		subtract(result,lst.back());
		success = recursion(result,nlst);
	}
	return success;
}

int checkMultiply(long long &result, long long cnum){
	if (result%cnum!=0) return 0;
	result /= cnum;
	return 1;
}

void subtract(long long &result, long long cnum){
	result -= cnum;
}

long long concatenate(long long n1, long long n2) {
	return n1*pow(10,int(log10(n2)+1))+n2;
}
