#pragma once
#include <vector>
#include <set>
#include <utility>
#include <tuple>
#include <array>

std::pair<int,int> region(const std::vector<std::vector<char>>&, std::vector<std::vector<bool>>&, std::size_t, std::size_t);
void cell(const std::vector<std::vector<char>>&, std::vector<std::vector<bool>>&,
          std::set<std::pair<std::size_t,std::size_t>>&, std::tuple<int,int,int>&);
bool vFilter (const char, const char, const char, const char);
std::array<bool,4> dirInBounds (const std::vector<std::vector<char>>&, const std::size_t, const std::size_t);
void getChar(const char, const bool, const char, const bool, char &, char &, char &, char &,
             const std::vector<std::vector<char>>&, std::size_t, std::size_t);

