#include <cassert>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdint>

class cnf{
    public : 

    int32_t n,k1,k2,new_var,p;

    std::vector<std::vector<int32_t>> clauses;

    cnf(int32_t n, std::vector<std::vector<int32_t>>& adj, int32_t k1, int32_t k2) : n(n), k1(k1), k2(k2){
        new_var = 2*n + 1; // first n variables = G11,G12,...,G1n next n variables = G21,G22,...,G2n
        p = ceil(log2((double)n)) + 2; // remove later

        //####################### constraints ##################################
        full_connection(adj); // G1i and G1j => eij, G2i and G2j => eij
        no_common_nodes(); // ~(G1i and G2i) 
        

        //adder circuits

        std::vector<int32_t> add1;
        add1.emplace_back(1);
        for(int32_t i = 2;i<=n;i++){
            std::vector<int32_t> temp;
            temp.emplace_back(i);
            while(temp.size() != add1.size()){
                int32_t zero = new_var++;
                clauses.emplace_back(std::vector<int32_t>({-zero}));
                temp.emplace_back(zero);
            }
            add1 = p_bit_adder(add1,temp);
        }


        std::vector<int32_t> add2;
        add2.emplace_back(n+1);
        for(int32_t i =n+2;i<=2*n;i++){
            std::vector<int32_t> temp;
            temp.emplace_back(i);
            while(temp.size() != add2.size()){
                int32_t zero = new_var++;
                clauses.emplace_back(std::vector<int32_t>({-zero}));
                temp.emplace_back(zero);
            }
            add2 = p_bit_adder(add2,temp);
        }


        CMP(add1,k1); //Number of nodes in the first sub graph should be equal to k1
        CMP(add2,k2); //Number of nodes in the second sub graph should be equal to k2

        return;

    } 

    void AND_CNF(int32_t c, int32_t a, int32_t b){
        clauses.emplace_back(std::vector<int32_t>({-a,-b,c}));
        clauses.emplace_back(std::vector<int32_t>({a,-c}));
        clauses.emplace_back(std::vector<int32_t>({b,-c}));
        return;
    }

    void OR_CNF(int32_t c, int32_t a, int32_t b){
        clauses.emplace_back(std::vector<int32_t>({a,b,-c}));
        clauses.emplace_back(std::vector<int32_t>({-a,c}));
        clauses.emplace_back(std::vector<int32_t>({-b,c}));
        return;
    }

    void XOR_CNF(int32_t c, int32_t a, int32_t b){
        clauses.emplace_back(std::vector<int32_t>({-a,-b,-c}));
        clauses.emplace_back(std::vector<int32_t>({a,b,-c}));
        clauses.emplace_back(std::vector<int32_t>({-a,b,c}));
        clauses.emplace_back(std::vector<int32_t>({a,-b,c}));
        return;
    }

    void EQ_CNF(int32_t a, int32_t b){
        clauses.emplace_back(std::vector<int32_t>({-a,b}));
        clauses.emplace_back(std::vector<int32_t>({a,-b}));
        return;
    }

    void full_connection(std::vector<std::vector<int32_t>>& adj){
        for(int32_t i = 1;i<=n;i++){
            for(int32_t j = i+1;j<=n;j++){
                if(adj[i][j] == 0){
                    clauses.emplace_back(std::vector<int32_t>({-i,-j}));
                    clauses.emplace_back(std::vector<int32_t>({-(i+n),-(j+n)}));
                }
            }
        }
        return;
    }

    std::pair<int32_t, int32_t> full_adder(int32_t a, int32_t b, int32_t c){
        int32_t S1,C1,C2,S,C;
        S1 = new_var++;
        C1 = new_var++;
        C2 = new_var++;
        S = new_var++;
        C = new_var++;
        XOR_CNF(S1,a,b);
        AND_CNF(C1,a,b);
        AND_CNF(C2,S1,c);
        XOR_CNF(S,S1,c);
        OR_CNF(C,C1,C2);
        
        return {S,C};
    }

    std:: vector<int32_t> p_bit_adder(std::vector<int32_t>& A, std::vector<int32_t>& B){
        assert(A.size() == B.size());
        std::vector<int32_t> sum;
        int32_t c = new_var++;
        clauses.emplace_back(std::vector<int32_t>({-c}));
        std::pair<int32_t,int32_t> pr;
        for(int32_t i = 0;i<A.size();i++){
            pr = full_adder(A[i],B[i],c);
            sum.emplace_back(pr.first);
            c = pr.second;
        }
        sum.emplace_back(c);
        assert(c == pr.second);
        return sum;
    }


    void CMP(std::vector<int32_t>& add, int32_t k){
        int32_t len = 0;
        int32_t temp = k;
        while(temp>0){
            len++;
            temp = (temp>>1);
        }
        if(k == 0) len = 1;

        while(add.size() < len){
            int32_t zero = new_var++;
            clauses.emplace_back(std::vector<int32_t>({-zero}));
            add.emplace_back(zero);
        }

        assert(p<=31); // remove later

        for(int32_t i = 0;i<std::min((int32_t)add.size(),len);i++){
            if(((((long long)1)<<i)&k)) clauses.emplace_back(std::vector<int32_t>({add[i]}));
            else clauses.emplace_back(std::vector<int32_t>({-add[i]}));
        }
        for(int32_t i = std::min((int32_t)add.size(),len);i<add.size();i++){
            clauses.emplace_back(std::vector<int32_t>({-add[i]}));
        }
        return;
    }
    void no_common_nodes(){
        for(int32_t i = 1;i<=n;i++){
            clauses.emplace_back(std::vector<int32_t>({-i,-(i+n)}));
        }
        return;
    }
};