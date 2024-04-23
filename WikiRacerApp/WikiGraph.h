#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <limits>

using namespace std;

class WikiGraph {
    class Wiki {
    public:
        std::string name;
        std::vector<Wiki*> outWikis;
        Wiki(std::string &name_) {
            name = name_;
        }
    };
public:
    unordered_map<string, Wiki*> map; // mapping all wikis to their wiki objects

    void load_data(string fileName); // loads data from txt
    long str_to_int(string str); // home-made stoi

    // use dijkstra's algorithm to find the distance from one article to another
    // returns distance 
    unsigned int dijkstra(const std::string& start, const std::string& end);

    vector<Wiki*> bfs(Wiki* start, Wiki* end); // returns shortest bfs path to node
    std::string getSmallestDistance(std::unordered_map<std::string, std::pair<unsigned int, bool>>& nodes);
    unsigned int getDistToEnd(const std::string& end, std::unordered_map<std::string, std::pair<unsigned int, bool>>& nodes);

};
