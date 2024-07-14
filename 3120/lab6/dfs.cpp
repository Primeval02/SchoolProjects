#include <iostream>
#include <map>
#include <list>

using namespace std;

map<int, bool> visited;
map<int, list<int>> adjacent;

void dfs(int v)
{
    visited[v] = true;
    cout << v << " ";

    list<int>::iterator i;
    for (i = adjacent[v].begin(); i != adjacent[v].end(); i++)
        if (!visited[*i])
            dfs(*i);
}

void addEdge(int v, int w)
{
    adjacent[v].push_back(w);
}

int main()
{
    addEdge(2,1);
    addEdge(3,1);
    addEdge(4,1);
    addEdge(5,1);
    addEdge(6,1);
    addEdge(7,1);
    addEdge(8,1);
    addEdge(9,1);
    addEdge(11,1);
    addEdge(12,1);
    addEdge(13,1);
    addEdge(14,1);
    addEdge(18,1);
    addEdge(20,1);
    addEdge(22,1);
    addEdge(32,1);
    addEdge(1,2);
    addEdge(3,2);
    addEdge(4,2);
    addEdge(8,2);
    addEdge(14,2);
    addEdge(18,2);
    addEdge(20,2);
    addEdge(22,2);
    addEdge(31,2);
    addEdge(1,3);
    addEdge(2,3);
    addEdge(4,3);
    addEdge(8,3);
    addEdge(9,3);
    addEdge(10,3);
    addEdge(14,3);
    addEdge(28,3);
    addEdge(29,3);
    addEdge(33,3);
    addEdge(1,4);
    addEdge(2,4);
    addEdge(3,4);
    addEdge(8,4);
    addEdge(13,4);
    addEdge(14,4);
    addEdge(1,5);
    addEdge(7,5);
    addEdge(11,5);
    addEdge(1,6);
    addEdge(7,6);
    addEdge(11,6);
    addEdge(17,6);
    addEdge(1,7);
    addEdge(5,7);
    addEdge(6,7);
    addEdge(17,7);
    addEdge(1,8);
    addEdge(2,8);
    addEdge(3,8);
    addEdge(4,8);
    addEdge(1,9);
    addEdge(3,9);
    addEdge(31,9);
    addEdge(33,9);
    addEdge(34,9);
    addEdge(3,10);
    addEdge(34,10);
    addEdge(1,11);
    addEdge(5,11);
    addEdge(6,11);
    addEdge(1,12);
    addEdge(1,13);
    addEdge(4,13);
    addEdge(1,14);
    addEdge(2,14);
    addEdge(3,14);
    addEdge(4,14);
    addEdge(34,14);
    addEdge(33,15);
    addEdge(34,15);
    addEdge(33,16);
    addEdge(34,16);
    addEdge(6,17);
    addEdge(7,17);
    addEdge(1,18);
    addEdge(2,18);
    addEdge(33,19);
    addEdge(34,19);
    addEdge(1,20);
    addEdge(2,20);
    addEdge(34,20);
    addEdge(33,21);
    addEdge(34,21);
    addEdge(1,22);
    addEdge(2,22);
    addEdge(33,23);
    addEdge(34,23);
    addEdge(26,24);
    addEdge(28,24);
    addEdge(30,24);
    addEdge(33,24);
    addEdge(34,24);
    addEdge(26,25);
    addEdge(28,25);
    addEdge(32,25);
    addEdge(24,26);
    addEdge(25,26);
    addEdge(32,26);
    addEdge(30,27);
    addEdge(34,27);
    addEdge(3,28);
    addEdge(24,28);
    addEdge(25,28);
    addEdge(34,28);
    addEdge(3,29);
    addEdge(32,29);
    addEdge(34,29);
    addEdge(24,30);
    addEdge(27,30);
    addEdge(33,30);
    addEdge(34,30);
    addEdge(2,31);
    addEdge(9,31);
    addEdge(33,31);
    addEdge(34,31);
    addEdge(1,32);
    addEdge(25,32);
    addEdge(26,32);
    addEdge(29,32);
    addEdge(33,32);
    addEdge(34,32);
    addEdge(3,33);
    addEdge(9,33);
    addEdge(15,33);
    addEdge(16,33);
    addEdge(19,33);
    addEdge(21,33);
    addEdge(23,33);
    addEdge(24,33);
    addEdge(30,33);
    addEdge(31,33);
    addEdge(32,33);
    addEdge(34,33);
    addEdge(9,34);
    addEdge(10,34);
    addEdge(14,34);
    addEdge(15,34);
    addEdge(16,34);
    addEdge(19,34);
    addEdge(20,34);
    addEdge(21,34);
    addEdge(23,34);
    addEdge(24,34);
    addEdge(27,34);
    addEdge(28,34);
    addEdge(29,34);
    addEdge(30,34);
    addEdge(31,34);
    addEdge(32,34);
    addEdge(33,34);
    dfs(1);
    cout << endl;
    return 0; 
}

