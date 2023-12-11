#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <iomanip>
#include <math.h>
#include <chrono>
#include <unordered_map>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:

vector <vector<string>> train_data;
vector <vector<float>> ques_mark;

struct Graph_Node{
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    //vector<string> encode;
    map<string,int> decode;

	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
		nvalues=n;
		values=vals;
	}
    float get_prob(string code){
        return CPT[decode[code]];
    }
	string get_name()
	{
		return Node_Name;
	}
	int get_nvalues()
	{
		return nvalues;
	}

	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	vector<string> get_values()
	{
		return values;
	}
    // vector<string> get_encode(){
    //     return encode;
    // }
    map<string,int> get_decode(){
        return decode;
    }

    // void set_encode(vector<string> &new_encode){
    //     encode.clear();
    //     swap(this->encode,new_encode);
    // }
    void set_decode(map<string,int> &new_decode){
        decode.clear();
        swap(this->decode,new_decode);
    }
	void set_CPT(vector<float> &new_CPT)
	{
		CPT.clear();
        swap(this->CPT,new_CPT);
	}
    void set_Parents(vector<string> &Parent_Nodes)
    {
        Parents.clear();
        swap(this->Parents,Parent_Nodes);
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }
};


 // The whole network represted as a list of nodes
struct network{
	vector<Graph_Node> Pres_Graph;
    unordered_map<string,int> Pres_Graph2; // string to index mapping of graph nodes
	
    void addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
        Pres_Graph2[node.get_name()] = Pres_Graph.size()-1;
	}
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        if(Pres_Graph2.find(val_name)==Pres_Graph2.end()) return -1;
        else return Pres_Graph2[val_name];
    }
        // get the node at nth index
        vector<Graph_Node>::iterator get_nth_node(int n){
        vector<Graph_Node>::iterator it = Pres_Graph.begin();
        advance(it,n);
        return it;
    }
    //get the iterator of a node with a given name
    vector<Graph_Node>::iterator search_node(string val_name)
    {
        vector<Graph_Node>::iterator it;
        int idx = get_index(val_name);
        if(idx==-1) {
            cout<<"node not found\n";
            return it;
        }
        it = Pres_Graph.begin();
        advance(it,idx);
        return it;
    }
};

network read_network(string file_name)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(file_name); 
  	string temp;
  	string name;
  	vector<string> values;
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		ss.str(line);
     		ss>>temp;
     		if(temp.compare("variable")==0)
     		{
     				ss>>name;
     				getline (myfile,line);
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					ss2>>temp;
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				Alarm.addNode(new_node);
     		}
     		else if(temp.compare("probability")==0)
     		{
     				ss>>temp;
     				ss>>temp;
                    vector<Graph_Node>::iterator listIt;
                    vector<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					ss>>temp;
    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
     				ss2.str(line);
     				ss2>> temp;
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
     					curr_CPT.push_back(atof(temp.c_str()));
     					ss2>>temp;
    				}
                    listIt->set_CPT(curr_CPT);
     		}
            else
            {
            }
    	}
    	if(find==1)
    	myfile.close();
  	}
  	return Alarm;
}


void read_data(string dat_file){
    string line;
    ifstream myfile(dat_file);
    string temp;
    if(myfile.is_open()){
        while(!myfile.eof()){
            stringstream ss;
            getline(myfile,line);
            ss.str(line);
            vector<string> dpt;
            while(ss>>temp){
                dpt.push_back(temp);
            }
            train_data.push_back(dpt);
        }
    }
}

void increment(vector<int> &v, vector<int> &MOD){
    int carry = 1;
    int n = v.size();
    for(int i = n-1;i>=0;i--){
        int temp = v[i];
        v[i] = (carry+temp)%MOD[i];
        carry = (carry + temp)/MOD[i];
    }
}

float update(network &Alarm){
    float res = 0.0;
    for(int node = 0;node<Alarm.netSize();node++){
        auto curr = Alarm.get_nth_node(node);
        auto parents = curr->get_Parents();
        auto curr_nvalue = curr->get_nvalues();
        auto curr_values = curr->get_values();
        auto old_cpt = curr->get_CPT();
        vector<int> v(parents.size() + 1,0); //number of descrete values of curr and its parents;
        vector<int> MOD; //number of descrete values of curr and its parents;
        vector<vector<string>> values;
        vector<int> indices;
        indices.push_back(Alarm.get_index(curr->get_name()));
        MOD.push_back(curr_nvalue);
        values.push_back(curr_values);
        int cpt_size = curr_nvalue;
        for(auto par: parents){
            auto iter = Alarm.search_node(par);
            MOD.push_back(iter->get_nvalues());
            values.push_back(iter->get_values());
            indices.push_back(Alarm.get_index(par));
            cpt_size*=(iter->get_nvalues());
        }
       
        vector<float> count(cpt_size,0.0);
        vector<float> cpt(cpt_size);

        for(int i = 0;i<cpt_size;i++){
            for(int j = 0;j<train_data.size();j++){
                float wt = 1.0;
                int found = 1;
                for(int k = 0;k<v.size();k++){
                    if(train_data[j][indices[k]].compare(values[k][v[k]]) == 0){
                        continue;
                    }
                    else if(train_data[j][indices[k]].compare("\"?\"") == 0){
                        wt = ques_mark[j][v[k]];
                        // if(ques_mark[j].size() <= v[k]) cout <<ques_mark[j].size() - v[k] <<"\n";
                    }
                    else found = 0;
                }
                count[i]+= wt*((float)found);
            }
            count[i]+=1e-4; //smoothing
            increment(v,MOD);
        }

        for(int i = 0;i<(cpt_size/curr_nvalue);i++){
            int temp = (cpt_size/curr_nvalue);
            float sum = 0.0;
            for(int j = i;j<cpt_size;j+=temp){
                sum+=count[j];
            }
            for(int j = i;j<cpt_size;j+=temp){
                cpt[j] = count[j]/sum;
                if(cpt[j]<0.0001) cpt[j]=0.0;
                else if(cpt[j]>0.9999) cpt[j]=1.0;
                // cpt[j] = fmax(count[j]/sum,0.01);
                // cpt[j] = fmin(cpt[j],0.99);
            }
        }

        for(int i = 0;i<cpt_size;i++){
            float temp = cpt[i] - old_cpt[i];
            if(temp < 0) temp = -temp;
            if(temp > res){
                res = temp;
            }
        }
        
        curr->set_CPT(cpt);
   }
   return res;
}

bool time_check(std::chrono::high_resolution_clock::time_point& start_time){
    auto cur_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::seconds>(cur_time-start_time);
    if(duration.count()>115)
    {
        return 0;
    }
    return 1;
}

void train(network &Alarm, std::chrono::high_resolution_clock::time_point& start_time){
   for(auto &x : train_data){
        vector<float> temp;
        for(int i = 0;i<Alarm.netSize();i++){
            if(x[i].compare("\"?\"") == 0){
                auto iter = Alarm.get_nth_node(i);
                int nvalues = iter->get_nvalues();
                for(int j = 0;j<nvalues;j++){
                    temp.push_back(1.0/((float)nvalues));
                }
                break;
            }
        }
        ques_mark.push_back(temp);
   }


   //initialize the cpts
   for(int node = 0;node<Alarm.netSize();node++){
        auto curr = Alarm.get_nth_node(node);
        auto parents = curr->get_Parents();
        auto curr_nvalue = curr->get_nvalues();
        auto curr_values = curr->get_values();
        vector<int> v(parents.size() + 1,0); //number of descrete values of curr and its parents;
        vector<int> MOD; //number of descrete values of curr and its parents;
        vector<vector<string>> values;
        vector<int> indices;
        indices.push_back(Alarm.get_index(curr->get_name()));
        MOD.push_back(curr_nvalue);
        values.push_back(curr_values);
        int cpt_size = curr_nvalue;
        for(auto par: parents){
            auto iter = Alarm.search_node(par);
            MOD.push_back(iter->get_nvalues());
            values.push_back(iter->get_values());
            indices.push_back(Alarm.get_index(par));
            cpt_size*=(iter->get_nvalues());
        }
        //vector<string> encode(cpt_size);
        map<string,int> decode;

        vector<float> count(cpt_size,0.0);
        vector<float> cpt(cpt_size);

        for(int i = 0;i<cpt_size;i++){
            string temp = "";
            for(auto x : v){
                temp+=to_string(x);
            }
            //encode[i] = temp;
            decode[temp] = i;
            // cout<<temp<<"\n";
            for(int j = 0;j<train_data.size();j++){
                float wt = 1.0;
                int found = 1;
                for(int k = 0;k<v.size();k++){
                    if(train_data[j][indices[k]].compare(values[k][v[k]]) == 0){
                        continue;
                    }
                    else if(train_data[j][indices[k]].compare("\"?\"") == 0){
                        wt = ques_mark[j][v[k]];
                        // if(ques_mark[j].size() <= v[k]) cout <<ques_mark[j].size() - v[k] <<"\n";
                    }
                    else found = 0;
                }
                count[i]+= wt*((float)found);
            }
            count[i]+=1e-4; //smoothing
            increment(v,MOD);
        }

        for(int i = 0;i<(cpt_size/curr_nvalue);i++){
            int temp = (cpt_size/curr_nvalue);
            float sum = 0.0;
            for(int j = i;j<cpt_size;j+=temp){
                sum+=count[j];
            }
            for(int j = i;j<cpt_size;j+=temp){
                cpt[j] = count[j]/sum;
                if(cpt[j]<0.001) cpt[j]=0.0;
                else if(cpt[j]>0.999) cpt[j]=1.0;
                // cpt[j] = fmax(count[j]/sum,0.01);
                // cpt[j] = fmin(cpt[j],0.99);
            }
        }
        
        curr->set_CPT(cpt);
        //curr->set_encode(encode);
        curr->set_decode(decode);
   }


    //training...
    float prev_res = -1;
    for(int epoch = 0;epoch<1000;epoch++){
        for(int j = 0;j<train_data.size();j++){
            if(ques_mark[j].size() == 0) continue;
            vector<Graph_Node>::iterator curr;
            for(int k = 0;k<Alarm.netSize();k++){
                if(train_data[j][k].compare("\"?\"") == 0){
                    curr = Alarm.get_nth_node(k);
                    break;
                }
            }
            auto children = curr->get_children();
            auto parents = curr->get_Parents();
            vector<vector<string>> child_par;
            for(auto x : children){
                auto iter = Alarm.get_nth_node(x);
                child_par.push_back(iter->get_Parents());
            }
            float sum = 0.0;
            for(int k = 0;k<ques_mark[j].size();k++){
                float prob = 1.0;
                string code1 = to_string(k);
                for(auto par: parents){
                    int idx = Alarm.get_index(par);
                    auto iter = Alarm.search_node(par);
                    auto par_values = iter->get_values();
                    int val_idx = 0;
                    for(;val_idx<par_values.size();val_idx++){
                        if(par_values[val_idx].compare(train_data[j][idx]) == 0) break;
                    }
                    code1+=to_string(val_idx);
                }
                prob = curr->get_prob(code1);
                for(int l = 0;l<children.size();l++){
                    auto child_iter = Alarm.get_nth_node(children[l]);
                    auto child_values = child_iter->get_values();
                    int child_val_idx = 0;
                    for(;child_val_idx<child_values.size();child_val_idx++){
                        if(child_values[child_val_idx].compare(train_data[j][children[l]]) == 0){
                            break;
                        }
                    }
                    string code2 = to_string(child_val_idx);
                    for(auto par: child_par[l]){
                        int idx = Alarm.get_index(par);
                        if(train_data[j][idx].compare("\"?\"") == 0){
                            code2+=to_string(k);
                            continue;
                        }
                        auto iter = Alarm.search_node(par);
                        auto par_values = iter->get_values();
                        int val_idx = 0;
                        for(;val_idx<par_values.size();val_idx++){
                            if(par_values[val_idx].compare(train_data[j][idx]) == 0) break;
                        }
                        code2+=to_string(val_idx);
                    }
                    prob*=child_iter->get_prob(code2);
                }
                ques_mark[j][k] = prob;
                sum+=prob;
            }
            for(int k = 0;k<ques_mark[j].size();k++){
                ques_mark[j][k] = ques_mark[j][k]/sum;
            }
        }
        float res = update(Alarm);
        //cout<<"Epoch: "<<epoch+1<<","<<"residual: "<<res<<"\n";
        if((res < 1e-16) || (!time_check(start_time)) ||(prev_res == res)) break;
        prev_res = res;
    }
}

void print_tables(network &Alarm){
    for(int i = 0;i<Alarm.netSize();i++){
        auto iter = Alarm.get_nth_node(i);
        auto cpt = iter->get_CPT();
        for(auto x: cpt){
            cout<<x<<" ";
        }
        cout<<"\n";
    }
}

float get_loss(network &Alarm, network &Alarm_true){
    float loss = 0.0;
    for(int i = 0;i<Alarm.netSize();i++){
        auto iter = Alarm.get_nth_node(i);
        auto cpt = iter->get_CPT();
        auto iter_true = Alarm_true.get_nth_node(i);
        auto cpt_true = iter_true->get_CPT();
        for(int j = 0;j<cpt.size();j++){
            float temp = cpt[j] - cpt_true[j];
            if(temp < 0) temp = -temp;
            loss+=temp;
        }
    }
    return loss;
}


void bif(network &Alarm, string bayes_net){
    ofstream out_file;
    out_file.open("solved_alarm.bif", std::ios::out | std::ios::trunc);
    string line;
    ifstream myfile(bayes_net);
    string temp;
    if(myfile.is_open()){
        bool start = 1;
        int i = 0;
        while(!myfile.eof()){
            stringstream ss;
            getline(myfile,line);
            ss.str(line);
            ss>>temp;
            if(!start){
                out_file<<"\n";
            }
            else start = 0;
            // if(i<Alarm.netSize())out_file<<line<<"\n";
            // else out_file<<line;
            out_file<<line;
            if(temp.compare("probability") != 0) continue;
            else{
                out_file<<"\n";
                auto curr = Alarm.get_nth_node(i);
                // string name = curr->get_name();
                // auto parents = curr->get_Parents();
                auto cpt = curr->get_CPT();
                // int nvalue = curr->get_nvalues();
                out_file<<"\ttable ";
                out_file<<fixed<<setprecision(4);
                for(auto prob:cpt){
                    prob = fmax(prob,0.01);
                    prob = fmin(prob,0.99);
                    out_file<<(float)((int)(prob*10000))/10000<<" ";
                }
                out_file<<";";
                i++;
                getline(myfile,line);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    auto start_time=std::chrono::high_resolution_clock::now();
	network Alarm;
    string bayes_net = argv[1];
    string dat_file = argv[2];
	Alarm=read_network(bayes_net);
    // Alarm_true = read_network("gold_alarm.bif");
    read_data(dat_file);
    
    train(Alarm,start_time);
    bif(Alarm,bayes_net);

    cout<<"solved_alarm.bif produced \n";
    auto curr_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::seconds>(curr_time-start_time);
    cout<<"Time taken: "<<duration.count()<<" seconds"<<endl;
    //print_tables(Alarm);

    // float loss = get_loss(Alarm,Alarm_true);
    // cout<<"Loss: "<<loss<<"\n";
    // cout<<(train_data[0][0].compare("\"False\"") == 0);
    
    return 0;
}