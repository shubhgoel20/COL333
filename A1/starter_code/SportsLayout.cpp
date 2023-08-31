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



    long long SportsLayout::cost_fn(){


        long long cost=0;

        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[mapping[i]-1][mapping[j]-1];
           }
        }

        return cost;
    }

    long long SportsLayout::cost_fn(vector<int> &state){
        long long cost=0;

        for(int i=0;i<z;i++)
        {
           for(int j=0;j<z;j++) 
           {
                cost+=(long long)N[i][j]*(long long)T[state[i]-1][state[j]-1];
           }
        }

        return cost;
    }
    long long SportsLayout::cost_fn_swap(vector<int> &state, long long curr_cost, int i, int j){
        for(int k = 0;k<z;k++){
            if(k == i || k == j) continue;
            curr_cost+=((long long)N[k][i]*(long long)T[state[k] - 1][state[j] - 1] + (long long)N[k][j]*(long long)T[state[k] - 1][state[i] - 1]);
            curr_cost-=((long long)N[k][i]*(long long)T[state[k] - 1][state[i] - 1] + (long long)N[k][j]*(long long)T[state[k] - 1][state[j] - 1]);
        }

        for(int k = 0;k<z;k++){
            if(k != i){
                curr_cost-= (long long)N[i][k]*(long long)T[state[i] - 1][state[k] - 1];
                if(k != j) curr_cost+= (long long)N[i][k]*(long long)T[state[j] - 1][state[k] - 1];
                else curr_cost+= (long long)N[i][k]*(long long)T[state[j] - 1][state[i] - 1];
            }
            if(k != j){
                curr_cost-= (long long)N[j][k]*(long long)T[state[j] - 1][state[k] - 1];
                if(k != i) curr_cost+= (long long)N[j][k]*(long long)T[state[i] - 1][state[k] - 1];
                else curr_cost+= (long long)N[j][k]*(long long)T[state[i] - 1][state[j] - 1];
            }
            
        }

        return curr_cost;
        
    }

    long long SportsLayout::cost_fn_exchange(vector<int> &state, long long curr_cost, int i, int new_loc){
        for(int k = 0;k<z;k++){
            if(k == i) continue;
            curr_cost+=((long long)N[k][i]*(long long)T[state[k] - 1][new_loc - 1]);
            curr_cost-=((long long)N[k][i]*(long long)T[state[k] - 1][state[i] - 1]);
        }

        for(int k = 0;k<z;k++){
            if(k != i){
                curr_cost-= (long long)N[i][k]*(long long)T[state[i] - 1][state[k] - 1];
                curr_cost+= (long long)N[i][k]*(long long)T[new_loc - 1][state[k] - 1];
            }
            
        }

        return curr_cost;
        
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
        while(temp.size() != z)
        {
            temp.insert(dist(generator));
        }
        vector<int> random_state(temp.begin(),temp.end());
        random_shuffle(random_state.begin(),random_state.end());
        cout<<"-----Random restart called-----"<<"\n";
        // for(auto x: random_state){
        //     cout<<x<<" ";
        // }
        // cout<<"\n";
        return random_state;
    }

    pair<vector<int>,long long> SportsLayout::get_neighbour(vector<int> &current, long long curr_cost, std::chrono::high_resolution_clock::time_point start_time){
        static srand(static_cast<unsigned>(std::time(0)));
        vector<int> neighbour = current;
        long long min_cost = curr_cost;

        //Swap function
        if(z < 500){ //Heuristic, can be tuned O(z^3)
            // cout<<"here"<<"\n";
            for(int i = 0;i<z;i++){
                for(int j = i+1;j<z;j++){
                    auto current_time = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                    if (elapsed_time.count() > (time*60*0.99))
                    {
                        abbort = true;
                        return {neighbour,min_cost};
                    }
                    // swap(current[i],current[j]);
                    long long temp = cost_fn_swap(current,curr_cost,i,j);
                    if(temp < min_cost){
                        min_cost = temp;
                        swap(current[i],current[j]);
                        neighbour = current;
                        swap(current[i],current[j]);
                    }
                    
                }
            }
        }
        else{ //O(z^2)
            vector<int> indices;
            vector<bool> mark(z,0);
            while(indices.size()*indices.size() < z){
                int idx = (rand()%z);
                if(!mark[idx]){
                    indices.push_back(idx);
                    mark[idx] = 1;
                }
            }
            // vector<int> indices(ind.begin(),ind.end());
            for(int i = 0;i<indices.size();i++){
                for(int j = i+1;j<indices.size();j++){
                    auto current_time = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                    if (elapsed_time.count() > (time*60*0.99))
                    {
                        abbort = true;
                        return {neighbour,min_cost};
                    }
                    // swap(current[i],current[j]);
                    long long temp = cost_fn_swap(current,curr_cost,indices[i],indices[j]);
                    if(temp < min_cost){
                        min_cost = temp;
                        swap(current[indices[i]],current[indices[j]]);
                        neighbour = current;
                        swap(current[indices[i]],current[indices[j]]);
                    }
                    
                }
            }
        }
        // cout<<"First neighbourhood function executed"<<"\n";
        //neighbourhood function for unused locations.
        //Can add a condition to execute it if l - z is very small.
        fill(used.begin(),used.end(),0);
        for(auto ele : current){
            used[ele] = 1;
        }
        not_used.clear();
        for(int i = 1;i<=l;i++){
            if(!used[i]){
                not_used.push_back({tot_time[i],i});
            }
        }
        // random_shuffle(not_used.begin(),not_used.end());
        sort(not_used.begin(),not_used.end());
        int max_size = (l-z)*z*z > 1e7 ? min((int)(1e7/(z*z)),(int)not_used.size()):not_used.size();
        

        for(int i = 0;i<z;i++){
            for(int j = 0;j<max_size;j++){
                auto current_time = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                if (elapsed_time.count() > (time*60*0.99))
                {
                    abbort = true;
                    return {neighbour,min_cost};
                }
                auto ele = not_used[j].second;
                int temp_ele = current[i];
                
                long long temp = cost_fn_exchange(current,curr_cost,i,ele);
                if(temp < min_cost){
                    current[i] = ele;
                    min_cost = temp;
                    neighbour = current;
                    current[i] = temp_ele;
                }
                
            }
        }

        // cout<<"Second neighbourhood function executed"<<"\n";
        return {neighbour,min_cost};
    }

    pair<vector<int>,long long> SportsLayout::get_random_neighour(vector<int> &current, long long curr_cost){
        static srand(static_cast<unsigned>(std::time(0)));
        cout<<"-----Random walk initiated-----"<<"\n";
        int neighbour_type = rand()%2;
        vector<int> neighbour;
        long long neighbour_cost;
        if(neighbour_type == 0){
            int i = rand()%z;
            int j = rand()%z;
            while(j == i){
                j = rand()%z;
            }
            
            neighbour_cost = cost_fn_swap(current,curr_cost,i,j);
            swap(current[i],current[j]);
            neighbour = current;
            swap(current[i],current[j]);
        }
        else{
           fill(used.begin(),used.end(),0);
            for(auto ele : current){
                used[ele] = 1;
            }
            not_used.clear();
            for(int i = 1;i<=l;i++){
                if(!used[i]){
                    not_used.push_back({tot_time[i],i});
                }
            }
            int i = rand()%z;
            int j = rand()%(not_used.size());
            neighbour_cost = cost_fn_exchange(current,curr_cost,i,not_used[j].second);
            neighbour = current;
            neighbour[i] = not_used[j].second;
            
        }
        return {neighbour,neighbour_cost};
    }

    double SportsLayout::get_prob(){
        static srand(static_cast<unsigned>(std::time(0)));
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
            if (elapsed_time.count() > (time*60*0.99))
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
                if(neigh_val.second < min_cost){
                    min_cost = neigh_val.second;
                    ans = neigh_val.first;
                }
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
                
                current = neigh_val.first;
                current_cost = neigh_val.second;
            }
        }
        return current;
    }

    vector<int> SportsLayout::hill_climbing_random_walks(double prob, std::chrono::high_resolution_clock::time_point start_time){
        auto current = get_random_state();
        auto current_cost = cost_fn(current);
        long long min_cost = current_cost;
        auto ans = current;
        while(true){
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
            if (elapsed_time.count() > (time*60*0.99))
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
                current = neigh_val.first;
                current_cost = neigh_val.second;
            }
            else{
                cout<<"-----Taking greedy step-----"<<"\n";
                auto neigh_val = get_neighbour(current,current_cost,start_time);
                if(abbort){
                    current_time = std::chrono::high_resolution_clock::now();
                    elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                    std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                    cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                    if(neigh_val.second < min_cost){
                        min_cost = neigh_val.second;
                        ans = neigh_val.first;
                    }
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
                    
                    current = neigh_val.first;
                    current_cost = neigh_val.second;
                }
            }
            
        }
        return current;
    }

    vector<int> SportsLayout::hill_climbing_random_walks_restarts(int max_restarts, double prob, std::chrono::high_resolution_clock::time_point start_time)
    {
        auto current = get_random_state();
        auto current_cost = cost_fn(current);
        long long min_cost = current_cost;
        auto ans = current;
        while(true){
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
            if (elapsed_time.count() > (time*60*0.99))
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
                current = neigh_val.first;
                current_cost = neigh_val.second;
            }
            else{
                cout<<"-----Taking greedy step-----"<<"\n";
                auto neigh_val = get_neighbour(current,current_cost,start_time);
                if(abbort){
                    current_time = std::chrono::high_resolution_clock::now();
                    elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(current_time - start_time);
                    std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                    cout<<"Time Taken: "<<elapsed_time.count()<<" seconds"<<"\n";
                    if(neigh_val.second < min_cost){
                        min_cost = neigh_val.second;
                        ans = neigh_val.first;
                    }
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
                    
                    current = neigh_val.first;
                    current_cost = neigh_val.second;
                }
            }
            
        }
        return current;

    }

    void SportsLayout::compute_allocation()
    {
        //you can write your code here 
        //comment out following dummy code
        // for(int i=0;i<z;i++)
        // mapping[i]=i+1;

        auto start_time = std::chrono::high_resolution_clock::now();
        int max_restarts = 1000;
        // auto ans = hill_climbing_random_restarts(max_restarts,start_time);
        double prob = 0.3;
        // auto ans = hill_climbing_random_walks_restarts(max_restarts,prob, start_time);
        // auto ans = hill_climbing_random_walks_restarts(max_restarts,prob, start_time);
        auto ans = hill_climbing_random_restarts(max_restarts,start_time);
        for(int i = 0;i<z;i++){
            mapping[i] = ans[i];
        }
  
    }


