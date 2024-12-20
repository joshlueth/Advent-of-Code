#pragma once
#include <vector>
#include <iterator>
#include <utility>
#include <map>


std::vector<std::vector<int>> Dijkstra(const std::vector<std::vector<char>>& , std::pair<std::size_t,std::size_t> , std::pair<std::size_t,std::size_t> , const char , const char );

std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>::iterator
  it_inQueue(std::multimap<std::size_t,std::pair<std::size_t,std::size_t>>& , std::pair<std::size_t,std::size_t> );

void pathTrace (std::vector<std::vector<char>> , const std::vector<std::vector<std::pair<std::size_t,std::size_t>>>& , const std::pair<std::size_t,std::size_t> );

std::pair<std::size_t,std::size_t> findChar(const std::vector<std::vector<char>>& , const char );

int findCheats(const std::vector<std::vector<char>>& , const std::vector<std::vector<int>>& , const std::vector<std::vector<int>>& , 
               const int , const int , const int );
