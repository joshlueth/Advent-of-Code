#pragma once
#include <vector>
#include <iterator>
#include <utility>
#include <map>


std::size_t Dijkstra(const std::vector<std::vector<char>>& , std::pair<std::size_t,std::size_t> , std::pair<std::size_t,std::size_t> , const char , const char );

std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>::iterator
  it_inQueue(std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>& , std::pair<std::size_t,std::size_t> );

bool inBounds (int , int , int );

void pathTrace (std::vector<std::vector<char>> , const std::vector<std::vector<std::pair<std::size_t,std::size_t>>>& , const std::pair<std::size_t,std::size_t> );

bool check_righttop(const std::vector<std::vector<char>>& , const std::pair<std::size_t,std::size_t> );
