#include <fstream>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

#include "SportsLayout.h"

    SportsLayout::SportsLayout(string inputfilename)
    {
          
        readInInputFile(inputfilename);
        mapping= new int[z];

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

    // void SportsLayout::readOutputFile(string output_filename)
    // {
    //         fstream ipfile;
	//         ipfile.open(output_filename, ios::in);
    //         if (!ipfile) {
    //             cout << "No such file\n";
    //             exit( 0 );
    //         }
    //         else {
                
    //             vector<int> ip;

    //             while (1) {
    //                 int t;
    //                 ipfile >> t;
    //                 ip.push_back(t);
    //                 if (ipfile.eof())
    //                     break;
                    
    //             }
            
    //         if(ip.size()!=z)
    //         {
    //             cout<<"number of values not equal to number of zones, check output format\n";
    //             exit(0);
    //         }
    //         for(int i=0;i<z;i++)
    //         mapping[i]=ip[i];
    //     ipfile.close();

    //     if(!check_output_format())
    //         exit(0);
    //     cout<<"Read output file, format OK"<<endl;

    //         }
        
    // }


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


            

            int **tempT;
            int **tempN;

          tempT = new int*[l];
         for (int i = 0; i < l; ++i)
            tempT[i] = new int[l];
        
        tempN = new int*[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new int[z];

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
        for(auto x: random_state){
            cout<<x<<" ";
        }
        cout<<"\n";

        return random_state;
    }

    pair<vector<int>,long long> SportsLayout::get_neighbour(vector<int> &current){
        vector<int> neighbour;
        long long min_cost = LLONG_MAX;
        //Swap function
        for(int i = 0;i<z;i++){
            for(int j = i+1;j<z;j++){
                swap(current[i],current[j]);
                long long temp = cost_fn(current);
                if(temp < min_cost){
                    min_cost = temp;
                    neighbour = current;
                }
                swap(current[i],current[j]);
            }
        }

        //neighbourhood function for unused locations.
        //Can add a condition to execute it if l - z is very small.

        vector<bool> used(z+1);
        for(auto &ele : current){
            used[ele] = 1;
        }
        vector<int> not_used;
        for(int i = 1;i<=l;i++){
            if(!used[i]){
                not_used.push_back(i);
            }
        }
        
        for(int i = 0;i<z;i++){
            for(auto &ele: not_used){
                int temp_ele = current[i];
                current[i] = ele;
                long long temp = cost_fn(current);
                if(temp < min_cost){
                    min_cost = temp;
                    neighbour = current;
                }
                current[i] = temp_ele;
            }
        }

        return {neighbour,min_cost};
    }

    pair<vector<int>,long long> SportsLayout::get_random_neighour(vector<int> &current){
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
            swap(current[i],current[j]);
            neighbour = current;
            neighbour_cost = cost_fn(neighbour);
            swap(current[i],current[j]);
        }
        else{
            vector<bool> used(z+1);
            for(auto &ele : current){
                used[ele] = 1;
            }
            vector<int> not_used;
            for(int i = 1;i<=l;i++){
                if(!used[i]){
                    not_used.push_back(i);
                }
            }
            int i = rand()%z;
            int j = rand()%(not_used.size());
            neighbour = current;
            neighbour[i] = not_used[j];
            neighbour_cost = cost_fn(neighbour);
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
            if (elapsed_time.count() > (time*60*0.95))
            {
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                return ans;
            }
            auto neigh_val = get_neighbour(current);
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
            if (elapsed_time.count() > (time*60*0.95))
            {
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                return ans;
            }
            double check = get_prob();
            cout<<check<<"\n";
            if(check <= prob){
                auto neigh_val = get_random_neighour(current);
                current = neigh_val.first;
                current_cost = neigh_val.second;
            }
            else{
                cout<<"-----Taking greedy step-----"<<"\n";
                auto neigh_val = get_neighbour(current);
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
            if (elapsed_time.count() > (time*60*0.95))
            {
                std::cout << "Time limit exceeded, returning the current best allocation" << std::endl;
                return ans;
            }
            double check = get_prob();
            cout<<check<<"\n";
            if(check <= prob){
                auto neigh_val = get_random_neighour(current);
                current = neigh_val.first;
                current_cost = neigh_val.second;
            }
            else{
                cout<<"-----Taking greedy step-----"<<"\n";
                auto neigh_val = get_neighbour(current);
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
        int max_restarts = 30;
        // auto ans = hill_climbing_random_restarts(max_restarts,start_time);
        double prob = 0.2;
        auto ans = hill_climbing_random_walks_restarts(max_restarts, prob, start_time);
        for(int i = 0;i<z;i++){
            mapping[i] = ans[i];
        }
  
    }


