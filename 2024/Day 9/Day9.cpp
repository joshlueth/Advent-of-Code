#include "Day9.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <numeric>
#include <algorithm>


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
// one line, only numbers
	std::getline(inputFile,inputStr);
	inputFile.close();
  
// split into files and freespace
  std::string files {}, freespace{};
  for (std::size_t ii = {0}; ii < inputStr.size(); ii+=2) {
    files += inputStr[ii];
    if (ii+1 <inputStr.size()) freespace += inputStr[ii+1];
    else freespace += '0';
  }

	auto t2 {std::chrono::high_resolution_clock::now()};

// a little complicated here:
// alterate pulling from the back and front of files
  // place 0 {files[0] times}
  // check the first freespace value
  // place end {files[end] times}
    // if {files[end]} is greater than the value of the freespace, figure out how much is left
    // if it is less than the value of the freespace, figure out how many more space are left

// distinguish between ID (index of file) and Num (amount of that index)
  std::vector<int> compacted {};
  std::size_t ff {0}, fb {files.size()-1}, fs {0}; // filesforward, filesbackward, and freespace
  int fID {}, bID{}, fNum{}, fsNum{}, bTrack{};
  bTrack = std::stoi(std::string(1,files[fb]));
  while (ff<fb) {
  // current file
    fID = int(ff);
    fNum = std::stoi(std::string(1,files[ff]));
    for (int ii{0}; ii<fNum; ++ii) {
      compacted.push_back(fID);
    }
    ++ff;

  // fill in freespace
    bID = int(fb);
    fsNum = std::stoi(std::string(1,freespace[fs]));
    for (int ii{0}; ii<fsNum; ++ii) {
      if (bTrack==0) {
        --fb;
        if (ff==fb) break;
        --bID;
        bTrack = std::stoi(std::string(1,files[fb]));;
      }
      compacted.push_back(bID);
      --bTrack;
    }
    ++fs;
  }

  for (int ii{0}; ii<bTrack; ++ii) {
    compacted.push_back(int(fb));
  }

  long long sum1{0};
  for (std::size_t it{0}; it<compacted.size(); ++it) {
    sum1 += int(it)*compacted[it];
  }
	auto t3 {std::chrono::high_resolution_clock::now()};

// for each file, in reverse order, try to see if it fits in a file in forward order
// keep track of the key value through a std::vector<int>
// and after each attempt, adjust the freespace and keys

  std::vector<int> indices (files.size());
  std::vector<int> filesV {}, freespaceV {};
  for (auto ch : files) {
    filesV.push_back(std::stoi(std::string(1,ch)));
  }
  for (auto ch : freespace) {
    freespaceV.push_back(std::stoi(std::string(1,ch)));
  }
  std::vector<int> filesAdjust {filesV};
  std::iota (indices.begin(),indices.end(),0);
  for (std::size_t ii{filesV.size()-1}; ii>0; --ii) {
    int counter {int(indices.rend()-std::find(indices.rbegin(),indices.rend(),ii)-1)};
    for (std::size_t jj {0}; int(jj)<counter; ++jj) {
      if (freespaceV[jj]>=filesV[ii]) {
        // move everything...
        std::rotate(indices.begin()+int(jj)+1,indices.begin()+counter,indices.begin()+1+counter);
        std::rotate(filesAdjust.begin()+int(jj)+1,filesAdjust.begin()+counter,filesAdjust.begin()+1+counter);
        freespaceV[jj] = freespaceV[jj]-filesV[ii];
        freespaceV[counter-1] = freespaceV[counter-1] + freespaceV[counter] + filesV[ii];
        freespaceV.erase(freespaceV.begin()+counter);
        freespaceV.insert(freespaceV.begin()+int(jj),0);
        break;
      }
    }
  }

  std::vector<int> transfer{};
  for (std::size_t ii{0}; ii<filesAdjust.size(); ++ii) {
    for (std::size_t jj{0}; int(jj)<filesAdjust[ii]; ++jj) {
      transfer.push_back(indices[ii]);
    }
    for (std::size_t jj{0}; int(jj)<freespaceV[ii]; ++jj) {
      transfer.push_back(0);
    }
  }

  long long sum2{0};
  for (std::size_t it{0}; it<transfer.size(); ++it) {
    sum2 += int(it)*transfer[it];
  }
  auto t4 {std::chrono::high_resolution_clock::now()};

  std::cout << "Filesystem Checksum (Part 1) is: " << sum1 << "\n";
  std::cout << "Filesystem Checksum (Part 2) is: " << sum2 << "\n";
	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Preprocessing       : " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Part 1 (Fragment)   : " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Part 2 (Continguous): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

	return 0;
}
