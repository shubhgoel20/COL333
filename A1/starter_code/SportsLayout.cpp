#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
          
        readInInputFile(inputfilename);
        mapping= vector<int>(z);
        used = vector<int>(l+1);
        tot_time = vector<long long>(l+1,0);
        for(int i = 1;i<=l;i++){
            for(int j = 1;j<l;j++){
                tot_time[i]+=(long long)T[i-1][j-1];
            }
        }
        Cswap = vector<vector<long long>>(z,vector<long long>(z,0));
        symsumN = vector<vector<int>>(z,vector<int>(z,0));
        Cex = vector<vector<long long>>(z,vector<long long>(l,0));
        for(int i=0; i<z; i++)
        {
            for(int j=0; j<z; j++)
            {
                symsumN[i][j] = (N[i][j] + N[j][i]);
            }
        }
    }

    bool SportsLayout::is_unique(vector<int> &state){
        set<int> s(state.begin(),state.end());
        return (s.size() == state.size());
    }

    void SportsLayout::preprocessCswap(vector<int> &state){
        for(int i = 0; i<z; i++)
        {
            for(int j = 0; j<z; j++)
            {
                long long ans1 = 0;
                for(int k = 0; k<z; k++)
                {
                    ans1+= (long long)symsumN[i][k]*(long long)T[state[k] - 1][state[j] - 1];
                }
                Cswap[i][j] = ans1;
            }
        }
    }

    void SportsLayout::preprocess(vector<int> &state){

         for(int i = 0; i<z; i++)
        {
            for(int j = 0; j<z; j++)
            {
                long long ans1 = 0;
                long long ans2 = 0;
                for(int k = 0; k<z; k++)
                {
                    ans1+= (long long)symsumN[i][k]*(long long)T[state[k] - 1][state[j] - 1];
                    ans2+= (long long)symsumN[i][k]*(long long)T[state[k] - 1][j];
                }
                Cswap[i][j] = ans1;
                Cex[i][j] = ans2;
            }
            for(int j = z;j<l;j++){
                long long ans = 0;
                for(int k = 0; k<z; k++)
                {
                    ans+= (long long)symsumN[i][k]*(long long)T[state[k] - 1][j];
                }
                Cex[i][j] = ans;
            }
        }
    }

    void SportsLayout::updateCswap(int i, int j, vector<int> &state)
    {
        for(int x = 0; x < z; x++)
        {
            swap(Cswap[x][j],Cswap[x][i]);
            long long store = symsumN[x][i] - symsumN[x][j];
            Cswap[x][j]+= store*(long long)T[state[j]-1][state[i]-1];
            Cswap[x][i]-= store*(long long)T[state[i]-1][state[j]-1];
            for(int y = 0; y<l; y++)
            {
                Cex[x][y]+= store*(long long)(T[state[j]-1][y] - T[state[i]-1][y]);
                if(y<z){
                    if(y == i || y == j){continue;}
                    Cswap[x][y]+= store*(long long)(T[state[j]-1][state[y]-1] - T[state[i]-1][state[y]-1]);
                }
            }
        }
    }

    void SportsLayout::updateCex(int i, int new_loc, vector<int> &state)
    {
        for(int x = 0; x < z; x++)
        {
            long long store = symsumN[x][i];
            long long ans_i = 0;
            for(int k = 0;k<z;k++){
                if(k != i) ans_i+=(long long)symsumN[x][k]*(long long)T[state[k] - 1][new_loc-1];
            }
            Cswap[x][i] = ans_i;
            for(int y = 0; y<l; y++)
            {
                Cex[x][y]+= store*(long long)(T[new_loc-1][y] - T[state[i]-1][y]);
                if(y<z){
                    if(y == i) continue;
                    Cswap[x][y]+= store*(long long)(T[new_loc-1][state[y] - 1] - T[state[i]-1][state[y] - 1]);
                }
            }
        }
    }

    bool SportsLayout::check_output_format()
    {

        vector<bool> visited(l,false);
        for(int i=0;i<z;i++)
        {
            if((mapping[i]>=1 && mapping[i]<=l))
            {
                if(!visited[mapping[i]-1])
                visited[mapping[i]-1]=true;
                else
                {
                    cout<<"Repeated locations, check format\n";
                    return false;
                }
            }
            else
            {
                cout<<"Invalid location, check format\n";
                return false;
            }
        }

        return true;

    }



    long long SportsLayout::cost_fn()
    {
        long long cost=0;
        for(int i=0;i<z;i++)
        {
           for(int j=(i+1);j<z;j++)
            {
                cost+=(long long)(N[i][j]+N[j][i])*T[mapping[i]-1][mapping[j]-1];
            }
        }
        return cost;
    }

    long long SportsLayout::cost_fn(vector<int> &state)
    {
        long long cost=0;
        for(int i=0;i<z;i++)
        {
            for(int j=(i+1);j<z;j++)
            {
                cost+=(long long)(N[i][j]+N[j][i])*T[state[i]-1][state[j]-1];
            }
        }
        return cost;
    }

    long long SportsLayout::cost_fn_swap(vector<int> &state, long long curr_cost, int i, int j)
    {
         for(int k = 0;k<z;k++)
        {
            if(k == i || k == j) continue;
            curr_cost+=((long long)(symsumN[i][k] - symsumN[j][k]))*(T[state[k] - 1][state[j] - 1] - T[state[k] - 1][state[i] - 1]);
        }
        return curr_cost;
    }

    long long SportsLayout::cost_fn_swap_fast(vector<int> &state, long long curr_cost, int i, int j)
    {

        long long new_cost = curr_cost + Cswap[i][j] - Cswap[i][i]  - Cswap[j][j] + Cswap[j][i];
        new_cost-= ((long long)(symsumN[i][i] - symsumN[j][i]))*(T[state[i] - 1][state[j] - 1] - T[state[i] - 1][state[i] - 1]);
        new_cost-= ((long long)(symsumN[i][j] - symsumN[j][j]))*(T[state[j] - 1][state[j] - 1] - T[state[j] - 1][state[i] - 1]);
        return new_cost;
    }

    long long SportsLayout::cost_fn_exchange(vector<int> &state, long long curr_cost, int i, int new_loc)
    {
        for(int k = 0;k<z;k++)
        {
            if(k == i) continue;
            curr_cost+=((long long)(symsumN[i][k]))*(T[state[k] - 1][new_loc - 1] - T[state[k] - 1][state[i] - 1]);
        }
        return curr_cost;
    }

    long long SportsLayout::cost_fn_exchange_fast(vector<int> &state, long long curr_cost, int i, int new_loc)
    {
        long long new_cost = curr_cost + Cex[i][new_loc-1] - Cswap[i][i];
        new_cost-= ((long long)(symsumN[i][i]))*(T[state[i] - 1][new_loc - 1] - T[state[i] - 1][state[i] - 1]);
        return new_cost;
    }

    void SportsLayout::readInInputFile(string inputfilename)
    {
            fstream ipfile;
	        ipfile.open(inputfilename, ios::in);
            if (!ipfile) {
                cout << "No such file\n";
                exit( 0 );
            }
            else {
                

                ipfile>> time;
                ipfile >> z;
                ipfile >> l;

                if(z>l)
                {
                    cout<<"Number of zones more than locations, check format of input file\n";
                    exit(0);
                }


            

        //     int **tempT;
        //     int **tempN;

        //   tempT = new int*[l];
        //  for (int i = 0; i < l; ++i)
        //     tempT[i] = new int[l];
        
        // tempN = new int*[z];
        // for (int i = 0; i < z; ++i)
        //     tempN[i] = new int[z];
        vector<vector<int>> tempT(l,vector<int>(l));
        vector<vector<int>> tempN(z,vector<int>(z));
        for(int i=0;i<z;i++)
        {
            for(int j=0;j<z;j++)
            ipfile>>tempN[i][j];
        }

        for(int i=0;i<l;i++)
        {
            for(int j=0;j<l;j++)
            ipfile>>tempT[i][j];
        }

        ipfile.close();

        T= tempT;
        N= tempN;
         }

    }

    void SportsLayout::write_to_file(string outputfilename){

        bool correct = check_output_format();
        if(!correct) return;

         // Open the file for writing
        ofstream outputFile(outputfilename);

        // Check if the file is opened successfully
        if (!outputFile.is_open()) {
            cerr << "Failed to open the file for writing." << std::endl;
            exit(0);
        }

        for(int i=0;i<z;i++)
        outputFile<<mapping[i]<<" ";

        // Close the file
        outputFile.close();

        cout << "Allocation written to the file successfully." << endl;

    }

    vector<int> SportsLayout::get_random_state(){
        int min = 1;
        random_device rd;
        static mt19937_64 generator(rd());
        uniform_int_distribution<int> dist{min, l};
        set<int> temp;
        if(z <= (l/2)){
            while(temp.size() != z)
            {
                temp.insert(dist(generator));
            }
        }
        else{
            for(int i = 1;i<=l;i++){
                temp.insert(i);
            }
            while(temp.size() != z){
                auto iter = temp.find(dist(generator));
                if(iter == temp.end()) continue;
                temp.erase(iter);
            }
        }
        vector<int> random_state(temp.begin(),temp.end());
        random_shuffle(random_state.begin(),random_state.end());
        cout<<"-----Random restart called-----"<<"\n";
        preprocess(random_state);
        fill(used.begin(),used.end(),0);
        for(auto ele : random_state){
            used[ele] = 1;
        }
        not_used.clear();
        for(int i = 1;i<=l;i++){
            if(!used[i]){
                not_used.insert(i);
            }
        }
        return random_state;
    }

    pair<pair<int,pair<int,int>>,long long> SportsLayout::get_neighbour(vector<int> &current, long long curr_cost, std::chrono::high_resolution_clock::time_point start_time){
        // static srand(static_cast<unsigned>(std::time(0)));
        vector<int> neighbour = current;
        long long min_cost = curr_cost;
        int neighbour_type = 0;
        int swap_pos1 = 0;
        int swap_pos2 = 0;
        int exchange_pos = -1;
        int new_loc = 1;
        int old_loc = 1;
        
        for(int i = 0;i<z;i++){
            for(int j = i+1;j<z;j++){
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                if (elapsed_time.count() > (time*60*0.96))
                {
                    abbort = true;
                    return {{neighbour_type,{swap_pos1,swap_pos2}},min_cost};
                }
                long long temp = cost_fn_swap_fast(current,curr_cost,i,j);
                if(temp < min_cost){
                    min_cost = temp;
                    neighbour_type = 0;
                    swap_pos1 = i;
                    swap_pos2 = j;
                }
                
            }
        }
        
        for(int i = 0;i<z;i++){
            for(auto iter = not_used.begin();iter!=not_used.end();iter++){
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                if (elapsed_time.count() > (time*60*0.96))
                {
                    abbort = true;
                    return {{neighbour_type,{exchange_pos,new_loc}},min_cost};
                }
                auto ele = *iter;
                long long temp = cost_fn_exchange_fast(current,curr_cost,i,ele);
                if(temp < min_cost){
                    min_cost = temp;
                    neighbour_type = 1;
                    exchange_pos = i;
                    new_loc = ele;
                    old_loc = current[i];
                }
                
            }
        }

        if(neighbour_type == 0){
            return {{neighbour_type,{swap_pos1,swap_pos2}},min_cost};
        }
        not_used.erase(not_used.find(new_loc));
        not_used.insert(old_loc);
        used[new_loc] = 1;
        used[old_loc] = 0;
        cout<<"executed second neihgbour fn"<<"\n";
        return {{neighbour_type,{exchange_pos,new_loc}},min_cost};
    }

    pair<pair<int,pair<int,int>>,long long> SportsLayout::get_random_neighour(vector<int> &current, long long curr_cost){
        // static srand(static_cast<unsigned>(std::time(0)));
        cout<<"-----Random walk initiated-----"<<"\n";
        int neighbour_type = rand()%2;
        vector<int> neighbour;
        long long neighbour_cost;
        // cout<<neighbour_type<<"\n";
        if(neighbour_type == 0 || (z == l)){
            neighbour_type = 0;
            int i = rand()%z;
            int j = rand()%z;
            while(j == i){
                j = rand()%z;
            }
            
            neighbour_cost = cost_fn_swap_fast(current,curr_cost,i,j);
            return {{neighbour_type,{i,j}},neighbour_cost};
        }
        neighbour_type = 1;
        int i = rand()%z;
        int j;
        if(z <= (l/2)){
            // cout<<"type 1"<<"\n";
            j = rand()%(l+1);
            while(used[j] || (j == 0)){
                j = rand()%(l+1);
            }
        }
        else{
            // cout<<"type 2"<<"\n";
            auto iter = not_used.begin();
            int count = rand()%not_used.size();
            for(;iter != not_used.end();iter++){
                if(count == 0) break;
                count--;
            }
            j = *iter;
        }
        
        neighbour_cost = cost_fn_exchange_fast(current,curr_cost,i,j);
            
        not_used.erase(not_used.find(j));
        not_used.insert(current[i]);
        used[j] = 1;
        used[current[i]] = 0;
        
        
        return {{neighbour_type,{i,j}},neighbour_cost};
    }

    double SportsLayout::get_prob(){
        // static srand(static_cast<unsigned>(std::time(0)));
        return ((double)rand())/RAND_MAX;
    }

    vector<int> SportsLayout::hill_climbing_random_restarts(int max_restarts, std::chrono::high_resolution_clock::time_point start_time){
        auto current = get_random_state();
        auto current_cost = cost_fn(current);
        long long min_cost = LLONG_MAX;
        auto ans = current;
        while(true){
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
            if (elapsed_time.count() > (time*60*0.96))
            {
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                if(current_cost < min_cost){
                    min_cost = current_cost;
                    ans = current;
                }
                return ans;
            }
            
            cout<<"-----Taking greedy step-----"<<"\n";
            auto neigh_val = get_neighbour(current,current_cost,start_time);
            if(abbort){
                current_time = std::chrono::high_resolution_clock::now();
                elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                return ans;
            }
            if(neigh_val.second >= current_cost){
                if(current_cost < min_cost){
                    min_cost = current_cost;
                    ans = current;
                }
                if(max_restarts > 0){
                    current = get_random_state();
                    current_cost = cost_fn(current);
                    max_restarts--;
                }
                else{
                    cout<<"Max restarts limit reached, return the current best allocation"<<"\n";
                    return ans;
                }
            }
            else{
                
                current_cost = neigh_val.second;
                if(neigh_val.first.first == 0){
                    if(neigh_val.first.second.first != neigh_val.first.second.second){
                        updateCswap(neigh_val.first.second.first,neigh_val.first.second.second,current);
                        swap(current[neigh_val.first.second.first],current[neigh_val.first.second.second]);
                    }

                }
                else{
                    if(neigh_val.first.second.first != -1){
                        updateCex(neigh_val.first.second.first,neigh_val.first.second.second,current);
                        current[neigh_val.first.second.first] = neigh_val.first.second.second;
                    }
                }
                if(current_cost < min_cost){
                    ans = current;
                    min_cost = current_cost;
                }
            }
        }
        return ans;
    }

    vector<int> SportsLayout::hill_climbing_random_walks(double prob, std::chrono::high_resolution_clock::time_point start_time){
        auto current = get_random_state();
        auto current_cost = cost_fn(current);
        long long min_cost = current_cost;
        auto ans = current;
        while(true){
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
            if (elapsed_time.count() > (time*60*0.96))
            {
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                if(current_cost < min_cost){
                    min_cost = current_cost;
                    ans = current;
                }
                return ans;
            }
            double check = get_prob();
            cout<<check<<"\n";
            if(check <= prob){
                if(current_cost < min_cost){
                    min_cost = current_cost;
                    ans = current;
                }
                auto neigh_val = get_random_neighour(current, current_cost);
                current_cost = neigh_val.second;
                if(neigh_val.first.first == 0){
                    updateCswap(neigh_val.first.second.first,neigh_val.first.second.second,current);
                    swap(current[neigh_val.first.second.first],current[neigh_val.first.second.second]);

                }
                else{
                    updateCex(neigh_val.first.second.first,neigh_val.first.second.second,current);
                    current[neigh_val.first.second.first] = neigh_val.first.second.second;
                }
            }
            else{
                cout<<"-----Taking greedy step-----"<<"\n";
                auto neigh_val = get_neighbour(current,current_cost,start_time);
                if(abbort){
                    current_time = std::chrono::high_resolution_clock::now();
                    elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                    std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                    cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                    return ans;
                }
                if(neigh_val.second >= current_cost){
                    if(current_cost < min_cost){
                        min_cost = current_cost;
                        ans = current;
                    }
                    
                    cout<<"local optimum reached, return the current best allocation"<<"\n";
                    return ans;
                    
                }
                else{
                    current_cost = neigh_val.second;
                    if(neigh_val.first.first == 0){
                        if(neigh_val.first.second.first != neigh_val.first.second.second){
                            updateCswap(neigh_val.first.second.first,neigh_val.first.second.second,current);
                            swap(current[neigh_val.first.second.first],current[neigh_val.first.second.second]);
                        }

                    }
                    else{
                        if(neigh_val.first.second.first != -1){
                            updateCex(neigh_val.first.second.first,neigh_val.first.second.second,current);
                            current[neigh_val.first.second.first] = neigh_val.first.second.second;
                        }
                    }
                    if(current_cost < min_cost){
                        ans = current;
                        min_cost = current_cost;
                    }
                }
            }
            
        }
        return ans;
    }

    vector<int> SportsLayout::hill_climbing_random_walks_restarts(int max_restarts, double prob, std::chrono::high_resolution_clock::time_point start_time)
    {
        auto current = get_random_state();
        // assert(is_unique(current));
        auto current_cost = cost_fn(current);
        long long min_cost = current_cost;
        auto ans = current;
        if(z==1){return ans;}
        while(true){
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
            if (elapsed_time.count() > (time*60*0.96))
            {
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                if(current_cost < min_cost){
                    min_cost = current_cost;
                    ans = current;
                }
                return ans;
            }
            double check = get_prob();
            cout<<check<<"\n";
            if(check <= prob){
                if(current_cost < min_cost){
                    min_cost = current_cost;
                    ans = current;
                }
                auto neigh_val = get_random_neighour(current,current_cost);
                current_cost = neigh_val.second;
                if(neigh_val.first.first == 0){
                    updateCswap(neigh_val.first.second.first,neigh_val.first.second.second,current);
                    swap(current[neigh_val.first.second.first],current[neigh_val.first.second.second]);
                }
                else{
                    updateCex(neigh_val.first.second.first,neigh_val.first.second.second,current);
                    current[neigh_val.first.second.first] = neigh_val.first.second.second;
                }
                // assert(is_unique(current));
            }
            else{
                cout<<"-----Taking greedy step-----"<<"\n";
                auto neigh_val = get_neighbour(current,current_cost,start_time);
                if(abbort){
                    current_time = std::chrono::high_resolution_clock::now();
                    elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                    std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                    cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                    return ans;
                }
                if(neigh_val.second >= current_cost){
                    if(current_cost < min_cost){
                        min_cost = current_cost;
                        ans = current;
                    }
                    if(max_restarts > 0){
                        current = get_random_state();
                        current_cost = cost_fn(current);
                        max_restarts--;
                    }
                    else{
                        cout<<"Max restarts limit reached, return the current best allocation"<<"\n";
                        return ans;
                    }
                }
                else{
                    current_cost = neigh_val.second;
                    if(neigh_val.first.first == 0){
                        if(neigh_val.first.second.first != neigh_val.first.second.second){
                            updateCswap(neigh_val.first.second.first,neigh_val.first.second.second,current);
                            swap(current[neigh_val.first.second.first],current[neigh_val.first.second.second]);
                        }

                    }
                    else{
                        if(neigh_val.first.second.first != -1){
                            updateCex(neigh_val.first.second.first,neigh_val.first.second.second,current);
                            current[neigh_val.first.second.first] = neigh_val.first.second.second;
                        }
                    }
                    if(current_cost < min_cost){
                        ans = current;
                        min_cost = current_cost;
                    }
                    // assert(is_unique(current));
                }
            }
            
        }
        return ans;

    }

    void SportsLayout::compute_allocation()
    {
        //you can write your code here 
        //comment out following dummy code
        // for(int i=0;i<z;i++)
        // mapping[i]=i+1;

        

        auto start_time = std::chrono::high_resolution_clock::now();
        // vector<double> probs = {0.05,0.075,0.1,0.125,0.15,0.175,0.2,0.225,0.25,0.275,0.3,0.325};
        //best results are coming for prob = 0.275, can tune more if required
        double prob1 = 0.275;
        double prob2 = 0.075;
        // std::srand(static_cast<unsigned>(std::time(0)));
        mapping = hill_climbing_random_walks_restarts(100000,prob1,start_time);
    }


