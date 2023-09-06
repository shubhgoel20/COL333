
#ifndef SPORTSLAYOUT_H
#define	SPORTSLAYOUT_H

#include <fstream>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class SportsLayout{

    private:
    int z,l;
    vector<vector<int>> T;
    vector<vector<int>> N;
    vector<vector<long long>> Cswap;
    vector<vector<int>> symsumN;
    vector<vector<long long>> Cex;
    double time;
    double stop;
    vector<int> mapping;
    vector<int> used;
    set<int> not_used;
    // vector<long long> tot_time;
    bool abbort = false;
    public:

    SportsLayout(string inputfilename);

    bool check_output_format();

    // void readOutputFile(string output_filename);
    
    long long cost_fn();

    long long cost_fn(vector<int> &state);

    long long cost_fn_swap(vector<int> &state, long long curr_cost, int i, int j);
    long long cost_fn_swap_fast(vector<int> &state, long long curr_cost, int i, int j);

    long long cost_fn_exchange(vector<int> &state, long long curr_cost, int i, int new_loc);
    long long cost_fn_exchange_fast(vector<int> &state, long long curr_cost, int i, int new_loc);

    void write_to_file(string outputfilename);

    void readInInputFile(string inputfilename);

    void compute_allocation(std::chrono::high_resolution_clock::time_point &start_time);

    vector<int> hill_climbing_random_restarts(int max_restarts, std::chrono::high_resolution_clock::time_point &start_time);
    vector<int> hill_climbing_random_walks(double prob, std::chrono::high_resolution_clock::time_point &start_time);
    vector<int> hill_climbing_random_walks_restarts(int max_restarts, double prob, std::chrono::high_resolution_clock::time_point &start_time);
    vector<int> simulated_annealing(vector<int> &curr, long long curr_cost, std::chrono::high_resolution_clock::time_point &start_time);
    pair<pair<int,pair<int,int>>,long long> get_neighbour(vector<int> &current, long long curr_cost,std::chrono::high_resolution_clock::time_point &start_time);
    pair<pair<int,pair<int,int>>,long long> get_random_neighbour(vector<int> &current, long long curr_cost, std::chrono::high_resolution_clock::time_point &start_time);
    vector<int> get_random_state();
    vector<int> get_random_state_2();
    vector<int> get_random_state_3();
    double get_prob();
    double get_prob_uniform();
    void preprocess(vector<int> &state);
    void preprocessCswap(vector<int> &state);
    void updateCswap(int i, int j, vector<int> &state);
    void updateCex(int i, int new_loc, vector<int> &state);
    bool is_unique(vector<int> &state);
    pair<vector<int>,long long> bfs(vector<int> state, std::chrono::high_resolution_clock::time_point &start_time);
    vector<int> get_neighbour_bfs(vector<int> &state, std::chrono::high_resolution_clock::time_point &start_time);
    double exp_decay(int epoch, int k, int init);
    double time_based_decay(double curr, double decay, int epoch);
};


#endif