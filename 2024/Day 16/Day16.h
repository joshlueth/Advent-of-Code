#pragma once
#include <vector>
#include <utility>
#include <tuple>
#include <map>

std::pair<std::size_t,std::size_t> findChar(const std::vector<std::vector<char>>& , const char );
void Dijkstra(std::vector<std::vector<std::vector<int>>>& , std::vector<std::vector<std::vector<bool>>>& , std::multimap<int,std::tuple<std::size_t,std::size_t,std::size_t>>& );
std::multimap<int, std::tuple<std::size_t,std::size_t,std::size_t>>::iterator 
  currentValue(std::tuple<std::size_t,std::size_t,std::size_t> , std::multimap<int,std::tuple<std::size_t,std::size_t,std::size_t>>& );
std::tuple<std::size_t,std::size_t,std::size_t> forward (std::tuple<std::size_t,std::size_t,std::size_t> );
std::tuple<std::size_t,std::size_t,std::size_t> CCW (std::tuple<std::size_t,std::size_t,std::size_t> );
std::tuple<std::size_t,std::size_t,std::size_t> CW (std::tuple<std::size_t,std::size_t,std::size_t> );
