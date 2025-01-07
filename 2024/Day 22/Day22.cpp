#include "Day22.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <array>
#include <bitset>

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

  const std::size_t hsh_base {19};
  int loops {2000}; // 2001 secret numbers if we count the first one
  const std::size_t N {hsh_base*hsh_base*hsh_base*hsh_base};
  std::array<int,N> seq {0};
  for (std::size_t sn {0}; sn<iniSecret.size(); ++sn) {
    int current_price {price(iniSecret[sn])}, old_price {0}, old_price_change_m3 {0}, old_price_change_m2 {0}, old_price_change_m1 {0}, price_change {0};
    std::bitset<N> seq_lst {};
    seq_lst.set();
    for (int lp {0}; lp<loops; ++lp) {
      evolve(iniSecret[sn]);

      old_price = current_price;
      current_price = price(iniSecret[sn]);

      old_price_change_m3 = old_price_change_m2;
      old_price_change_m2 = old_price_change_m1;
      old_price_change_m1 = price_change;
      price_change = current_price - old_price;

      int hsh {sequence_hash(price_change, old_price_change_m1, old_price_change_m2, old_price_change_m3, hsh_base)};
      if (lp>=3 && seq_lst[hsh]) {
        seq_lst[hsh] = false;
        seq[hsh] += current_price;
      }
    }
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  long long sum {0};
  for (std::size_t sn {0}; sn<iniSecret.size(); ++sn) {
    sum += iniSecret[sn];
  }

  long long bananas {0};
  for (std::size_t entry {0}; entry < N; ++entry) {
    if (seq[entry] > bananas) bananas = seq[entry];
  }

  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Sum of 2000th Secret Numbers is: " << sum << "\n";
  std::cout << "Maximum Number of Bananas is: " << bananas << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:            " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File: " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving:       " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Answer Determining:    " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";


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

long long mix (const long long& num, const long long& new_num) {
  return num ^ new_num;
}

long long prune (const long long& num) {
  return num % 16777216;
}

int price(const long long& num) {
  return num % 10;
}

int sequence_hash(const int& pm1, const int& pm2, const int& pm3, const int& pm4, const std::size_t& hsh_base) {
  int hsh {};
  hsh += pm4 + 9;
  hsh += hsh_base * (pm3 + 9);
  hsh += hsh_base * hsh_base * (pm2 + 9);
  hsh += hsh_base * hsh_base * hsh_base * (pm1 + 9);
  return hsh;
}