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