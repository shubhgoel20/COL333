#include <algorithm>
#include <random>
#include <iostream>
#include <thread>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

#include <unordered_map>
#include <cassert>

#define FLOAT_MAX 1e20
#define FLOAT_MIN -1e20
// #include "NeuralNet.hpp"
#include "weights.hpp"



int total_searches, found_searches, old_map_searches;
int count1;
std::unordered_map<std::string,float> transTable[2];

class Matrix{
    public:
        std::vector<std::vector<double>> mat;
        std::vector<int> shape;
        Matrix(){}
        Matrix(std::vector<std::vector<double>> mat);
        Matrix(int n,int m);
        Matrix(double f);
        Matrix(std::vector<double> v);
        Matrix(const Matrix &m);
    

        void print_mat(){
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    std::cout<<mat[i][j]<<" ";
                }
                std::cout<<std::endl;
            }
        }

        // void print_mat(std::ofstream &out_file){
        //     for(int i = 0;i<shape[0];i++){
        //         for(int j = 0;j<shape[1];j++){
        //             out_file<<mat[i][j];
        //             if(j != shape[1] - 1) out_file<<" ";
        //         }
        //         out_file<<std::endl;
        //     }
        // }

        double sum(){
            double ans = 0.0;
            for(int i = 0;i<shape[0];i++){
                for(int j = 0;j<shape[1];j++){
                    ans+=mat[i][j];
                }
            }
            return ans;
        }

        static Matrix matmul(Matrix &m1, Matrix &m2);
        static Matrix prod(Matrix &m1, Matrix &m2);
        static Matrix prod(double f, Matrix &m);
        static Matrix add(Matrix &m1, Matrix &m2);
        static Matrix add(double f, Matrix &m);
        static Matrix sub(Matrix &m1, Matrix &m2);
        static Matrix sub(double f, Matrix &m);
        static Matrix transpose(Matrix &m);
};

Matrix::Matrix(std::vector<double> v){
    this->shape.push_back(v.size());
    this->shape.push_back(1);
    // std::cout<<"here"<<"\n";
    std::vector<double> temp;
    for(auto x : v){
        // std::cout<<x<<"\n";
        temp.clear();
        temp.push_back(x);
        this->mat.push_back(temp);
    }
}

Matrix::Matrix(double f){
    this->shape.push_back(1);
    this->shape.push_back(1);
    std::vector<double> temp;
    temp.push_back(f);
    this->mat.push_back(temp);
}

Matrix::Matrix(const Matrix &m){
    this->shape = m.shape;
    std::vector<double> temp;
    for(int i = 0;i<m.shape[0];i++){
        temp.clear();
        for(int j = 0;j<m.shape[1];j++){
            temp.push_back(m.mat[i][j]);
        }
        this->mat.push_back(temp);
    }
}

Matrix::Matrix(std::vector<std::vector<double>> mat){
    this->mat = mat;
    shape.push_back(mat.size());
    shape.push_back(mat[0].size());
}

Matrix::Matrix(int n, int m){
    shape.push_back(n);
    shape.push_back(m);
    std::vector<double> temp;
    for(int i = 0;i<n;i++){
        temp.clear();
        for(int j = 0;j<m;j++){
            temp.push_back((std::rand()/((double)RAND_MAX)));
        }
        this->mat.push_back(temp);        
    }
}

Matrix Matrix::transpose(Matrix &m){
    Matrix res(m.shape[1],m.shape[0]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[j][i] = m.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::matmul(Matrix &m1, Matrix &m2){
    assert(m1.shape[1] == m2.shape[0]);
    Matrix res(m1.shape[0],m2.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m2.shape[1];j++){
            res.mat[i][j] = 0.0;
            for(int k = 0;k<m1.shape[1];k++){
                res.mat[i][j]+=m1.mat[i][k]*m2.mat[k][j];
            }
        }
    }
    return res;
}

Matrix Matrix::prod(Matrix &m1, Matrix &m2){
    assert(m1.shape == m2.shape);
    Matrix res(m1.shape[0],m1.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m1.shape[1];j++){
            res.mat[i][j] = m1.mat[i][j]*m2.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::prod(double f, Matrix &m){
    Matrix res(m.shape[0],m.shape[1]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j]*f;
        }
    }
    return res;
}

Matrix Matrix::add(Matrix &m1, Matrix &m2){
    assert(m1.shape == m2.shape);
    Matrix res(m1.shape[0],m1.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m1.shape[1];j++){
            res.mat[i][j] = m1.mat[i][j]+m2.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::add(double f, Matrix &m){
    Matrix res(m.shape[0],m.shape[1]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j]+f;
        }
    }
    return res;
}

Matrix Matrix::sub(Matrix &m1, Matrix &m2){
    assert(m1.shape == m2.shape);
    Matrix res(m1.shape[0],m1.shape[1]);
    for(int i = 0;i<m1.shape[0];i++){
        for(int j = 0;j<m1.shape[1];j++){
            res.mat[i][j] = m1.mat[i][j]-m2.mat[i][j];
        }
    }
    return res;
}

Matrix Matrix::sub(double f, Matrix &m){
    Matrix res(m.shape[0],m.shape[1]);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = f - m.mat[i][j];
        }
    }
    return res;
}

//########################################### </Matrix Class> #############################

//########################################### <Activations Class> #############################

class Activations{
    public:
        Activations(){}
        static Matrix tanh(Matrix &m);
        static Matrix tanh_grad(Matrix &m);
        static Matrix relu(Matrix &m);
        static Matrix relu_grad(Matrix &m);
};

Matrix Activations::tanh(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = std::tanh(m.mat[i][j]);
        }
    }
    return res;
}

Matrix Activations::relu(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j] > 0 ? m.mat[i][j]:0;
        }
    }
    return res;
}

Matrix Activations::tanh_grad(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = 1 - (m.mat[i][j]*m.mat[i][j]);
        }
    }
    return res;
}

Matrix Activations::relu_grad(Matrix &m){
    Matrix res(m);
    for(int i = 0;i<m.shape[0];i++){
        for(int j = 0;j<m.shape[1];j++){
            res.mat[i][j] = m.mat[i][j] > 0 ? 1.0:0.0;
        }
    }
    return res;
}

Matrix h1_wts(hidden1_wts);
Matrix h2_wts(hidden2_wts);
Matrix h3_wts(hidden3_wts);
Matrix out_wts(output_wts);

Matrix h1_bias(hidden1_bias);
Matrix h2_bias(hidden2_bias);
Matrix h3_bias(hidden3_bias);
Matrix out_bias(output_bias);
    


    
int Engine::rook_count(const Board&b,PlayerColor color, PawnInfo& pi)
{
    int val1=0;
    if(color==WHITE)
    {
        val1=val1 + 
                (b.data.w_rook_1!=DEAD) + 
                (b.data.w_rook_2!=DEAD) + 
                ((pi.white_pawn_1 & ROOK)!=0) +
                ((pi.white_pawn_2 & ROOK)!=0) +
                ((pi.white_pawn_3 & ROOK)!=0) +
                ((pi.white_pawn_4 & ROOK)!=0);
    }
    else
    {
        val1 = val1 +
                (b.data.b_rook_1!=DEAD) +
                (b.data.b_rook_2!=DEAD) + 
                ((pi.black_pawn_1 & ROOK)!=0) +
                ((pi.black_pawn_2 & ROOK)!=0) +
                ((pi.black_pawn_3 & ROOK)!=0) +
                ((pi.black_pawn_4 & ROOK)!=0);
    }
    return val1;
}
int Engine::bishop_count(const Board&b,PlayerColor color, PawnInfo& pi)
{
    int val1=0;
    if(color==WHITE)
    {
        val1=val1 + 
                (b.data.w_bishop!=DEAD) + 
                ((pi.white_pawn_1 & BISHOP)!=0) +
                ((pi.white_pawn_2 & BISHOP)!=0) +
                ((pi.white_pawn_3 & BISHOP)!=0) +
                ((pi.white_pawn_4 & BISHOP)!=0);
    }
    else
    {
        val1 = val1 +
                (b.data.b_bishop!=DEAD) +
                ((pi.black_pawn_1 & BISHOP)!=0) +
                ((pi.black_pawn_2 & BISHOP)!=0) +
                ((pi.black_pawn_3 & BISHOP)!=0) +
                ((pi.black_pawn_4 & BISHOP)!=0);
    }
    return val1;
}
int Engine::pawn_count(const Board&b,PlayerColor color, PawnInfo& pi)
{
    int val1=0;
    if(color==WHITE)
    {
        val1=val1 + 
                ((pi.white_pawn_1)!=0) +
                ((pi.white_pawn_2)!=0) +
                ((pi.white_pawn_3)!=0) +
                ((pi.white_pawn_4)!=0);
    }
    else
    {
        val1 = val1 +
                ((pi.black_pawn_1)!=0) +
                ((pi.black_pawn_2)!=0) +
                ((pi.black_pawn_3)!=0) +
                ((pi.black_pawn_4)!=0);
    }
    return val1;
}
int Engine::knight_count(const Board&b,PlayerColor color, PawnInfo& pi)
{
    int val1=0;
    if(color==WHITE)
    {
        val1=val1 + 
                (b.data.w_knight_1!=DEAD) + 
                (b.data.w_knight_2!=DEAD) + 
                ((pi.white_pawn_1 & KNIGHT)!=0) +
                ((pi.white_pawn_2 & KNIGHT)!=0) +
                ((pi.white_pawn_3 & KNIGHT)!=0) +
                ((pi.white_pawn_4 & KNIGHT)!=0);
    }
    else
    {
        val1 = val1 + 
                (b.data.b_knight_1!=DEAD) + 
                (b.data.b_knight_2!=DEAD) + 
                ((pi.black_pawn_1 & KNIGHT)!=0) +
                ((pi.black_pawn_2 & KNIGHT)!=0) +
                ((pi.black_pawn_3 & KNIGHT)!=0) +
                ((pi.black_pawn_4 & KNIGHT)!=0);
    }
    return val1;
}
int Engine::king_in_check(const Board&b, PlayerColor color) //1=color king in check, -1=opponent king in check, 0=no king in check
{
    if(b.in_check())
    {
        if(b.data.player_to_play==color)
            return 1;
        else
            return -1;
    }
    else
        return 0;
}

void Engine::feature_values(const Board& b, std::vector<int>& ans, PawnInfo& pi)
{    
    Board temp_b(b);
    int r1 = rook_count(b,my_player,pi);
    int b1 = bishop_count(b,my_player,pi);
    int p1 = pawn_count(b,my_player,pi);
    int k1 = knight_count(b,my_player,pi);
    temp_b.data.player_to_play=my_player;
    int num1 = temp_b.get_legal_moves().size();
    
    int r2 = rook_count(b,oppcolor(my_player),pi);
    int b2 = bishop_count(b,oppcolor(my_player),pi);
    int p2 = pawn_count(b,oppcolor(my_player),pi);
    int k2 = knight_count(b,oppcolor(my_player),pi);
    temp_b.data.player_to_play = oppcolor(my_player);
    int num2 = temp_b.get_legal_moves().size();

    
    ans.push_back(king_in_check(b,my_player));
    ans.push_back(b1-b2);
    ans.push_back(r1-r2);
    ans.push_back(k1-k2);
    ans.push_back(p1-p2);
    ans.push_back(b1*r1-b2*r2);
    ans.push_back(b1*k1-b2*k2);
    ans.push_back(b1*p1-b2*p2);
    ans.push_back(r1*k1-r2*k2);
    ans.push_back(r1*p1-r2*p2);
    ans.push_back(k1*p1-k2*p2);
    ans.push_back(num1-num2);
    ans.push_back( ((int)(b1==0)) - ((int)(b2==0)) );
    ans.push_back( ((int)(r1==0)) - ((int)(r2==0)) );
    ans.push_back( ((int)(k1==0)) - ((int)(k2==0)) );
}

bool are_adjacent(U8 p1, U8 p2)
{
    int x1=getx(p1);
    int y1=gety(p1);
    int x2=getx(p2);
    int y2=gety(p2);
    if(x1-x2>=2 || x1-x2<=-2)
        return false;
    if(y1-y2>=2 || y1-y2<=-2)
        return false;
    return true;
}


float Engine::pawn_structure(const Board& b, PlayerColor color, PawnInfo& pi)
{
    float ans=0;
    if(color==WHITE)
    {
        std::vector<U8> pawn_pos;
        if(pi.white_pawn_1 & PAWN)
        {
            ans+=weights.pawn_wts[b.data.board_type][0][b.data.w_pawn_1];
            pawn_pos.push_back(b.data.w_pawn_1);
        }      
        if(pi.white_pawn_2 & PAWN)
        {
            ans+=weights.pawn_wts[b.data.board_type][0][b.data.w_pawn_2];
            pawn_pos.push_back(b.data.w_pawn_2);
        }
        if(pi.white_pawn_3 & PAWN)      
        {
            ans+=weights.pawn_wts[b.data.board_type][0][b.data.w_pawn_3];
            pawn_pos.push_back(b.data.w_pawn_3);
        }
        if(pi.white_pawn_4 & PAWN)      
        {
            ans+=weights.pawn_wts[b.data.board_type][0][b.data.w_pawn_4];
            pawn_pos.push_back(b.data.w_pawn_4);
        }
        for(int x=0;x<pawn_pos.size();x++)
        {
            for(int y=x+1;y<pawn_pos.size();y++)
            {
                if(are_adjacent(pawn_pos[x],pawn_pos[y]))
                    ans+=0.5;
            }
        }
    }
    else
    {
        std::vector<U8> pawn_pos;
        if(pi.black_pawn_1 & PAWN)
        {
            ans+=weights.pawn_wts[b.data.board_type][1][b.data.b_pawn_1];
            pawn_pos.push_back(b.data.b_pawn_1);
        }      
        if(pi.black_pawn_2 & PAWN)
        {
            ans+=weights.pawn_wts[b.data.board_type][1][b.data.b_pawn_2];
            pawn_pos.push_back(b.data.b_pawn_2);
        }
        if(pi.black_pawn_3 & PAWN)      
        {
            ans+=weights.pawn_wts[b.data.board_type][1][b.data.b_pawn_3];
            pawn_pos.push_back(b.data.b_pawn_3);
        }
        if(pi.black_pawn_4 & PAWN)      
        {
            ans+=weights.pawn_wts[b.data.board_type][1][b.data.b_pawn_4];
            pawn_pos.push_back(b.data.b_pawn_4);
        }
        for(int x=0;x<pawn_pos.size();x++)
        {
            for(int y=x+1;y<pawn_pos.size();y++)
            {
                if(are_adjacent(pawn_pos[x],pawn_pos[y]))
                    ans+=3;
            }
        }

    }
    return ans;
}


float Engine::rook_structure(const Board& b, PlayerColor color, PawnInfo& pi)
{
    float ans=0;
    if(color==WHITE)
    {
        if(pi.white_pawn_1 & ROOK)
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.w_pawn_1];
        }      
        if(pi.white_pawn_2 & ROOK)
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.w_pawn_2];
        }
        if(pi.white_pawn_3 & ROOK)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.w_pawn_3];
        }
        if(pi.white_pawn_4 & ROOK)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.w_pawn_4];
        }
        if(b.data.w_rook_1!=DEAD)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.w_rook_1];
        }
        if(b.data.w_rook_2!=DEAD)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.w_rook_2];
        }
    }
    else
    {
        if(pi.black_pawn_1 & ROOK)
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.b_pawn_1];
        }      
        if(pi.black_pawn_2 & ROOK)
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.b_pawn_2];
        }
        if(pi.black_pawn_3 & ROOK)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.b_pawn_3];
        }
        if(pi.black_pawn_4 & ROOK)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.b_pawn_4];
        }
        if(b.data.b_rook_1!=DEAD)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.b_rook_1];
        }
        if(b.data.b_rook_2!=DEAD)      
        {
            ans+=weights.rook_wts[b.data.board_type][0][b.data.b_rook_2];
        }
    }
    return ans;
}


float Engine::knight_structure(const Board& b, PlayerColor color, PawnInfo& pi)
{
    float ans=0;
    if(color==WHITE)
    {
        if(pi.white_pawn_1 & KNIGHT)
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.w_pawn_1];
        }      
        if(pi.white_pawn_2 & KNIGHT)
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.w_pawn_2];
        }
        if(pi.white_pawn_3 & KNIGHT)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.w_pawn_3];
        }
        if(pi.white_pawn_4 & KNIGHT)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.w_pawn_4];
        }
        if(b.data.w_knight_1!=DEAD)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.w_knight_1];
        }
        if(b.data.w_knight_2!=DEAD)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.w_knight_2];
        }
    }
    else
    {
        if(pi.black_pawn_1 & KNIGHT)
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.b_pawn_1];
        }      
        if(pi.black_pawn_2 & KNIGHT)
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.b_pawn_2];
        }
        if(pi.black_pawn_3 & KNIGHT)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.b_pawn_3];
        }
        if(pi.black_pawn_4 & KNIGHT)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.b_pawn_4];
        }
        if(b.data.b_knight_1!=DEAD)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.b_knight_1];
        }
        if(b.data.b_knight_2!=DEAD)      
        {
            ans+=weights.knight_wts[b.data.board_type][0][b.data.b_knight_2];
        }
    }
    return ans;
}


float Engine::bishop_structure(const Board& b, PlayerColor color, PawnInfo& pi)
{
    float ans=0;
    if(color==WHITE)
    {
        if(pi.white_pawn_1 & BISHOP)
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.w_pawn_1];
        }      
        if(pi.white_pawn_2 & BISHOP)
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.w_pawn_2];
        }
        if(pi.white_pawn_3 & BISHOP)      
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.w_pawn_3];
        }
        if(pi.white_pawn_4 & BISHOP)      
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.w_pawn_4];
        }
        if(b.data.w_bishop!=DEAD)      
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.w_bishop];
        }
    }
    else
    {
        if(pi.black_pawn_1 & BISHOP)
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.b_pawn_1];
        }      
        if(pi.black_pawn_2 & BISHOP)
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.b_pawn_2];
        }
        if(pi.black_pawn_3 & BISHOP)      
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.b_pawn_3];
        }
        if(pi.black_pawn_4 & BISHOP)      
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.b_pawn_4];
        }
        if(b.data.b_bishop!=DEAD)      
        {
            ans+=weights.bishop_wts[b.data.board_type][0][b.data.b_bishop];
        }
    }
    return ans;
}


std::string Engine::state_Encoding(const Board& b, unsigned char depth, PlayerColor player) //0-white, 1-black
{
    std::string str = board_to_str(&b.data);
    str = str +","+ std::to_string(depth)+",";
    if(player==WHITE)
        str+="1";
    else
        str+="0";
    return str;

    // unsigned long long int ans = piece_Encoding(b);
    // ans=ans<<8;
    // ans = ans|depth;
    // ans=ans<<1;
    // ans = ans|player;
    // return ans;
}



float Engine::state_value(const Board& b, int depth)
{
    

    count1++;
    // std::vector<std::vector<double>> input(320,std::vector<double>(1,0));
    std::vector<double> input(320,0);
    int index=0;
    for(int x=0;x<64;x++)
    {
        if(b.data.board_0[x] == (my_player|KING) )
            input[index++]=10;
        else if(b.data.board_0[x] == (oppcolor(my_player)|KING) )
            input[index++]=-10;
        else
            input[index++]=0;
    }
    for(int x=0;x<64;x++)
    {
        if(b.data.board_0[x] == (my_player|BISHOP) )
            input[index++]=5;
        else if(b.data.board_0[x] == (oppcolor(my_player)|BISHOP) )
            input[index++]=-5;
        else    
            input[index++]=0;
    }
    // std::cerr<<std::endl;
    for(int x=0;x<64;x++)
    {
        if(b.data.board_0[x] == (my_player|ROOK) )
            input[index++]=3;
        else if(b.data.board_0[x] == (oppcolor(my_player)|ROOK) )
            input[index++]=-3;
        else
            input[index++]=0;
    }
    // std::cerr<<std::endl;
    for(int x=0;x<64;x++)
    {
        if(b.data.board_0[x] == (my_player|KNIGHT) )
            input[index++]=3;
        else if(b.data.board_0[x] == (oppcolor(my_player)|KNIGHT) )
            input[index++]=-3;
        else
            input[index++]=0;
    }
    // std::cerr<<std::endl;
    for(int x=0;x<64;x++)
    {
        if(b.data.board_0[x] == (my_player|PAWN) )
            input[index++]=1;
        else if(b.data.board_0[x] == (oppcolor(my_player)|PAWN) )
            input[index++]=-1;
        else
            input[index++]=0;
    }
    
    Matrix mat_input(input);

    
    Matrix x = Matrix::matmul(h1_wts,mat_input);
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

    double ans=x.mat[0][0];
    // PawnInfo pi(b);
    // float ans=0;
    // std::vector<int> temp;
    // feature_values(b,temp,pi);
    // for(int x=0;x<weights.value.size();x++)
    // {
    //     ans+=weights.value[x]*temp[x];
    // }
    // ans+= 10*(pawn_structure(b,my_player,pi) - pawn_structure(b,oppcolor(my_player),pi));
    // // ans+= 10*(rook_structure(b,my_player,pi) - rook_structure(b,oppcolor(my_player),pi));
    // // ans+= 10*(bishop_structure(b,my_player,pi) - bishop_structure(b,oppcolor(my_player),pi));
    // // ans+= 10*(knight_structure(b,my_player,pi) - knight_structure(b,oppcolor(my_player),pi));
    
    ans = ans + 1 - (depth/100.0);

    return ans;
}



float Engine::minval_quescence(const Board& b, float alpha, float beta, int curr_depth)
{
    total_searches++;
    auto moveset=b.get_legal_moves();
    if(moveset.empty())
    {
        if(b.in_check())
        {
            if(b.data.player_to_play==my_player)
                return FLOAT_MIN ;
            else
                return FLOAT_MAX;
        }
        else
        {
            return state_value(b,curr_depth);//utility of stalemate.
        }
    }
    Board temp_b(b);
    float ans=FLOAT_MAX;
    bool are_capture_moves=false;
    for(auto move:moveset)
    {
        U8 pos = getp1(move);
        if(b.data.board_0[pos] & oppcolor(b.data.player_to_play))
        {
            are_capture_moves=true;
            temp_b.do_move_(move);
            float temp_ans = maxval_quescence(temp_b,alpha,beta,curr_depth+1);
            temp_b.undo_last_move_without_flip_(move);
            temp_b.flip_player_();
            ans=std::min(ans,temp_ans);
            beta=std::min(beta,ans);
            if(alpha>=beta)
                break;
        }
    }
    if(!are_capture_moves)
    {
        // std::cout<<"Quescence search till "<<curr_depth<<std::endl;
        return state_value(b,curr_depth);
    }
    return ans;
}

float Engine::maxval_quescence(const Board& b, float alpha, float beta, int curr_depth)
{
    total_searches++;
    auto moveset=b.get_legal_moves();
    if(moveset.empty())
    {
        if(b.in_check())
        {
            if(b.data.player_to_play==my_player)
                return FLOAT_MIN;
            else
                return FLOAT_MAX;
        }
        else
        {
            return state_value(b,curr_depth);//utility of stalemate.
        }
    }
    Board temp_b(b);
    float ans=FLOAT_MIN;
    bool are_capture_moves=false;
    for(auto move:moveset)
    {
        U8 pos = getp1(move);
        if(b.data.board_0[pos] & oppcolor(b.data.player_to_play))
        {
            are_capture_moves=true;
            temp_b.do_move_(move);
            float temp_ans = minval_quescence(temp_b,alpha,beta,curr_depth+1);
            temp_b.undo_last_move_without_flip_(move);
            temp_b.flip_player_();
            ans=std::max(ans,temp_ans);
            alpha=std::max(alpha,ans);
            if(alpha>=beta)
                break;
        }
    }
    if(!are_capture_moves)
    {
        // std::cout<<"Quescence search till "<<curr_depth<<std::endl;
        return state_value(b,curr_depth);
    }
    return ans;
}

float Engine::quescence_search(const Board& b, int curr_depth, int alpha, int beta)
{
    float ans;
    if(b.data.player_to_play==my_player)
    {
        ans =  maxval_quescence(b,alpha,beta,curr_depth);
    }
    else
    {
        ans = minval_quescence(b,alpha,beta,curr_depth);
    }
    // std::cout<<"Quescence Search went till depth = "<<debug_max_depth<<std::endl;
    return ans;
}




float Engine::maxval(const Board& b, int curr_depth,int max_depth, float alpha, float beta)
{
    total_searches++;
    std::string state_str = state_Encoding(b,curr_depth,b.data.player_to_play);
    auto itr=transTable[call_count].find(state_str);
    if(itr!=transTable[call_count].end())
    {
        found_searches++;
        return itr->second;
    }

    if(curr_depth==max_depth)
    {
        float val=0;
        if(do_quescence)
        {
            val = quescence_search(b,curr_depth,alpha,beta);
        }
        else
        {
            val=state_value(b,curr_depth);
        }
        transTable[call_count].insert({state_str,val});
        return val;
    }

    auto moveset=b.get_legal_moves();
    Board temp_b(b);
    float ans=FLOAT_MIN;
    U16 best_move;

    std::vector<std::pair<float,U16>> moves1;
    std::vector<U16> moves2;
    for(auto move:moveset)
    {
        if(curr_depth+2<=max_depth)
        {
            temp_b.do_move_(move);
            std::string temp_str =state_Encoding(temp_b,curr_depth+2,temp_b.data.player_to_play);
            auto itr = transTable[1-call_count].find(temp_str);
            temp_b.undo_last_move_without_flip_(move);
            temp_b.flip_player_();
            if(itr!=transTable[1-call_count].end())
            {
                old_map_searches++;
                moves1.push_back(std::pair<float,U16>(itr->second,move));
            }
            else
                moves2.push_back(move);
        }
        else
            moves2.push_back(move);
    }
    sort(moves1.begin(),moves1.end());

    auto my_do_move = [&](U16 move){
        temp_b.do_move_(move);
        float temp_ans=minval(temp_b,curr_depth+1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();

        if(temp_ans>ans)
        {
            ans=temp_ans;
            best_move=move;
        }
        alpha=std::max(alpha,ans);
        
    };


    for(int x=moves1.size()-1;x>=0;x--)
    {
        my_do_move(moves1[x].second);
        if(alpha>=beta)
            break;
    }
    for(auto move:moves2)
    {
        my_do_move(move);
        if(alpha>=beta)
            break;
    }

    transTable[call_count].insert({state_str,ans});
    return ans;
}

float Engine::minval(const Board& b, int curr_depth,int max_depth, float alpha, float beta)
{
    total_searches++;
    std::string state_str = state_Encoding(b,curr_depth,b.data.player_to_play);
    auto itr=transTable[call_count].find(state_str);
    if(itr!=transTable[call_count].end())
    {
        found_searches++;
        return itr->second;
    }


    if(curr_depth==max_depth)
    {
        float val=0;
        if(do_quescence)
        {
            val = quescence_search(b,curr_depth,alpha,beta);
        }
        else
        {
            val=state_value(b,curr_depth);
        }
        transTable[call_count].insert({state_str,val});
        return val;
    }

    
    auto moveset=b.get_legal_moves();
    Board temp_b(b);
    float ans=FLOAT_MAX;
    U16 best_move;

    std::vector<std::pair<float,U16>> moves1;
    std::vector<U16> moves2;
    for(auto move:moveset)
    {
        if(curr_depth+3<=max_depth)
        {
            temp_b.do_move_(move);
            std::string temp_str =state_Encoding(temp_b,curr_depth+3,temp_b.data.player_to_play);
            auto itr = transTable[1-call_count].find(temp_str);
            temp_b.undo_last_move_without_flip_(move);
            temp_b.flip_player_();
            if(itr!=transTable[1-call_count].end())
            {
                old_map_searches++;
                moves1.push_back(std::pair<float,U16>(itr->second,move));
            }
            else
                moves2.push_back(move);
        }
        else
            moves2.push_back(move);
    }
    sort(moves1.begin(),moves1.end());

    auto my_do_move = [&](U16 move){
        temp_b.do_move_(move);
        float temp_ans=maxval(temp_b,curr_depth+1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();
        if(temp_ans<ans)
        {
            ans=temp_ans;
            best_move=move;
        }
        beta=std::min(beta,ans);
    };


    for(int x=0;x<moves1.size();x++)
    {
        my_do_move(moves1[x].second);
        if(alpha>=beta)
            break;
    }
    for(auto move:moves2)
    {
        my_do_move(move);
        if(alpha>=beta)
            break;
    }


    transTable[call_count].insert({state_str,ans});
    return ans;
}

std::pair<U16,float>  Engine::minimax(const Board& b, int max_depth)
{
    Board temp_b(b);
    auto moveset=b.get_legal_moves();
    U16 best_move=0;
    float val,best_val;
    
    float alpha=FLOAT_MIN;
    float beta=FLOAT_MAX;
    val=FLOAT_MIN;
    best_val=FLOAT_MIN;

    std::vector<std::pair<float,U16>> moves1;
    std::vector<U16> moves2;
    for(auto move:moveset)
    {
        if(3<=max_depth)
        {
            temp_b.do_move_(move);
            std::string temp_str =state_Encoding(temp_b,3,temp_b.data.player_to_play);
            auto itr = transTable[1-call_count].find(temp_str);
            temp_b.undo_last_move_without_flip_(move);
            temp_b.flip_player_();
            if(itr!=transTable[1-call_count].end())
            {
                old_map_searches++;
                moves1.push_back(std::pair<float,U16>(itr->second,move));
            }
            else
                moves2.push_back(move);
        }
        else
            moves2.push_back(move);
    }


    auto my_do_func = [&](U16 move){
        temp_b.do_move_(move);
        val = minval(temp_b,1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();
        if(val>=best_val)
        {
            best_val=val;
            best_move=move;
        }
        alpha=std::max(alpha,best_val);        
    };

    for(int x=moves1.size()-1;x>=0;x--)
        my_do_func(moves1[x].second);
    
    for(auto move:moves2)
        my_do_func(move);


    return std::pair<U16,float>(best_move,best_val);
}


void Engine::find_best_move(const Board& b) {
    my_player=b.data.player_to_play;
    const int range_from  = 0;
    const int range_to    = 10000;
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(range_from, range_to);
    if(my_player==WHITE)
    {
            call_count=1-call_count;
            count1=0;
            total_searches=0;
            found_searches=0;
            old_map_searches=0;
            transTable[call_count].clear();
            
            do_quescence=true;
            auto time1 = std::chrono::high_resolution_clock::now();

            
            auto p = minimax(b,3);
            this->best_move=p.first;
            
            

            

            auto time2 = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);
            std::cout<<"---------------------------------------------------------------------------------------------"<<std::endl;
            std::cout<<"Total searches = "<<total_searches<<std::endl;
            std::cout<<"Found searches = "<<found_searches<<std::endl;
            std::cout<<"Found searches in old map = "<<old_map_searches<<std::endl;
            std::cout<<"Time taken = "<<duration.count()<<" ms"<<std::endl;
            std::cout<<"Current Map size = "<<transTable[call_count].size()<<std::endl;
            std::cout<<"Previous Map size = "<<transTable[1-call_count].size()<<std::endl;
            std::cout<<"Evaluation func call Count = "<<count1<<std::endl;
            std::cout<<"---------------------------------------------------------------------------------------------"<<std::endl;

            // std::cerr<<board_to_str(&b.data)<<std::endl;
            for(int x=0;x<64;x++)
            {
                if(b.data.board_0[x] == (my_player|KING) )
                    std::cerr<<10<<" ";
                else if(b.data.board_0[x] == (oppcolor(my_player)|KING) )
                    std::cerr<<-10<<" ";
                else
                    std::cerr<<0<<" ";
            }
            // std::cerr<<std::endl;
            for(int x=0;x<64;x++)
            {
                if(b.data.board_0[x] == (my_player|BISHOP) )
                    std::cerr<<5<<" ";
                else if(b.data.board_0[x] == (oppcolor(my_player)|BISHOP) )
                    std::cerr<<-5<<" ";
                else    
                    std::cerr<<0<<" ";
            }
            // std::cerr<<std::endl;
            for(int x=0;x<64;x++)
            {
                if(b.data.board_0[x] == (my_player|ROOK) )
                    std::cerr<<3<<" ";
                else if(b.data.board_0[x] == (oppcolor(my_player)|ROOK) )
                    std::cerr<<-3<<" ";
                else
                    std::cerr<<0<<" ";
            }
            // std::cerr<<std::endl;
            for(int x=0;x<64;x++)
            {
                if(b.data.board_0[x] == (my_player|KNIGHT) )
                    std::cerr<<3<<" ";
                else if(b.data.board_0[x] == (oppcolor(my_player)|KNIGHT) )
                    std::cerr<<-3<<" ";
                else
                    std::cerr<<0<<" ";
            }
            // std::cerr<<std::endl;
            for(int x=0;x<64;x++)
            {
                if(b.data.board_0[x] == (my_player|PAWN) )
                    std::cerr<<1<<" ";
                else if(b.data.board_0[x] == (oppcolor(my_player)|PAWN) )
                    std::cerr<<-1<<" ";
                else
                    std::cerr<<0<<" ";
            }
            std::cerr<<std::endl;
            std::cerr<<p.second<<std::endl;
            
            // float temp_val = rand()%RAND_MAX;
            // if(temp_val>0.8)
            //     return;
            auto moveset = b.get_legal_moves();
            int set_size = moveset.size();
            int index = rand()%set_size;
            for(auto temp_p:moveset)
            {
                if(index<=0)
                {
                    this->best_move=temp_p;
                    break;
                }
                index--;
            }

            if(distr(generator)>(7*range_to/10))
            {
                std::cout<<"Ideal Move"<<std::endl;
                this->best_move=p.first;
            }
            else
                std::cout<<"random Move"<<std::endl;
    }
    else
    {
            auto moveset = b.get_legal_moves();
            int set_size = moveset.size();
            int index = rand()%set_size;
            for(auto p:moveset)
            {
                if(index<=0)
                {
                    this->best_move=p;
                    break;
                }
                index--;
            }
    }
}
