#include "Day22.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <map>
#include <algorithm>
#include <set>

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
  std::vector<long long> iniSecret {};
	while (std::getline(inputFile,inputStr))
	{
    iniSecret.push_back(std::stoll(inputStr));
	}

	inputFile.close();

  // for (auto sn : iniSecret) {
  //   std::cout << sn << "\n";
  // }

	auto t2 {std::chrono::high_resolution_clock::now()};

  int loops {2000};
  std::map<std::vector<int>,long long> seq {};
  for (std::size_t sn {0}; sn<iniSecret.size(); ++sn) {
    std::vector<int> prices (4,0);
    int current_price {price(iniSecret[sn])}, old_price {0}, price_change {0};
    std::set<std::vector<int>> seq_lst {};
    for (int lp {0}; lp<loops; ++lp) {
      evolve(iniSecret[sn]);

      old_price = current_price;
      current_price = price(iniSecret[sn]);
      price_change = current_price - old_price;
      std::rotate(prices.begin(), prices.begin()+1, prices.end());
      prices[3] = price_change;
      // lp: 0, first price change (pc); 1, second pc; 2, third pc; 3, fourth pc - finally have a sequence
      if (lp>=3 && seq_lst.find(prices)==seq_lst.end()) {
        seq[prices] += current_price;
        seq_lst.insert(prices);
        // std::cout << prices[0] << " " << prices[1] << " " << prices[2] << " " << prices[3] << " " << current_price << "\n";
      }
    }
  }

  long long sum {0};
  for (std::size_t sn {0}; sn<iniSecret.size(); ++sn) {
    // std::cout << sn << "\n";
    sum += iniSecret[sn];
  }

  long long bananas {0};
  for (auto entry : seq) {
    if (entry.second > bananas) {
      bananas = entry.second;
      // for (auto ch : entry.first) {
      //   std::cout << ch << " ";
      // }
      // std::cout << "\n";
    }
  }

  auto t3 {std::chrono::high_resolution_clock::now()};



  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Sum of 2000th Secret Numbers is: " << sum << "\n";
  std::cout << "Maximum Number of Bananas is: " << bananas << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


	return 0;
}

long long evolve (long long& secret_num) {
  long long new_num {secret_num * 64LL};
  secret_num = mix(secret_num, new_num);
  secret_num = prune(secret_num);
  
  new_num = secret_num / 32LL;
  secret_num = mix(secret_num, new_num);
  secret_num = prune(secret_num);

  new_num = secret_num * 2048LL;
  secret_num = mix(secret_num, new_num);
  secret_num = prune(secret_num);

  return secret_num;
}

long long mix (long long num, long long new_num) {
  return num ^ new_num;
}

long long prune (long long num) {
  return num % 16777216;
}

int price(long long num) {
  return std::to_string(num).back() - '0';
}