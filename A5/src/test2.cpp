#include "weights.hpp"
#include <iostream>
// #include <vector>
#include <random>
#include <chrono>
#include "NeuralNet.hpp"
using namespace std;

    
    // static bool equalDim(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B)
    // {
    //     if(A.size()!=B.size())
    //         return false;
    //     if(A.size()==0)
    //         return true;
    //     if(A[0].size()!=B[0].size())
    //         return false;
    //     return true;
    // }

    // static void matAdd(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B,std::vector<std::vector<double>>& C)
    // {
    //     if(equalDim(A,B) && equalDim(B,C))
    //     {
    //         for(int x=0;x<A.size();x++)
    //         {
    //             for(int y=0;y<A[x].size();y++)
    //             {
    //                 C[x][y]=A[x][y]+B[x][y];
    //             }
    //         }
    //     }   
    //     else
    //         std::cout<<"Not of same dimension"<<std::endl; 
    // }

    // static void matSub(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B,std::vector<std::vector<double>>& C)
    // {
    //     if(equalDim(A,B) && equalDim(B,C))
    //     {
    //         for(int x=0;x<A.size();x++)
    //         {
    //             for(int y=0;y<A[x].size();y++)
    //             {
    //                 C[x][y]=A[x][y]-B[x][y];
    //             }
    //         }
    //     }   
    //     else
    //         std::cout<<"Not of same dimension"<<std::endl; 
    // }

    // static void matMult(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B,std::vector<std::vector<double>>& C)
    // {
    //     if(A.size()==0 || B.size()==0 ||C.size()==0)
    //     {
    //         std::cout<<"Invalid dimensions"<<std::endl;
    //         return;
    //     }

    //     if(A[0].size() != B.size() || A.size() !=C.size() || B[0].size()!=C[0].size())
    //     {
    //         std::cout<<"Invalid dimensions"<<std::endl;
    //         return;
    //     }
        
    //     for(int x=0;x<C.size();x++)
    //     {
    //         for(int y=0;y<C[x].size();y++)
    //         {
    //             C[x][y]=0;
    //             for(int z=0;z<A[x].size();z++)
    //             {
    //                 C[x][y]+=A[x][z]*B[z][y];
    //             }
    //         }
    //     }
    // }

    // static double ReLU(std::vector<std::vector<double>>& matrix)
    // {
    //     for(int x=0;x<matrix.size();x++)
    //     {
    //         for(int y=0;y<matrix[x].size();y++)
    //         {
    //             if(matrix[x][y]<0)
    //                 matrix[x][y]=0;
    //         }
    //     }
    // }


    // static void randomizeWeights(std::vector<std::vector<double>>& wts)
    // {
    //     int max=100000;
    //     std::random_device var1;
    //     std::mt19937 var2(var1());
    //     std::uniform_int_distribution<int>  var3(0, max);
    //     for(int x=0;x<wts.size();x++)
    //     {
    //         for(int y=0;y<wts[x].size();y++)
    //         {
    //             wts[x][y]=var3(var2);
    //             wts[x][y]/=max;
    //             wts[x][y]*=2;
    //             wts[x][y]-=1;
    //         }
    //     }
    // }


    static void transpose(std::vector<std::vector<double>>& in, std::vector<std::vector<double>>& out)
    {
        if(in.size()==0 || out.size()==0)
            std::cout<<"Empty matrix"<<std::endl;
        if(in.size()!=out[0].size() || in[0].size()!=out.size())
            std::cout<<"Invalid dim"<<std::endl;
        
        for(int x=0;x<in.size();x++)
        {
            for(int y=0;y<in[x].size();y++)
            {
                out[y][x]=in[x][y];
            }
        }
    }


#define MAT_DEFINE(NAME) std::vector<std::vector<double>> NAME
#define MAT_INIT(NAME,X,Y) std::vector<std::vector<double>> NAME(X,std::vector<double>(Y,0))


int main()
{
    // MAT_INIT(input,320,1);
    // MAT_INIT(hidden1,400,1);
    // MAT_INIT(hidden2,128,1);
    // MAT_INIT(hidden3,16,1);
    // MAT_INIT(output,1,1);

    // transpose(temp_input,input);    Mstric 

    Matrix input(temp_input);
    Matrix h1_wts(hidden1_wts);
    Matrix h2_wts(hidden2_wts);
    Matrix h3_wts(hidden3_wts);
    Matrix out_wts(output_wts);

    Matrix h1_bias(hidden1_bias);
    Matrix h2_bias(hidden2_bias);
    Matrix h3_bias(hidden3_bias);
    Matrix out_bias(output_bias);

    input = Matrix::transpose(input);

    auto time1 = std::chrono::high_resolution_clock::now();

    Matrix x = Matrix::matmul(h1_wts,input);
    x=Matrix::add(x,h1_bias);
    x=Activations::relu(x);

    x = Matrix::matmul(h2_wts,x);
    x=Matrix::add(x,h2_bias);
    x=Activations::relu(x);

    x = Matrix::matmul(h3_wts,x);
    x=Matrix::add(x,h3_bias);
    x=Activations::relu(x);

    x = Matrix::matmul(out_wts,x);
    x=Matrix::add(x,out_bias);
    auto time2 = std::chrono::high_resolution_clock::now();
    
    cout<<x.mat[0][0]<<endl;


    // matMult(hidden1_wts,input,hidden1);
    // matAdd(hidden1,hidden1_bias,hidden1);
    // ReLU(hidden1);

    // matMult(hidden2_wts,hidden1,hidden2);
    // matAdd(hidden2,hidden2_bias,hidden2);
    // ReLU(hidden2);

    // matMult(hidden3_wts,hidden2,hidden3);
    // matAdd(hidden3,hidden3_bias,hidden3);
    // ReLU(hidden3);

    // matMult(output_wts,hidden3,output);
    // matAdd(output,output_bias,output);

    
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1);
    // cout<<output[0][0]<<endl;
    cout<<duration.count()<<endl;
    return 0;
}
