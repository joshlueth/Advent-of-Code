#pragma once
#include <vector>
#include <set>
#include <utility>

int region(const std::vector<std::vector<char>>&, std::vector<std::vector<bool>>&, std::size_t, std::size_t);
void cell(const std::vector<std::vector<char>>&, std::vector<std::vector<bool>>&,
          std::set<std::pair<std::size_t,std::size_t>>&, std::pair<int,int>&);