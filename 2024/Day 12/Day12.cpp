#include "Day12.h"
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <set>
#include <utility>
#include <tuple>
#include <array>

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
  std::vector<std::vector<char>> map{};
	while (std::getline(inputFile,inputStr))
	{
    std::vector<char> intoArray(inputStr.begin(),inputStr.end());
    map.push_back(intoArray);
	}

	inputFile.close();

	auto t2 {std::chrono::high_resolution_clock::now()};

// okay so the plan of attack is simple
// create a boolean array of equal size checking if we have already gone through that cell
// then go through each element in map
// if already traversed, keep going
// if not, create regionA with 1 and regionP with 4 and enter the recursive traversal of all neighboring cells
// when recursively going into a neighboring cell
  // add 1 to area and 4 to perimeter
  // of a neighboring type and traversed, decrease perimeter count by 1 and return
  // not of a neighboring type, return
  // of a neighboring type and not traversed, add to traverse queue (which is a set)

// The question at hand is then HOW on earth am I supposed to identify the number of sides for a given shape?
// Look at Euler Characteristic - edges=vertices for 2d planar polygon with any number of polygonal holes

  std::pair<int,int> apv {std::make_pair(0,0)};
  std::vector<std::vector<bool>> traversed(map.size(),std::vector<bool>(map[0].size(),false));
  for (std::size_t rw {0}; rw<map.size(); ++rw) {
    for (std::size_t ch {0}; ch<map[0].size(); ++ch) {
      if (traversed[rw][ch]) continue;
      std::pair<int,int> apv_update {};
      apv_update = region(map,traversed,rw,ch);
      apv.first += apv_update.first;
      apv.second += apv_update.second;
    } 
  }

  auto t3 {std::chrono::high_resolution_clock::now()};

  std::cout << "Sum of Area multiplied by Perimeter for all regions: " << apv.first << "\n";
  std::cout << "Sum of Area multiplied by Number of Sides for all regions: " << apv.second << "\n";

	std::cout << "Program took, in microseconds:" << "\n";
	std::cout << "  Total Time: " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t1).count() << "\n";
	std::cout << "  Reading in File: " << std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count() << "\n";
	std::cout << "  Problem Solving: " << std::chrono::duration_cast<std::chrono::microseconds>(t3-t2).count() << "\n";


	return 0;
}

std::pair<int,int> region(const std::vector<std::vector<char>>& map, std::vector<std::vector<bool>>& traversed, std::size_t rw, std::size_t ch ) {
  std::tuple<int,int,int> apv {std::make_tuple(0,0,0)};
  std::set<std::pair<std::size_t,std::size_t>> queue={std::make_pair(rw,ch)};
  while (!queue.empty()){
    cell(map,traversed,queue,apv);
  }
  return std::make_pair(std::get<0>(apv)*std::get<1>(apv),std::get<0>(apv)*std::get<2>(apv));
}

void cell(const std::vector<std::vector<char>>& map, std::vector<std::vector<bool>>& traversed,
          std::set<std::pair<std::size_t,std::size_t>>& queue, std::tuple<int,int,int>& apv) {
// by definition, have not traversed a cell in the queue, and it is of the same region
  auto cl = *queue.begin();
  std::array<bool,4> drul {dirInBounds(map,cl.first,cl.second)}; // check if down/right/up/left are in bounds
  std::get<0>(apv)+=1; // add area
  std::get<1>(apv)+=4; // add initial perimeter

  // do perimeter checks
  if (drul[0]) { // down
    if (map[cl.first+1][cl.second]==map[cl.first][cl.second]) {
      std::get<1>(apv)-=1;
      if (!traversed[cl.first+1][cl.second]) {
        queue.insert(std::make_pair(cl.first+1,cl.second));
      }
    }
  }
  if (drul[1]) { // right
    if (map[cl.first][cl.second+1]==map[cl.first][cl.second]) {
      std::get<1>(apv)-=1;
      if (!traversed[cl.first][cl.second+1]) {
        queue.insert(std::make_pair(cl.first,cl.second+1));
      }
    }    
  } 
  if (drul[2]) { // up
    if (map[cl.first-1][cl.second]==map[cl.first][cl.second]) {
      std::get<1>(apv)-=1;
      if (!traversed[cl.first-1][cl.second]) {
        queue.insert(std::make_pair(cl.first-1,cl.second));
      }
    }
  }  
  if (drul[3]) { // left
    if (map[cl.first][cl.second-1]==map[cl.first][cl.second]) {
      std::get<1>(apv)-=1;
      if (!traversed[cl.first][cl.second-1]) {
        queue.insert(std::make_pair(cl.first,cl.second-1));
      }
    }
  }

  // do vertex checks
  char ch, chrw, chcl, chop {};
  getChar('d',drul[0],'r',drul[1],ch,chrw,chcl,chop,map,cl.first,cl.second); // down-right vertex (++)
  if(vFilter(ch,chrw,chcl,chop)) std::get<2>(apv)+=1;
  getChar('u',drul[2],'r',drul[1],ch,chrw,chcl,chop,map,cl.first,cl.second); // up-right vertex (-+)
  if(vFilter(ch,chrw,chcl,chop)) std::get<2>(apv)+=1;
  getChar('u',drul[2],'l',drul[3],ch,chrw,chcl,chop,map,cl.first,cl.second); // up-left vertex (--)
  if(vFilter(ch,chrw,chcl,chop)) std::get<2>(apv)+=1;
  getChar('d',drul[0],'l',drul[3],ch,chrw,chcl,chop,map,cl.first,cl.second); // down-left vertex (+-)
  if(vFilter(ch,chrw,chcl,chop)) std::get<2>(apv)+=1;

  traversed[cl.first][cl.second] = true; // set traversed to true
  queue.erase(cl); // remove traversed cell from queue
}

std::array<bool,4> dirInBounds (const std::vector<std::vector<char>>& map, const std::size_t rw, const std::size_t ch) {
  std::array<bool,4> drul {false};
  if (rw+1<map.size())    drul[0] = true; // check down
  if (ch+1<map[0].size()) drul[1] = true; // check right
  if (rw>0)               drul[2] = true; // check up
  if (ch>0)               drul[3] = true; // check left
  return drul; // return down|right|up|left
}

bool vFilter (const char cur, const char cur_rw, const char cur_ch, const char cur_op) {
  if (cur==cur_rw && cur==cur_ch && cur!=cur_op) {
    return true;  
  }
  if (cur!=cur_rw && cur!=cur_ch) {
    return true;   
  }
  return false;
}


void getChar(const char d1, const bool d1exist, const char d2, const bool d2exist, char &ch, char &chrw , char &chcl, char &chop,
             const std::vector<std::vector<char>>& map, std::size_t rw, std::size_t cl) {
  ch = map[rw][cl];
  if (d1exist) {
    if (d1=='d') chrw = map[rw+1][cl];
    if (d1=='u') chrw = map[rw-1][cl];
  } else chrw = '0';
  if (d2exist) {
    if (d2=='r') chcl = map[rw][cl+1];
    if (d2=='l') chcl = map[rw][cl-1];
  } else chcl = '0';
  if (d1exist && d2exist) {
    if (d1=='d') {
      if (d2=='r') {
        chop = map[rw+1][cl+1];
      } else if (d2=='l') {
        chop = map[rw+1][cl-1];
      }
    } else if (d1=='u') {
      if (d2=='r') {
        chop = map[rw-1][cl+1];
      } else if (d2=='l') {
        chop = map[rw-1][cl-1];        
      }
    }
  } else chop = '0';
}


