#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include "board.hpp"
#include "engine.hpp"

#define INT_MAX 1e9
//this performs the specified move, and evaluates the board for the player who moved last
std::pair<int,int> evaluate_func(Board *board_copy)
{
    // Board* board_copy=b_orig->copy();
    // board_copy->do_move(move);
    int w_king=8,w_bishop=5,w_rook=3,w_pawn=1;
    int white_in_check,black_in_check;
    if(board_copy->in_check())
    {
        if(board_copy->data.player_to_play==WHITE)  white_in_check=1;
        else    black_in_check=1;
    }
    //how to handle pawn promotions
    int bishop_diff = (board_copy->data.w_bishop!=DEAD) - (board_copy->data.b_bishop!=DEAD);
    int rook_diff = (board_copy->data.w_rook_ws!=DEAD) + (board_copy->data.w_rook_bs!=DEAD) - (board_copy->data.b_rook_ws!=DEAD) - (board_copy->data.b_rook_bs!=DEAD);
    int pawn_diff = (board_copy->data.w_pawn_ws!=DEAD) + (board_copy->data.w_pawn_bs!=DEAD) - (board_copy->data.b_pawn_ws!=DEAD) - (board_copy->data.b_pawn_bs!=DEAD);
    // if(my_col==BLACK)
    // {
    //     bishop_diff=-bishop_diff;
    //     rook_diff=-rook_diff;
    //     pawn_diff=-pawn_diff;
    // }
    // int evaluation=king_under_check*w_king + bishop_diff * w_bishop + rook_diff * w_rook + pawn_diff * w_pawn;
    std::pair<int,int> evaluation;
    int symmetric_eval = bishop_diff * w_bishop + rook_diff * w_rook + pawn_diff * w_pawn;
    evaluation.first  =  symmetric_eval - white_in_check*w_king;
    evaluation.second = -symmetric_eval - black_in_check*w_king;

    // delete board_copy;
    // std::cout<<evaluation.first<<" "<<evaluation.second<<std::endl;   //for debug purpose
    return evaluation;
}

// void un_do_move(Board* b,U16 move) {

//     U8 p0 = getp0(move);
//     U8 p1 = getp1(move);
//     U8 promo = getpromo(move);

//     U8 piecetype = b->data.board_0[p1];
//     b->data.last_killed_piece = 0;
//     b->data.last_killed_piece_idx = -1;

//     // scan and get piece from coord
//     U8 *pieces = (U8*)this;
//     for (int i=0; i<12; i++) {
//         if (pieces[i] == p1) {
//             pieces[i] = DEAD;
//             this->data.last_killed_piece = this->data.board_0[p1];
//             this->data.last_killed_piece_idx = i;
//         }
//         if (pieces[i] == p0) {
//             pieces[i] = p1;
//         }
//     }

//     if (promo == PAWN_ROOK) {
//         piecetype = (piecetype & (WHITE | BLACK)) | ROOK;
//     }
//     else if (promo == PAWN_BISHOP) {
//         piecetype = (piecetype & (WHITE | BLACK)) | BISHOP;
//     }

//     this->data.board_0[p1]           = piecetype;
//     this->data.board_90[cw_90[p1]]   = piecetype;
//     this->data.board_180[cw_180[p1]] = piecetype;
//     this->data.board_270[acw_90[p1]] = piecetype;

//     this->data.board_0[p0]           = 0;
//     this->data.board_90[cw_90[p0]]   = 0;
//     this->data.board_180[cw_180[p0]] = 0;
//     this->data.board_270[acw_90[p0]] = 0;

//     // std::cout << "Did last move\n";
//     // std::cout << all_boards_to_str(*this);
// }


std::pair<int,int> evaluate_move(Board *b, int depth,int end_depth)    //I am going to move
{
    std::cout<<"Depth="<<depth<<std::endl;
    auto moveset=b->get_legal_moves();
    PlayerColor my_col = b->data.player_to_play;
    if(moveset.size()==0 && b->in_check())
    {
        if(my_col==WHITE)
            return std::pair<int,int>(-1*INT_MAX,INT_MAX);
        else
            return std::pair<int,int>(INT_MAX,-1*INT_MAX);
    }
    else if(moveset.size()==0)  //stalemate
        return std::pair<int,int>(0,0);
        
    int flag=-1;
    std::pair<int,int> ans,temp_ans;
    U16 best_move;
    for(auto curr_move : moveset)
    {
        Board* board_copy=b->copy();
        board_copy->do_move(curr_move);
        if(depth==end_depth)
        {
            temp_ans=evaluate_func(board_copy);
        }
        else
        {
            temp_ans=evaluate_move(board_copy,depth+1,end_depth);
        }

        if(my_col==WHITE && (flag==-1 || temp_ans.first>ans.first))
        {
            ans=temp_ans;
            flag=1;
            best_move=curr_move;
            // std::cout<<"Best move set"<<std::endl;
        }
        else if(my_col==BLACK && (flag==-1 || temp_ans.second > ans.second))
        {
            ans=temp_ans;
            flag=1;
            best_move=curr_move;
            // std::cout<<"Best move set"<<std::endl;
        }
        delete board_copy;
    }
    if(depth==0)
    {
        std::pair<int,int> p;
        p.first=best_move;
        // p.first=p.first<<16;
        // p.first+=best_move;
        p.second=0;
        return p;
    }
    else
        return ans;
}

void Engine::find_best_move(const Board& b) {
    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
    }
    else {        
        // auto start=std::chrono::high_resolution_clock::now();
        // auto cur_time=std::chrono::high_resolution_clock::now();
        // auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start);
        // cur_time=std::chrono::high_resolution_clock::now();
        // duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start);            
        // if(duration.count()>1900) break;
        
        Board* b_copy=b.copy();
        this->best_move = (U16)(evaluate_move(b_copy,0,2).first); 
        std::cout<<"Reached"<<std::endl;
        delete b_copy;
    }

}
