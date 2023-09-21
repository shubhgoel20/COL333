#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <climits>
#include <map>
#include <assert.h>

#include "board.hpp"
#include "engine.hpp"

int INTMAX=1e9;

bool time_check(std::chrono::high_resolution_clock::time_point start_time){
    auto cur_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start_time);
    if(duration.count()>1900) return 0;
    return 1;
}

int evaluation_func(Board* board_copy,PlayerColor my_col)
{
    std::cout<<"Hello world"<<std::endl;
    static int count=-1;
    count++;
    int w_king=7,w_bishop=4,w_rook=6,w_pawn=2,w_moves=0;
    // int bishop_threat=3,rook_threat=2,pawn_threat=1;
    PlayerColor curr_col=board_copy->data.player_to_play;
    std::unordered_set<U16> moveset[2];     //0-WHITE, 1-BLACK
    
    board_copy->data.player_to_play=WHITE;
    moveset[0]=board_copy->get_legal_moves();
    board_copy->data.player_to_play=BLACK;
    moveset[1]=board_copy->get_legal_moves();

    board_copy->data.player_to_play=curr_col;
    
    BoardData *d = &(board_copy->data);

    if( ( (curr_col==WHITE && moveset[0].size()==0) || (curr_col==BLACK && moveset[1].size()==0) )  && board_copy->in_check())  //checkmate
    {
        if(curr_col==my_col)
            return -1*INT_MAX;
        else
            return INT_MAX;

    }
    else if( (curr_col==WHITE && moveset[0].size()==0) || (curr_col==BLACK && moveset[1].size()==0) )  //stalemate
        return 0;
    
    U8 black_pawn_1= (d->b_pawn_bs!=DEAD) ? d->board_0[d->b_pawn_bs] : 0;
    U8 black_pawn_2= (d->b_pawn_ws!=DEAD) ? d->board_0[d->b_pawn_ws] : 0;
    U8 white_pawn_1= (d->w_pawn_bs!=DEAD) ? d->board_0[d->w_pawn_bs] : 0;
    U8 white_pawn_2= (d->w_pawn_ws!=DEAD) ? d->board_0[d->w_pawn_ws] : 0;
    
    int num_rooks[2]={0};
    int num_bishops[2]={0};
    int num_pawns[2]={0};
    num_rooks[0]=(d->w_rook_bs!=DEAD) + (d->w_rook_ws!=DEAD) + ((white_pawn_1 & ROOK)!=0) + ((white_pawn_2 & ROOK)!=0);
    num_rooks[1]=(d->b_rook_bs!=DEAD) + (d->b_rook_ws!=DEAD) + ((black_pawn_1 & ROOK)!=0) + ((black_pawn_2 & ROOK)!=0);
    num_bishops[0]=(d->w_bishop!=DEAD) + ((white_pawn_1 & BISHOP)!=0) + ((white_pawn_2 & BISHOP)!=0);
    num_bishops[1]=(d->b_bishop!=DEAD) + ((black_pawn_1 & BISHOP)!=0) + ((black_pawn_2 & BISHOP)!=0);
    num_pawns[0]=((white_pawn_1 & PAWN)!=0) + ((white_pawn_2 & PAWN)!=0);
    num_pawns[1]=((black_pawn_1 & PAWN)!=0) + ((black_pawn_2 & PAWN)!=0);
    
    int evaluation=0;
    evaluation+=(num_rooks[0]-num_rooks[1])*w_rook;
    evaluation+=(num_bishops[0]-num_bishops[1])*w_bishop;
    evaluation+=(num_pawns[0]-num_pawns[1])*w_pawn;
    evaluation+=(moveset[0].size()-moveset[1].size())*w_moves;
    if(my_col==BLACK)
    {
        evaluation=-evaluation;
    }

    if(my_col==curr_col)
        evaluation -= w_king*(board_copy->in_check());
    else
        evaluation += w_king*(board_copy->in_check());

    //now can also add weight of pieces in threat
    if(count>=3)
    {
        std::cout<<"YAY Parinting"<<std::endl;
        std::cout<<"Pawns = "<<num_pawns[0]<<","<<num_pawns[1]<<std::endl;
        std::cout<<"Rooks = "<<num_rooks[0]<<","<<num_rooks[1]<<std::endl;
        std::cout<<"Bishops = "<<num_bishops[0]<<","<<num_bishops[1]<<std::endl;
        std::cout<<"Moves = "<<moveset[0].size()<<","<<moveset[1].size()<<std::endl;
    }
    return evaluation;      

}

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col);
int maxval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col)
{
    if(curr_depth==0)
    {
        return evaluation_func(b,my_col);
    }
    auto moveset=b->get_legal_moves();

    for(auto move:moveset)
    {
        Board * temp_board=b->copy();
        temp_board->do_move(move);
        alpha=std::max(alpha,minval(temp_board,curr_depth-1,alpha,beta,start_time,my_col));
        delete temp_board;
        if(alpha>=beta)
            return alpha;
    }
    return alpha;
}

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col)
{
    if(curr_depth==0)
    {
        return evaluation_func(b,my_col);
    }
    auto moveset=b->get_legal_moves();
    for(auto move:moveset)
    {
        Board * temp_board=b->copy();
        temp_board->do_move(move);
        beta=std::min(beta,maxval(temp_board,curr_depth-1,alpha,beta,start_time,my_col));
        delete temp_board;
        if(alpha>=beta)
            return beta;
    }
    return beta;
}

std::vector<std::pair<int,U16>> minimax(Board *b, int curr_depth,std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col)
{
    std::vector<std::pair<int,U16>> ans;
    auto moveset=b->get_legal_moves();
    for(auto move : moveset)
    {
        Board * temp_board=b->copy();
        temp_board->do_move(move);
        int val = minval(temp_board,curr_depth-1,-1*INTMAX,INTMAX,start_time,my_col);
        delete temp_board;
        ans.push_back(std::pair<int,U16>(val,move));
    }
    sort(ans.begin(),ans.end());
    return ans;
}

void Engine::find_best_move(const Board& b) 
{
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) 
    {
        this->best_move = 0;
    }
    else
    {
        auto start_time=std::chrono::high_resolution_clock::now();
        Board* temp_b=b.copy();
        auto ans=minimax(temp_b,2,start_time,b.data.player_to_play);
        delete temp_b;
        for(auto p:ans)
        {
            std::cout<<p.first<<" "<<p.second<<std::endl;
        }
        this->best_move=ans[ans.size()-1].second;
    }

}