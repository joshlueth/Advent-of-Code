#include "Day9.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <algorithm>
#include <unordered_map>

bool sz_max(std::pair<long long,std::pair<std::size_t,std::size_t>>& a, std::pair<long long,std::pair<std::size_t,std::size_t>>& b) {
  return a.first>b.first;
}

// bool tx_sort(std::pair<int,int>& a, std::pair<int,int>& b) {
//   return a.first<b.first;
// }
// bool ty_sort(std::pair<int,int>& a, std::pair<int,int>& b) {
//   return a.second<b.second;
// }

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
  long long part1{0}, part2{0};

  std::vector<int> tilesx{},tilesy{};

	while (std::getline(inputFile,inputStr))
	{
    std::size_t pos {inputStr.find_first_of(',')};
    tilesx.push_back(std::stoi(inputStr.substr(0,pos)));
    tilesy.push_back(std::stoi(inputStr.substr(pos+1)));
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

  std::vector<std::pair<long long,std::pair<std::size_t,std::size_t>>> in_largest(tilesx.size()*(tilesx.size()-1)/2);
  std::size_t countertmp = 0;
  for (std::size_t ii=0; ii<tilesx.size(); ii++) {
    for (std::size_t jj=ii+1; jj<tilesx.size(); jj++) {
      long long sz = static_cast<long long>(std::abs(tilesx[ii]-tilesx[jj])+1)*static_cast<long long>(std::abs(tilesy[ii]-tilesy[jj])+1);
      in_largest[countertmp++]=std::make_pair(sz,std::make_pair(ii,jj));
    }
  } 
  std::sort(in_largest.begin(),in_largest.end(),sz_max);
  part1 = in_largest[0].first;

  auto t3 {std::chrono::high_resolution_clock::now()};

  // get list of sorted coordinates
  std::vector<int> txs {tilesx}, tys {tilesy};
  std::sort(txs.begin(),txs.end());
  std::sort(tys.begin(),tys.end());

  // calculate coordinate compression
  std::unordered_map<int,int> xmap {}, ymap {};
  int counter=0, recent=-1;
  for (std::size_t ii=0;ii<txs.size();ii++) {
    if (txs[ii]==recent) continue;
    xmap[txs[ii]] = 2*counter++;
    recent = txs[ii];
  }
  counter = 0; recent = -1;
  for (std::size_t jj=0;jj<tys.size();jj++) {
    if (tys[jj]==recent) continue;
    ymap[tys[jj]] = 2*counter++;
    recent = tys[jj];
  }

  std::size_t xsz {2*xmap.size()-1}, ysz {2*ymap.size()-1};
  std::vector<std::vector<char>> grid(xsz,std::vector<char>(ysz,'.'));
  for (std::size_t xt=0, yt=0; xt<tilesx.size(); xt++, yt++) {
    int xactual {tilesx[xt]}, yactual {tilesy[yt]};
    std::size_t gx {static_cast<std::size_t>(xmap[xactual])}, gy {static_cast<std::size_t>(ymap[yactual])};
    int xnext, ynext;
    if (xt==tilesx.size()-1) {
      xnext = tilesx[0]; ynext = tilesy[0];
    } else {
      xnext = tilesx[xt+1]; ynext = tilesy[yt+1];
    }
    std::size_t gxn {static_cast<std::size_t>(xmap[xnext])}, gyn {static_cast<std::size_t>(ymap[ynext])};
    for (std::size_t startx=std::min(gx,gxn); startx<=std::max(gx,gxn); startx++) {
      for (std::size_t starty=std::min(gy,gyn); starty<=std::max(gy,gyn); starty++) {
        grid[startx][starty] = '#';
      }
    }
    // grid[gx][gy] = 'O';
    // grid[gxn][gyn] = 'O';
  }

  std::vector<std::pair<std::size_t,std::size_t>> inside {};
  // only look at interior points
  for (std::size_t printy=1; printy<grid[0].size()-1; printy++) {
    for (std::size_t printx=1; printx<grid.size()-1; printx++) {
      if (grid[printx][printy]=='#') continue;
      std::vector<char> subvl(grid[printx-1].begin(),grid[printx-1].begin()+static_cast<long long>(printy));
      std::vector<char> subv(grid[printx].begin(),grid[printx].begin()+static_cast<long long>(printy));
      std::vector<char> subvh(grid[printx+1].begin(),grid[printx+1].begin()+static_cast<long long>(printy));
      // count the number of '#' in subv
      // if odd, add point to inside
      int countLine=0, LineDir=0; //, countO=0;
      for (std::size_t across=0; across<subv.size(); across++) {
        if (subv[across]=='#') {
          if (subvl[across]=='#'&&subvh[across]=='#') countLine++;
          else if (subvl[across]=='#') {
            if (LineDir==-1) LineDir=0;
            else if (LineDir==0) LineDir=-1;
            else {
              countLine++; LineDir=0;
            }
          } else if (subvh[across]=='#') {
            if (LineDir==1) LineDir=0;
            else if (LineDir==0) LineDir=1;
            else {
              countLine++; LineDir=0;
            }
          }
        }
      }
      if (countLine%2==1) inside.push_back(std::make_pair(printx,printy));
    }
  }

  // file in interior points
  for (const auto& entry : inside) {
    grid[entry.first][entry.second] = '#';
  }

  // check interior bounding boxes
  bool interior=false;
  while (!interior) {
    for (const auto& box : in_largest) {
      std::size_t ind1 {box.second.first}, ind2 {box.second.second};
      int indtx1 {tilesx[ind1]}, indtx2 {tilesx[ind2]}, indty1 {tilesy[ind1]}, indty2 {tilesy[ind2]};
      int indlx1 {xmap[indtx1]}, indlx2 {xmap[indtx2]}, indly1 {ymap[indty1]}, indly2 {ymap[indty2]};
      // std::cout << "overall index " << indtx1 << " " << indtx2 << " " << indty1 << " " << indty2 << " " << box.first << "\n";
      // std::cout << "local index " << indlx1 << " " << indly1 << " " << indlx2 << " " << indly2 << "\n";
      for (std::size_t xc=static_cast<std::size_t>(std::min(indlx1,indlx2)); xc<=static_cast<std::size_t>(std::max(indlx1,indlx2)); xc++) {
        for (std::size_t yc=static_cast<std::size_t>(std::min(indly1,indly2)); yc<=static_cast<std::size_t>(std::max(indly1,indly2)); yc++) {
          if(grid[xc][yc]=='.') {
            // std::cout << "has dot: " << xc << " " << yc << "\n";
            goto next_loop;
          }
        }
      }
      interior = true;
      part2 = static_cast<long long>(std::abs(indtx1-indtx2)+1)*static_cast<long long>(std::abs(indty1-indty2)+1);
      next_loop:
      if (interior) break;
    }
  }

  // output grid to terminal (or text file with redirection)
  // for (std::size_t printx = 0; printx<grid.size(); printx++) {
  //   for (std::size_t printy = 0; printy<grid[0].size(); printy++) {
  //     std::cout << grid[printx][printy];
  //   }
  //   std::cout << "\n";
  // }

  auto t4 {std::chrono::high_resolution_clock::now()};

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time:               " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t1).count() << "\n";
	std::cout << "  Reading in Input File:    " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving (Part 1): " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";
	std::cout << "  Problem Solving (Part 2): " << std::chrono::duration_cast<std::chrono::microseconds>(t4-t3).count() << "\n";

  std::cout << "\nPart 1: " << part1 << "\n";
  std::cout << "Part 2: " << part2 << "\n";

	return 0;
}
