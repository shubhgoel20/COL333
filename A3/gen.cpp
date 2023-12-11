#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <cmath>
#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>

using namespace std;

int main(int argc, char* argv[]){
    int n = atoi(argv[1]);
    int k1 = atoi(argv[2]);
    int k2 = atoi(argv[3]);

    vector<int> used(n+1,0);
    vector<int> g;
    while(g.size() != k1+k2){
        int v = (rand()%n) + 1;
        while(used[v]){
            v = (rand()%n) + 1;
        }
        used[v] = 1;
        g.push_back(v);
    }

    set<pair<int,int>> edges;

    for(int i = 0;i<k1;i++){
        for(int j = i+1;j<k1;j++){
            if(g[i] > g[j]) edges.insert({g[j],g[i]});
            else edges.insert({g[i],g[j]});
        }
    }
    for(int i = k1;i<k1+k2;i++){
        for(int j = i+1;j<k1+k2;j++){
            if(g[i] > g[j]) edges.insert({g[j],g[i]});
            else edges.insert({g[i],g[j]});
        }
    }

    int tot = (n*(n-1))/2;

    int e = (k1*(k1-1))/2;
    e+= (k2*(k2-1))/2;
    if(e < tot) e+= (rand()%(tot - (e)));

    while(edges.size() != e){
        int u = (rand()%n) + 1;
        int v = (rand()%n) + 1;
        while(v == u){
            v = (rand()%n) + 1;
        }
        if(u>v) swap(u,v);
        edges.insert({u,v});
    }

    ofstream out;
    out.open("test.graph", std::ios::out | std::ios::trunc);
    out<<n<<" "<<edges.size()<<" "<<k1<<" "<<k2<<"\n";
    for(auto iter = edges.begin();iter != edges.end();iter++){
        out<<(*iter).first<<" "<<(*iter).second<<"\n";
    }
    out.close();

}