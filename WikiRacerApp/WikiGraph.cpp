#include "WikiGraph.h"

long WikiGraph::str_to_int(string str){
    long num = 0;
    for (char i : str){
        if (i >= '0' && i <= '9'){
            num = num * 10 + (i - '0');
        }
        else {
            cout << "File format error." <<endl;
            return -1;
        }
    }
    return num;
}
void WikiGraph::load_data(string fileName) {

    fstream file;
    file.open(fileName);

    if (!file.is_open()) {
        cout << "Failed to open file." << endl;
        return;
    }
    else {
        string line;
        getline(file, line);

        int num_pages = str_to_int(line);
        for (int i = 0; i < num_pages; i++) {
            string name;
            string links;
            getline(file, line);
            int pos = line.size() - 1;
            // get number of links from page
            for (; pos > 0; pos--){
                if (line.at(pos) >= '0' && line.at(pos) <= '9'){
                    links = line.at(pos) + links;
                }
                else {
                    break;}
            }

            name = line.substr(0, pos);

            // find/create current page wiki
            Wiki* fromPage;
            Wiki* toPage;
            vector<Wiki*>* vec1, vec2;
            if (map.find(name) == map.end()){ // not found
                fromPage = new Wiki(name);
                vec1 = &fromPage->outWikis;
                map[name] = fromPage;
            }
            else { // found
                fromPage = map[name];
                vec1 = &fromPage->outWikis;
            }

            // add links to the map for the current page
            for (int j = 0; j < str_to_int(links); j++) {
                getline(file, line);
                // find/create to page wiki
                if (map.find(line) == map.end()){
                    toPage = new Wiki(line);
                    map[line] = toPage;
                }
                else {
                    toPage = map[line];
                }
                vec1->push_back(toPage);
            }
        }
        file.close();
    }
}

vector<WikiGraph::Wiki*> WikiGraph::bfs(Wiki* start, Wiki* end){
    queue<string> q; // wiki name queue for map search
    q.push(start->name);

    queue<vector<Wiki*>> paths; // queue of paths from s->e
    vector<Wiki*> currPath;
    currPath.push_back(start);
    paths.push(currPath);

    unordered_map<string, bool> visited; // map of visited wikis to avoid cycles

    while(!q.empty()){
        vector<Wiki*> nextPath = paths.front();
        if (map[q.front()] == end){ // end found, path will contain start and end
            return nextPath;
        }
        Wiki* curr = map[q.front()];
        for (int i = 0; i < curr->outWikis.size(); i++){
            nextPath = paths.front(); // prep for new outwiki
            if (visited.find(curr->outWikis.at(i)->name) == visited.end()){ // not visited
                visited[curr->name] = true;
                q.push(curr->outWikis.at(i)->name);
                nextPath.push_back(curr->outWikis.at(i));
                paths.push(nextPath);
            }
        }
        q.pop();
        paths.pop();
    }
    return paths.front();
}


// return the string for the unvisisted node with the smallest distance 
//std::string WikiGraph::getSmallestDistance(std::unordered_map<std::string, std::pair<unsigned int, bool>>& nodes) {
std::string WikiGraph::getSmallestDistance(std::unordered_map<std::string, unsigned int>& nodes) { // testing
    // set initial string and distance
    std::string smallestNode = nodes.begin()->first;
    //unsigned int smallestDistance = nodes.begin()->second.first;
    unsigned int smallestDistance = nodes.begin()->second; // testing

    for (auto it = nodes.begin(); it != nodes.end(); it++) {
        // don't check visited nodes 
        /*
        if (it->second.second == true) {
            continue;
        }
        */
        

        std::string node = it->first;
        //unsigned int distance = it->second.first;
        unsigned int distance = it->second; // testing

        if (distance < smallestDistance) {
            smallestNode = node;
            smallestDistance = distance;
        }
    }

    // if all unvisited nodes had a distance of inf, algorithm ends
    if (smallestDistance == std::numeric_limits<unsigned int>::max()) {
        return "";
    }

    return smallestNode;
}

// return the smallest distance to end
unsigned int WikiGraph::getDistToEnd(const std::string& end, std::unordered_map<std::string, std::pair<unsigned int, bool>>& nodes) {
    std::vector<std::pair<std::string, unsigned int>> nodeAndDist;

    // loop through all nodes
    for (auto it = map.begin(); it != map.end(); it++) {
        std::string node = it->first;

        // if node checking is end, don't count it
        if (node == end) {
            continue;
        }

        // loop through nodes it points to
        //for (int i = 0; i < it->second.size(); i++) {
        for (int i = 0; i < it->second->outWikis.size(); i++) {
            // [NOTE]: IF USING MAYA'S CODE UNCOMMENT THIS
            std::string subNode = it->second->outWikis.at(i)->name;

            //std::string subNode = it->second.at(i); don't use this line, use below
            //std::string subNode = it->second->outWikis.at(i)->name;

            // if a subnode is end, add the parent to list & its distance
            if (subNode == end) {
                nodeAndDist.emplace_back(node, nodes.find(node)->second.first);
            }
        }
    }

    // if no nodes point to end, return infinity
    if (nodeAndDist.size() == 0) {
        return std::numeric_limits<unsigned int>::max();
    }

    // find smallest dist
    unsigned int smallestDist = nodeAndDist.at(0).second;

    for (int i = 1; i < nodeAndDist.size(); i++) {
        if (nodeAndDist.at(i).second < smallestDist) {
            smallestDist = nodeAndDist.at(i).second;
        }
    }

    return smallestDist;
}

unsigned int WikiGraph::dijkstra(const std::string& start, const std::string& end) {
    if (map.find(start)->second->outWikis.size() == 0) {
        return std::numeric_limits<unsigned int>::max();
    }

    // create set of all unvisited nodes with their distances
    // string for node title, int for distance, bool for visited
    std::unordered_map<std::string, std::pair<unsigned int, bool>> nodes;

    std::unordered_map<std::string, unsigned int> smallestDistNodes;

    // assign every node the max distance 
    for (auto it = map.begin(); it != map.end(); it++) { // O(N)
        // max represents infinity
        nodes.insert(std::make_pair(it->first, std::make_pair(std::numeric_limits<unsigned int>::max(), false)));
    }    

    // update current node (start)
    std::string currentNode = start;

    // update its distance from the start
    nodes.find(currentNode)->second.first = 0;
    smallestDistNodes.insert(std::make_pair(currentNode, 0));

    bool found = false; 

    // [note]: using "" as no more nodes without inf distance 
    while (currentNode != "") { // O(N)
        // [note]: what to do if currentNode is end?
        if (currentNode == end) {
            break;
        }

        // get distance of current node
        unsigned int currentNodeDistance = nodes.find(currentNode)->second.first;

        cout << "Node current node outwiki size: " << map.find(currentNode)->second->outWikis.size() << "\n";

        // loop through current node's neighbors
        for (int i = 0; i < map.find(currentNode)->second->outWikis.size(); i++) { // O(N*log(N)) (find is log)
            std::string neighbor = map.find(currentNode)->second->outWikis.at(i)->name;

            // testing
            if (neighbor == end) {
                found = true;
                break;
            }

            // algorithm should only check nodes that are in the map
            // if can't find that child node in map, don't do stuff with it
            if (map.find(currentNode) == map.end()) {
                continue;
            }
            
            // if the neighbor's distance is infinity or neighbor's distance is > currentNodeDistance 
            if (nodes.find(neighbor)->second.first == std::numeric_limits<unsigned int>::max() || // O(1)
                nodes.find(neighbor)->second.first > currentNodeDistance + 1) {

                nodes.find(neighbor)->second.first = currentNodeDistance + 1;
                smallestDistNodes.insert(std::make_pair(neighbor, currentNodeDistance + 1)); // O(1)
            }
        }

        // once done with neighbors, mark node as visited
        nodes.find(currentNode)->second.second = true;
        // remove from smallest pool
        smallestDistNodes.erase(currentNode);

        // find unvisited node with the smallest distance & set as new node
        currentNode = getSmallestDistance(smallestDistNodes);

        if (found) {
            currentNode = "";
        }
    }

    // find node that has end in outWikis with the smallest distance 
    return getDistToEnd(end, nodes) + 1;
}
