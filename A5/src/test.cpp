#include <iostream>
// #include "butils.hpp"
// #include "engine.hpp"
#include <vector>
#include <random>
#include <cmath>
#include <chrono>
using namespace std;

int testEncodingFunctions()
{
//     // Engine player;
//     // Board b;
//     // unsigned long long int val = player.pawn_Encoding(b);
//     // U8 blackPawn1 = val & 0x3f;   val=val>>6;
//     // U8 blackPawn2 = val & 0x3f;   val=val>>6;
//     // U8 whitePawn1 = val & 0x3f;   val=val>>6;
//     // U8 whitePawn2 = val & 0x3f;

//     // cout<<"Black Pawn 1: "<<getx(blackPawn1)<<" "<<gety(blackPawn1)<<endl;
//     // cout<<"Black Pawn 2: "<<getx(blackPawn2)<<" "<<gety(blackPawn2)<<endl;
//     // cout<<"White Pawn 1: "<<getx(whitePawn1)<<" "<<gety(whitePawn1)<<endl;
//     // cout<<"White Pawn 2: "<<getx(whitePawn2)<<" "<<gety(whitePawn2)<<endl;


//     // b.do_move_(move(pos(2,1),pos(1,2)));
//     // b.do_move_(move(pos(4,5),pos(5,4)));
//     // b.do_move_(move(pos(1,2),pos(0,3)));
//     // b.do_move_(move(pos(5,4),pos(6,3)));
//     // cout<<board_to_str(&b.data)<<endl;

//     // val = player.pawn_Encoding(b);
//     // blackPawn1 = val & 0x3f;   val=val>>6;
//     // blackPawn2 = val & 0x3f;   val=val>>6;
//     // whitePawn1 = val & 0x3f;   val=val>>6;
//     // whitePawn2 = val & 0x3f;

//     // cout<<"Black Pawn 1: "<<getx(blackPawn1)<<" "<<gety(blackPawn1)<<endl;
//     // cout<<"Black Pawn 2: "<<getx(blackPawn2)<<" "<<gety(blackPawn2)<<endl;
//     // cout<<"White Pawn 1: "<<getx(whitePawn1)<<" "<<gety(whitePawn1)<<endl;
//     // cout<<"White Pawn 2: "<<getx(whitePawn2)<<" "<<gety(whitePawn2)<<endl;

    
//     // val=player.state_Encoding(b,10,1);
//     // bool color = val & 1;   val=val>>1;
//     // int depth = val & 0xff; val=val>>8;
//     // cout<<endl;
//     // cout<<"depth="<<depth<<endl;
//     // cout<<"color="<<color<<endl;
//     // U8 blackRook1 = val & 0x3f; val=val>>6;
//     // U8 blackRook2 = val & 0x3f; val=val>>6;
//     // U8 blackBishop = val & 0x3f; val=val>>6;
//     // U8 blackKing = val & 0x3f; val=val>>6; 

//     // U8 whiteRook1 = val & 0x3f; val=val>>6;
//     // U8 whiteRook2 = val & 0x3f; val=val>>6;
//     // U8 whiteBishop = val & 0x3f; val=val>>6;
//     // U8 whiteKing = val & 0x3f; val=val>>6;

//     // cout<<"Black Rook 1: "<<getx(blackRook1)<<" "<<gety(blackRook1)<<endl;
//     // cout<<"Black Rook 2: "<<getx(blackRook2)<<" "<<gety(blackRook2)<<endl;
//     // cout<<"Black Bishop: "<<getx(blackBishop)<<" "<<gety(blackBishop)<<endl;
//     // cout<<"Black King: "<<getx(blackKing)<<" "<<gety(blackKing)<<endl;

//     // cout<<"White Rook 1: "<<getx(whiteRook1)<<" "<<gety(whiteRook1)<<endl;
//     // cout<<"White Rook 2: "<<getx(whiteRook2)<<" "<<gety(whiteRook2)<<endl;
//     // cout<<"White Bishop: "<<getx(whiteBishop)<<" "<<gety(whiteBishop)<<endl;
//     // cout<<"White King: "<<getx(whiteKing)<<" "<<gety(whiteKing)<<endl;

//     // b.do_move_(move(pos(3,0),pos(1,2)));
//     // b.do_move_(move(pos(3,6),pos(5,4)));
//     // b.do_move_(move(pos(4,0),pos(5,0)));
//     // b.do_move_(move(pos(3,5),pos(4,5)));

    
//     // val=player.state_Encoding(b,10,1);
//     // color = val & 1;   val=val>>1;
//     // depth = val & 0xff; val=val>>8;
//     // cout<<endl;
//     // cout<<"depth="<<depth<<endl;
//     // cout<<"color="<<color<<endl;
//     // blackRook1 = val & 0x3f; val=val>>6;
//     // blackRook2 = val & 0x3f; val=val>>6;
//     // blackBishop = val & 0x3f; val=val>>6;
//     // blackKing = val & 0x3f; val=val>>6; 

//     // whiteRook1 = val & 0x3f; val=val>>6;
//     // whiteRook2 = val & 0x3f; val=val>>6;
//     // whiteBishop = val & 0x3f; val=val>>6;
//     // whiteKing = val & 0x3f; val=val>>6;

//     // cout<<"Black Rook 1: "<<getx(blackRook1)<<" "<<gety(blackRook1)<<endl;
//     // cout<<"Black Rook 2: "<<getx(blackRook2)<<" "<<gety(blackRook2)<<endl;
//     // cout<<"Black Bishop: "<<getx(blackBishop)<<" "<<gety(blackBishop)<<endl;
//     // cout<<"Black King: "<<getx(blackKing)<<" "<<gety(blackKing)<<endl;

//     // cout<<"White Rook 1: "<<getx(whiteRook1)<<" "<<gety(whiteRook1)<<endl;
//     // cout<<"White Rook 2: "<<getx(whiteRook2)<<" "<<gety(whiteRook2)<<endl;
//     // cout<<"White Bishop: "<<getx(whiteBishop)<<" "<<gety(whiteBishop)<<endl;
//     // cout<<"White King: "<<getx(whiteKing)<<" "<<gety(whiteKing)<<endl;

//     // cout<<board_to_str(&b.data)<<endl;
//     // return 0;
}


#define MAT_DEFINE(NAME) std::vector<std::vector<double>> NAME
#define MAT_INIT(NAME,X,Y) std::vector<std::vector<double>> NAME(X,std::vector<double>(Y,0))

class NeuralNetwork
{
    public:
    
    MAT_DEFINE(input);
    MAT_DEFINE(hidden1);
    MAT_DEFINE(hidden2);
    MAT_DEFINE(wts1);
    MAT_DEFINE(wts2);
    MAT_DEFINE(wts3);
    MAT_DEFINE(output);
    NeuralNetwork()
    {
        MAT_INIT(temp_input,64,1);    //+ve for my color piece, -ve for opp color piece
        MAT_INIT(temp_hidden1,128,1);
        MAT_INIT(temp_hidden2,128,1);
        MAT_INIT(temp_output,1,1);
        MAT_INIT(temp_wts1,128,64);
        MAT_INIT(temp_wts2,128,128);
        MAT_INIT(temp_wts3,1,128);

        input=temp_input;
        hidden1=temp_hidden1;
        hidden2=temp_hidden2;
        wts1=temp_wts1;
        wts2=temp_wts2;
        wts3=temp_wts3;
        output=temp_output;
    }


    static bool equalDim(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B)
    {
        if(A.size()!=B.size())
            return false;
        if(A.size()==0)
            return true;
        if(A[0].size()!=B[0].size())
            return false;
        return true;
    }

    static void matAdd(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B,std::vector<std::vector<double>>& C)
    {
        if(equalDim(A,B) && equalDim(B,C))
        {
            for(int x=0;x<A.size();x++)
            {
                for(int y=0;y<A[x].size();y++)
                {
                    C[x][y]=A[x][y]+B[x][y];
                }
            }
        }   
        else
            std::cout<<"Not of same dimension"<<std::endl; 
    }

    static void matSub(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B,std::vector<std::vector<double>>& C)
    {
        if(equalDim(A,B) && equalDim(B,C))
        {
            for(int x=0;x<A.size();x++)
            {
                for(int y=0;y<A[x].size();y++)
                {
                    C[x][y]=A[x][y]-B[x][y];
                }
            }
        }   
        else
            std::cout<<"Not of same dimension"<<std::endl; 
    }

    static void matMult(std::vector<std::vector<double>>& A,std::vector<std::vector<double>>& B,std::vector<std::vector<double>>& C)
    {
        if(A.size()==0 || B.size()==0 ||C.size()==0)
        {
            std::cout<<"Invalid dimensions"<<std::endl;
            return;
        }

        if(A[0].size() != B.size() || A.size() !=C.size() || B[0].size()!=C[0].size())
        {
            std::cout<<"Invalid dimensions"<<std::endl;
            return;
        }
        
        for(int x=0;x<C.size();x++)
        {
            for(int y=0;y<C[x].size();y++)
            {
                C[x][y]=0;
                for(int z=0;z<A[x].size();z++)
                {
                    C[x][y]+=A[x][z]*B[z][y];
                }
            }
        }
    }

    static double ReLU(std::vector<std::vector<double>>& matrix)
    {
        for(int x=0;x<matrix.size();x++)
        {
            for(int y=0;y<matrix[x].size();y++)
            {
                if(matrix[x][y]<0)
                    matrix[x][y]=0;
            }
        }
    }

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

    static void scalarMult(std::vector<std::vector<double>>& mat, double val)
    {
        for(int x=0;x<mat.size();x++)
        {
            for(int y=0;y<mat[x].size();y++)
            {
                mat[x][y]*=val;
            }
        }
    }

    double inference(std::vector<double>& in)
    {
        //take board as parameter and set the input
        for(int x=0;x<input.size();x++)
            input[x][0]=in[x];

        matMult(wts1,input,hidden1);
        ReLU(hidden1);
        matMult(wts2,hidden1,hidden2);
        ReLU(hidden2);
        matMult(wts3,hidden2,output);        
        return output[0][0];
    }

    void update_wts(std::vector<double>& in, double ans)
    {
        double temp_ans = inference(in);

        double deri_out = 2*(temp_ans-ans);     //derivation of loss, wrt network output
        
        MAT_INIT(deri_wts3,1,128);
        transpose(hidden2,deri_wts3);
        scalarMult(deri_wts3,deri_out);

        MAT_INIT(deri_hidden2,128,1);
        MAT_DEFINE(temp1);
        temp1=wts3;
        scalarMult(temp1,deri_out);
        transpose(temp1,deri_hidden2);
        ReLU(deri_hidden2);

// d(L)/d(in) = trans( trans(d(L)/d(out)) *  wts  )

        MAT_INIT(deri_wts2,128,128);
        MAT_INIT(temp2,1,128);
        transpose(hidden1,temp2);
        matMult(deri_hidden2,temp2,deri_wts2);

        MAT_INIT(deri_hidden1,128,1);
        MAT_INIT(temp3,1,128);
        MAT_INIT(temp4,1,128);
        transpose(deri_hidden2,temp3);
        matMult(temp3,wts2,temp4);
        transpose(temp4,deri_hidden1);
        ReLU(deri_hidden1);

        MAT_INIT(deri_wts1,128,64);
        MAT_INIT(temp5,1,64);
        transpose(input,temp5);
        matMult(deri_hidden1,temp5,deri_wts1);

        double learn_rate = 0.000001;
        scalarMult(deri_wts1,learn_rate);
        scalarMult(deri_wts2,learn_rate);
        scalarMult(deri_wts3,learn_rate);

        // cout<<"WTS1"<<endl;
        // for(int x=0;x<deri_wts1.size();x++)
        // {
        //     for(int y=0;y<deri_wts1[x].size();y++)
        //     {
        //         cout<<deri_wts1[x][y]<<" ";
        //     }
        //     cout<<endl;
        // }
        // cout<<"WTS2"<<endl;
        // for(int x=0;x<deri_wts2.size();x++)
        // {
        //     for(int y=0;y<deri_wts2[x].size();y++)
        //     {
        //         cout<<deri_wts2[x][y]<<" ";
        //     }
        //     cout<<endl;
        // }
        // cout<<"WTS3"<<endl;
        // for(int x=0;x<deri_wts3.size();x++)
        // {
        //     for(int y=0;y<deri_wts3[x].size();y++)
        //     {
        //         cout<<deri_wts3[x][y]<<" ";
        //     }
        //     cout<<endl;
        // }

        MAT_INIT(updated_wts1,128,64);
        MAT_INIT(updated_wts2,128,128);
        MAT_INIT(updated_wts3,1,128);

        matSub(wts1,deri_wts1,updated_wts1);
        matSub(wts2,deri_wts2,updated_wts2);
        matSub(wts3,deri_wts3,updated_wts3);

        wts1=updated_wts1;
        wts2=updated_wts2;
        wts3=updated_wts3;
    }

    static void randomizeWeights(std::vector<std::vector<double>>& wts)
    {
        int max=100000;
        std::random_device var1;
        std::mt19937 var2(var1());
        std::uniform_int_distribution<int>  var3(0, max);
        for(int x=0;x<wts.size();x++)
        {
            for(int y=0;y<wts[x].size();y++)
            {
                wts[x][y]=var3(var2);
                wts[x][y]/=max;
                wts[x][y]*=2;
                wts[x][y]-=1;
            }
        }
    }
};


void testMatrix()
{

    // double wts1[]
    // MAT(A,2,3);
    // MAT(B,3,2);
    // MAT(C,2,2);
    // for(int x=0;x<A.size();x++)
    //     for(int y=0;y<A[x].size();y++)
    //         A[x][y]=x+y;
    // for(int x=0;x<B.size();x++)
    //     for(int y=0;y<B[x].size();y++)
    //         B[x][y]=x+y;
    // matMult(A,B,C);
    // for(int x=0;x<C.size();x++)
    // {
    //     for(int y=0;y<C[x].size();y++)
    //     {
    //         std::cout<<C[x][y]<<" ";
    //     }
    //     std::cout<<std::endl;
    // }
}


void neuralNet()
{
    NeuralNetwork net;
    net.randomizeWeights(net.wts1);
    net.randomizeWeights(net.wts2);
    net.randomizeWeights(net.wts3);

    int max=100000;
    std::random_device var1;
    std::mt19937 var2(var1());
    std::uniform_int_distribution<int>  var3(0, max);

    
    std::vector<double> temp_vect(64,0);

    vector<pair<double,double>> dataset;
    for(int x=0;x<100;x++)
    {
        double input = var3(var2)/((double)max)*3.14 - 1.57;
        double output = sin(input);
        dataset.push_back(pair<double,double>(input,output));
    }


    for(int epoch=0;epoch<50;epoch++)
    {
        double error=0;
        for(auto p:dataset)
        {
            for(int x=0;x<temp_vect.size();x++)
                temp_vect[x]=p.first;
            
            double output = net.inference(temp_vect);

            error += (output-p.second)*(output-p.second);

            net.update_wts(temp_vect,p.second);

            // cout<<"1 to proceed"<<endl;
            // int temp;
            // cin>>temp;
        }        
        cout<<"Epoch = "<<epoch<<"   Error = "<<error<<endl;
    }

    for(int x=0;x<64;x++)
        temp_vect[x]=3.14/6;
    
    double val = net.inference(temp_vect);
    cout<<"Output of neural net for pi/6 input = "<<val<<endl;

}

void testTransTable()
{
//  Engine player1,player2;
//     Board b;

//     player1.find_best_move(b);
//     b.do_move_(player1.best_move);
//     std::cerr<<"Move 1.1 done"<<std::endl;
//     player2.find_best_move(b);
//     b.do_move_(player2.best_move);
//     std::cerr<<"Move 1.2 done"<<std::endl;

//     player1.find_best_move(b);
//     b.do_move_(player1.best_move);
//     std::cerr<<"Move 2.1 done"<<std::endl;
//     player2.find_best_move(b);
//     b.do_move_(player2.best_move);
//     std::cerr<<"Move 2.2 done"<<std::endl;
//     player1.find_best_move(b);
//     b.do_move_(player1.best_move);
//     std::cerr<<"Move 3.1 done"<<std::endl;

//     player2.find_best_move(b);
//     b.do_move_(player2.best_move);
//     std::cerr<<"Move 3.2 done"<<std::endl;
//     player1.find_best_move(b);
//     b.do_move_(player1.best_move);
//     std::cerr<<"Move 4.1 done"<<std::endl;

//     player2.find_best_move(b);
//     b.do_move_(player2.best_move);
//     std::cerr<<"Move 4.2 done"<<std::endl;
//     player1.find_best_move(b);
//     b.do_move_(player1.best_move);
//     std::cerr<<"Move 5.1 done"<<std::endl;


//     // player1.printTable();

//     // std::cout<<"\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"<<std::endl;
//     // player2.printTable();
}

int main()
{
    // neuralNet();
    
    MAT_INIT(input,320,1);
    MAT_INIT(wts1,512,320);
    MAT_INIT(wts2,256,512);
    MAT_INIT(wts3,25,256);
    MAT_INIT(wts4,1,25);
    
    MAT_INIT(temp1,512,1);
    MAT_INIT(temp2,256,1);
    MAT_INIT(temp3,25,1);
    MAT_INIT(out,1,1);

    

    NeuralNetwork::randomizeWeights(wts1);
    NeuralNetwork::randomizeWeights(wts2);
    NeuralNetwork::randomizeWeights(wts3);
    NeuralNetwork::randomizeWeights(wts4);
    NeuralNetwork::randomizeWeights(input);

    auto time1 = std::chrono::high_resolution_clock::now();

    NeuralNetwork::matMult(wts1,input,temp1);
    NeuralNetwork::matMult(wts2,temp1,temp2);
    NeuralNetwork::matMult(wts3,temp2,temp3);
    NeuralNetwork::matMult(wts4,temp3,out);

    auto time2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time2-time1);

    cout<<duration.count()<<endl;
    cout<<out[0][0]<<endl;
}
