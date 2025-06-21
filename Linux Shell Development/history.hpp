#ifndef HISTORY_HPP
#define HISTORY_HPP
#include <iostream>
#include <deque>
#include <fstream>
#define CACHE_SIZE  1000
#define HISTORY_CACHE  ".maincache"

enum DIREC
{
    UP,
    DOWN
};

class History
{
    std::deque<std::string> arr;
    int index;
public:
    History();
    ~History();
    void addHistory(const std::string __str);
    std::string getHistory(DIREC dir);
    void resetHistory();
    void updateHistory();
};
#endif