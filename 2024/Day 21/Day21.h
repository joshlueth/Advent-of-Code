#pragma once
#include <string>
#include <utility>

int buttonPresses(std::string );
int firstKP(char , char );
int secondKP(std::pair<int,int> , std::pair<int,int> );
int thirdKP(std::string , char );
int myKP(std::string );
std::pair<int,int> buttonKPcoords(char );
char coordsKPbutton(std::pair<int,int> );
bool isbuttonKPavoid (std::pair<int,int> );
std::pair<int,int> directionalKPcoords(char );
char coordsKPdirectional(std::pair<int,int> );
bool isdirectionalKPavoid (std::pair<int,int> );
int factorial(int );
char movement2key(int , char );
int L1_distance(std::pair<int,int> , std::pair<int,int> );
bool getNumPath(std::string& , std::pair<int,int> , std::pair<int,int> , int , int );
bool getDirPath(std::string& , std::pair<int,int> , std::pair<int,int> , int , int );
