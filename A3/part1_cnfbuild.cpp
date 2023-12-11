#include <string>
#include <fstream>
#include <sstream>

#include "cnf_parallel.cpp"



int main(int argc, char* argv[]){
    //###################### <Input Handling> ############################
    std::string parameter = argv[1];
    std::string input_file = parameter + ".graph";
    
    std::ifstream file;
    file.open(input_file);
    std::string line;
    getline(file, line);
    std::istringstream iss(line);

    int32_t n,e,k1,k2;
    iss>>n>>e>>k1>>k2;

    std::vector<std::vector<int32_t>> adj(n+1,std::vector<int32_t>(n+1,0));
    
    while(getline(file, line))
    {
        std::istringstream iss(line);
        int u,v;
        iss>>u>>v;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }
    file.close();

    cnf my_cnf(n,adj,k1,k2);

    std::ofstream out_file;
    out_file.open(parameter+".satinput", std::ios::out | std::ios::trunc);
    out_file<<'p'<<" "<<"cnf"<<" "<<my_cnf.new_var-1<<" "<<my_cnf.clauses.size()<<"\n";
    for(int i = 0;i<my_cnf.clauses.size();i++){
        for(auto var: my_cnf.clauses[i]){
            out_file<<var<<" ";
        }
        out_file<<0<<"\n";
    }
    out_file.close();

    return 0;

}