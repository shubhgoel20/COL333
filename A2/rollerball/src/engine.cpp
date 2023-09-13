#include <algorithm>
#include <random>
#include <iostream>

#include "board.hpp"
#include "engine.hpp"


// #include <chrono>
#include <unordered_map>

#define INT_MAX 1e9

std::pair<int,int> evaluate_func(Board *board_copy)
{
    auto moveset=board_copy->get_legal_moves();
    PlayerColor my_col = board_copy->data.player_to_play;
    if(moveset.size()==0 && board_copy->in_check())
    {
        if(my_col==WHITE)
            return std::pair<int,int>(-1*INT_MAX,INT_MAX);
        else
            return std::pair<int,int>(INT_MAX,-1*INT_MAX);
    }
    else if(moveset.size()==0)  //stalemate
        return std::pair<int,int>(0,0);


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

    std::pair<int,int> evaluation;
    int symmetric_eval = bishop_diff * w_bishop + rook_diff * w_rook + pawn_diff * w_pawn;
    evaluation.first  =  symmetric_eval - white_in_check*w_king;
    evaluation.second = -symmetric_eval - black_in_check*w_king;

    return evaluation;
}

//assuming 0 is an invalid move
std::pair<int,int> evaluate_move(Board *b, int depth,int end_depth,U16 invalid_move=0)    //I am going to move
{
    // std::cout<<"Depth="<<depth<<std::endl;
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
    U16 best_move=*(moveset.begin());
    Board* board_copy=b->copy();
    board_copy->do_move(best_move);
    ans=evaluate_func(board_copy);
    delete board_copy;

    for(auto curr_move : moveset)
    {
        if(curr_move==invalid_move) continue;

        board_copy=b->copy();
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

    // pick a random move
    static std::unordered_map<std::string,signed char> board_string;
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

        // std::string board_str_rep=board_to_str(reinterpret_cast<const U8*>(&b));
        // auto temp = board_string.find(board_str_rep);
        // if(temp==board_string.end())
        // {   
        //     std::cout<<"New entry"<<std::endl;
        //     board_string[board_str_rep]=1;
        // }
        // else
        // {
        //     std::cout<<"old entry"<<std::endl;
        //     board_string[board_str_rep]++;
        // }


        Board* b_copy=b.copy();
        this->best_move = (U16)(evaluate_move(b_copy,0,0).first); 
        b_copy->do_move(this->best_move);
        std::string board_str_rep = board_to_str(reinterpret_cast<U8*>(b_copy));
        delete b_copy;
        // std::cout<<board_str_rep<<std::endl;
        auto temp = board_string.find(board_str_rep);
        if(temp==board_string.end())
        {   
            std::cout<<"New entry"<<std::endl;
            board_string[board_str_rep]=1;
        }
        else if(temp->second==1)
        {
            std::cout<<"old entry"<<std::endl;
            board_string[board_str_rep]++;
        }
        else
        {
            std::cout<<"Threefold repitition detected"<<std::endl;
            b_copy=b.copy();
            this->best_move=evaluate_move(b_copy,0,0,this->best_move).first;
            //instead of directly assigning 2nd best move, we can say if the 2nd best move gives us a loss, then draw the game
            delete b_copy;
        }


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
