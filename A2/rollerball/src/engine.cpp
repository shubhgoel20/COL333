#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>

#include "board.hpp"
#include "engine.hpp"

int evaluation_func(Board *b)
{
    int w_king=8,w_bishop=5,w_rook=3,w_pawn=1;
    //evaluates the board with respect to the player who did last move
    PlayerColor my_col = (b->data.player_to_play==WHITE) ? BLACK : WHITE;
    int king_under_check=b->in_check();
    //how to handle pawn promotions
    int bishop_diff = (b->data.w_bishop!=DEAD) - (b->data.b_bishop!=DEAD);
    int rook_diff = (b->data.w_rook_ws!=DEAD) + (b->data.w_rook_bs!=DEAD) - (b->data.b_rook_ws!=DEAD) - (b->data.b_rook_bs!=DEAD);
    int pawn_diff = (b->data.w_pawn_ws!=DEAD) + (b->data.w_pawn_bs!=DEAD) - (b->data.b_pawn_ws!=DEAD) - (b->data.b_pawn_bs!=DEAD);
    if(my_col==BLACK)
    {
        bishop_diff=-bishop_diff;
        rook_diff=-rook_diff;
        pawn_diff=-pawn_diff;
    }
    int evaluation=king_under_check*w_king + bishop_diff * w_bishop + rook_diff * w_rook + pawn_diff * w_pawn;
    std::cout<<evaluation<<std::endl;   //for debug purpose
    return evaluation;
    
}

void Engine::find_best_move(const Board& b) {

    
    
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
    }
    else {
        Board* b_copy;
        // std::vector<U16> moves(moveset.begin(),moveset.end());
    
        std::unordered_set<U16>::iterator itr=moveset.begin();
        U16 curr_best_move=*itr;

        b_copy=b.copy();
        b_copy->do_move(curr_best_move);
        int max_eval=evaluation_func(b_copy);
        delete b_copy;
        b_copy=nullptr;

        itr++;
        
        auto start=std::chrono::high_resolution_clock::now();
        auto cur_time=std::chrono::high_resolution_clock::now();
        auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start);
        while(1)
        {
            cur_time=std::chrono::high_resolution_clock::now();
            duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start);            
            if(duration.count()>1900) break;
            //process data here
            if(itr!=moveset.end())
            {
                b_copy=b.copy();
                b_copy->do_move(*itr);
                int temp_eval=evaluation_func(b_copy);
                delete b_copy;
                b_copy=nullptr;
                if(temp_eval > max_eval)
                {
                    curr_best_move=*itr;
                    max_eval=temp_eval;
                }
                itr++;
            }
            else
                break;
                //here instead of breaking, you can reset iterator and maintain a variable depth,
                //which could be increased to search till next depth it time remains.
        }
        this->best_move = curr_best_move;        

        // std::vector<U16> moves;
        // std::cout << all_boards_to_str(b) << std::endl;
        // for (auto m : moveset) {
        //     std::cout << move_to_str(m) << " ";
        // }
        // std::cout << std::endl;
        // std::sample(
        //     moveset.begin(),
        //     moveset.end(),
        //     std::back_inserter(moves),
        //     1,
        //     std::mt19937{std::random_device{}()}
        // );
        // this->best_move = moves[0];
    }

}
