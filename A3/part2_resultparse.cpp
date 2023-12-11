#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>


int main(int32_t argc, char* argv[]){
    //###################### <Input Handling> ############################
    std::string parameter = argv[1];
    std::string input_file = parameter + ".graph";
    
    std::ifstream file;
    std::string line;

    int32_t n;
    n = atoi(argv[2]);

    std::string minisat_output = parameter+".satoutput";
    std::string sub_graphs_output = parameter+".mapping";
    file.open(minisat_output);
    getline(file, line);
    std::istringstream iss(line);
    std::string res;
    iss>>res;
    std::vector<int32_t> G1;
    if(res != "UNSAT"){
        while(getline(file, line))
        {
            std::istringstream iss(line);
            for(int32_t i = 1;i<=n;i++){
                int32_t temp;
                iss>>temp;
                if(temp>0){
                    G1.push_back(temp);
                }
                
            }
        }
    }
        
    std::ofstream out_file;
    out_file.open(sub_graphs_output, std::ios::out | std::ios::trunc);
    if(res == "UNSAT") out_file<<0;
    else{
        out_file<<"#1"<<"\n"<<G1[0];
        for(int32_t i = 1; i<G1.size();i++){
            out_file<<" "<<G1[i];
        }
        
    }
    out_file.close();

    return 0;
}
