#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

using namespace std;


unordered_map<int, unordered_set<int>> map;

void addEdge(int x, int y) { 
    map[x].insert(y);
}

void topo() {
    unordered_set<int> visited; 

    stack<int> mainStack; 

    for(auto it = map.begin(); it != map.end(); it++) {

        if(visited.count(it->first) == 0) { 
            visited.insert(it -> first); 
            stack<int> stack2;
            stack2.push(it -> first); 

            while(!stack2.empty()) {
                int current = stack2.top();
                bool unvis = false; 

                for(auto it2 = map[current].begin(); it2 != map[current].end(); it2++) {
                    int child = *it2;
                    if(visited.count(child) == 0) {
                        unvis = true;
                        visited.insert(child);
                        stack2.push(child);
                    }
                }
                if(!unvis) { 
                    stack2.pop();
                    mainStack.push(current);
                }
            }
        }
    }

    while(!mainStack.empty()) {
        cout << mainStack.top() << " ";
        mainStack.pop(); 
    }
    cout << endl;
}

int main() {
    addEdge(1,9);
    addEdge(2,3);
    addEdge(3,4);
    addEdge(4,6);
    addEdge(5,6);
    addEdge(6,8);
    addEdge(7,2);
    addEdge(7,8);
    addEdge(7,9);
    addEdge(8,11);
    addEdge(11,10);

    topo();

    return 0;
}
