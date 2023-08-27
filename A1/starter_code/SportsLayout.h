
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout{

    private:
    int z,l;
    int** T;
    int **N;
    int time;
    int *mapping;

    public:

    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    long long cost_fn(vector<int> &state);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation();

    vector<int> hill_climbing_random_restarts(int max_restarts, std::chrono::high_resolution_clock::time_point start_time);
    vector<int> hill_climbing_random_walks(double prob, std::chrono::high_resolution_clock::time_point start_time);
    pair<vector<int>,long long> get_neighbour(vector<int> &current);
    pair<vector<int>,long long> get_random_neighour(vector<int> &current);
    vector<int> get_random_state();
    double get_prob();

};


#endif