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
    file.open(input_file);
    std::string line;
    getline(file, line);
    std::istringstream iss1(line);
    int32_t n;
    iss1>>n;
    file.close();
    
    std::string minisat_output = parameter+".satoutput";
    std::string sub_graphs_output = parameter+".mapping";
    file.open(minisat_output);
    getline(file, line);
    std::istringstream iss(line);
    std::string res;
    iss>>res;
    std::vector<int32_t> G1;
    std::vector<int32_t> G2;
    if(res != "UNSAT"){
        while(getline(file, line))
        {
            std::istringstream iss(line);
            for(int32_t i = 1;i<=2*n;i++){
                int32_t temp;
                iss>>temp;
                if(temp>0){
                    if(temp <= n){
                        G1.push_back(temp);
                    }
                    else{
                        G2.push_back(temp-n);
                    }    
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
        out_file<<"\n";
        out_file<<"#2"<<"\n"<<G2[0];
        for(int32_t i = 1; i<G2.size();i++){
            out_file<<" "<<G2[i];
        }
    }
    out_file.close();

    return 0;
}
