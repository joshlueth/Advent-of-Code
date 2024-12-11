#pragma once
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

long long numStones(std::vector<std::string>, int);
std::pair<std::string,std::string> blinkSS(std::string, std::unordered_map<std::string,std::pair<std::string,std::string>>&);
std::pair<std::string,std::string> stoneValue(std::string);
std::unordered_map<std::string,long long> blink (std::unordered_map<std::string,long long>, std::unordered_map<std::string,std::pair<std::string,std::string>>&);


