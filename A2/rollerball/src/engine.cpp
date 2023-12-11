#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <climits>
#include <assert.h>
#include "board.hpp"
#include "engine.hpp"

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col);
int maxval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col);

bool time_check(std::chrono::high_resolution_clock::time_point& start_time){
    auto cur_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start_time);
    if(duration.count()>1850)
    {
        return 0;
    }
    return 1;
}

inline int weight(U8 piece)
{
    int wt_multiplier=1000;
    if(piece & KING)    return 1*wt_multiplier;
    else if(piece & BISHOP) return 8*wt_multiplier;
    else if(piece & ROOK)   return 6*wt_multiplier;
    else if(piece & PAWN)   return 2*wt_multiplier;
    else return 0;
}

inline int threat_weight(U8 piece)
{
    int wt_multiplier=5;
    if(piece & KING)    return 2*wt_multiplier;
    else if(piece & BISHOP) return 4*wt_multiplier;
    else if(piece & ROOK)   return 3*wt_multiplier;
    else if(piece & PAWN)   return 2*wt_multiplier;
    else return 0;
}


int evaluation_func(Board* board_copy,PlayerColor my_col,std::chrono::high_resolution_clock::time_point& start_time)
{
    // if(board_copy->in_check() && time_check(start_time))
    // {
    //     if(board_copy->data.player_to_play==my_col)
    //         return minval(board_copy,2,INT_MIN,INT_MAX,start_time,my_col);
    //     else
    //         return maxval(board_copy,2,INT_MIN,INT_MAX,start_time,my_col);
    // }
    int w_moves=1;    

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
            return INT_MIN;
        else
            return INT_MAX;

    }
    
    U8 black_pawn_1= (d->b_pawn_bs!=DEAD) ? d->board_0[d->b_pawn_bs] : 0;
    U8 black_pawn_2= (d->b_pawn_ws!=DEAD) ? d->board_0[d->b_pawn_ws] : 0;
    U8 white_pawn_1= (d->w_pawn_bs!=DEAD) ? d->board_0[d->w_pawn_bs] : 0;
    U8 white_pawn_2= (d->w_pawn_ws!=DEAD) ? d->board_0[d->w_pawn_ws] : 0;
    
    int num_rooks[2]={0};
    int num_bishops[2]={0};
    int num_pawns[2]={0};

    //promotion to rook is favoured over promotion to pawn
    num_rooks[0]=(d->w_rook_bs!=DEAD) + (d->w_rook_ws!=DEAD) + ((white_pawn_2 & BISHOP)!=0);
    num_rooks[1]=(d->b_rook_bs!=DEAD) + (d->b_rook_ws!=DEAD) + ((black_pawn_2 & BISHOP)!=0);

    num_bishops[0]=(d->w_bishop!=DEAD) + ((white_pawn_1 & BISHOP)!=0) + ((white_pawn_1 & ROOK)!=0) + ((white_pawn_2 & ROOK)!=0);
    num_bishops[1]=(d->b_bishop!=DEAD) + ((black_pawn_1 & BISHOP)!=0) + ((black_pawn_1 & ROOK)!=0) + ((black_pawn_2 & ROOK)!=0);

    num_pawns[0]=((white_pawn_1 & PAWN)!=0) + ((white_pawn_2 & PAWN)!=0);
    num_pawns[1]=((black_pawn_1 & PAWN)!=0) + ((black_pawn_2 & PAWN)!=0);

    int evaluation=0;
    
    evaluation+=(num_rooks[0]-num_rooks[1])*weight(ROOK);
    evaluation+=(num_bishops[0]-num_bishops[1])*weight(BISHOP);
    evaluation+=(num_pawns[0]-num_pawns[1])*weight(PAWN);
    evaluation+=(moveset[0].size()-moveset[1].size())*w_moves;

    if(my_col==BLACK)
    {
        evaluation=-evaluation;
    }

    if(my_col==curr_col)
        evaluation -= weight(KING)*(board_copy->in_check());
    else
        evaluation += weight(KING)*(board_copy->in_check());

    return (evaluation);      
}

int maxval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col)
{
    if(curr_depth<=0  || time_check(start_time)==0)
    {
        int temp=evaluation_func(b,my_col,start_time);
        return temp;
    }   

    auto moveset=b->get_legal_moves();
    int ans=INT_MIN;
    for(auto move:moveset)
    {
        if(time_check(start_time)==0)   break;
        Board* temp_b=b->copy();
        temp_b->do_move(move);
        int temp_ans=minval(temp_b,curr_depth-1,alpha,beta,start_time,my_col);
        ans=std::max(ans,temp_ans);
        delete temp_b;
        alpha=std::max(alpha,ans);
        if(alpha>=beta)
            break;
    }
    return ans;
}

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col)
{
    if(curr_depth<=0 || time_check(start_time)==0)
    {
        int temp=evaluation_func(b,my_col,start_time);
        return temp;
    }
    auto moveset=b->get_legal_moves();
    int ans=INT_MAX;
    for(auto move:moveset)
    {
        if(time_check(start_time)==0)   break;
        Board* temp_b=b->copy();
        temp_b->do_move(move);
        int temp_ans=maxval(temp_b,curr_depth-1,alpha,beta,start_time,my_col);
        ans=std::min(ans,temp_ans);
        delete temp_b;
        beta=std::min(beta,ans);
        if(alpha>=beta)
            break;
    }
    return ans;
}

std::vector<std::pair<int,U16>> minimax(Board *b, int curr_depth,std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col)
{
    std::vector<std::pair<int,U16>> ans;
    auto moveset=b->get_legal_moves();
    for(auto move : moveset)
    {
        Board* temp_b=b->copy();
        temp_b->do_move(move);
        int val = minval(temp_b,curr_depth-1,INT_MIN,INT_MAX,start_time,my_col);
        delete temp_b;
        ans.push_back(std::pair<int,U16>(val,move));
    }
    sort(ans.begin(),ans.end());
    return ans;
}


U16 give_preferred_move(const Board& b)
{
    static int counter=-1;
    auto moveset=b.get_legal_moves();
    bool opening=1;
    if(b.data.player_to_play==BLACK)
    {
        if(b.data.b_bishop==DEAD || b.data.b_rook_bs==DEAD || b.data.b_rook_ws==DEAD || b.data.b_pawn_bs==DEAD || b.data.b_pawn_ws==DEAD || b.in_check())
            opening=0;
    }
    else
    {
        if(b.data.w_bishop==DEAD || b.data.w_rook_bs==DEAD || b.data.w_rook_ws==DEAD || b.data.w_pawn_bs==DEAD || b.data.w_pawn_ws==DEAD || b.in_check())            
            opening=0;
    }
    counter++;
    if(counter<=5 && opening)
    {
        U16 preferred_move=0;
        if(b.data.player_to_play==WHITE)
        {
            switch(counter)
            {
                case 0: preferred_move=move(pos(4,0),pos(5,0)); break;
                case 1: preferred_move=move(pos(4,1),pos(4,0)); break;
                case 2: preferred_move=move(pos(2,1),pos(1,1)); break;
                case 3: preferred_move=move(pos(1,1),pos(0,2)); break;
                case 4: preferred_move=move(pos(2,0),pos(1,1)); break;
                case 5: 
                if(b.data.board_0[pos(0,4)]==EMPTY)
                    preferred_move=move(pos(3,0),pos(0,3));
                else
                    preferred_move=move(pos(3,0),pos(1,2));
                break;
            }
        }
        else
        {
            switch(counter)
            {
                case 0: preferred_move=move(pos(2,6),pos(1,6)); break;
                case 1: preferred_move=move(pos(2,5),pos(2,6)); break;
                case 2: preferred_move=move(pos(4,5),pos(5,5)); break;
                case 3: preferred_move=move(pos(5,5),pos(6,4)); break;
                case 4: preferred_move=move(pos(4,6),pos(5,5)); break;
                case 5: 
                if(b.data.board_0[pos(6,2)]==EMPTY)
                    preferred_move=move(pos(3,6),pos(6,3));
                else
                    preferred_move=move(pos(3,6),pos(5,4));
                break;
            }
        }
        
        if(preferred_move!=0)
        {
            for(auto move:moveset)
            {
                if(preferred_move==move)
                {
                    std::cout<<"----------------Doing Preferred Move--------------"<<std::endl;
                    return preferred_move;
                }
            }
        }
        
    }
    return 0;

}

int num_powerful_piece(Board* b)
{
    int num=(b->data.b_bishop!=DEAD) + (b->data.b_rook_bs!=DEAD) +(b->data.b_rook_ws!=DEAD);
    num+=(b->data.w_bishop!=DEAD) + (b->data.w_rook_bs!=DEAD) +(b->data.w_rook_ws!=DEAD);
    num+= (b->data.b_pawn_ws!=DEAD && !(b->data.board_0[b->data.b_pawn_ws] & PAWN));
    num+= (b->data.b_pawn_bs!=DEAD && !(b->data.board_0[b->data.b_pawn_bs] & PAWN));
    num+= (b->data.w_pawn_ws!=DEAD && !(b->data.board_0[b->data.w_pawn_ws] & PAWN));
    num+= (b->data.w_pawn_bs!=DEAD && !(b->data.board_0[b->data.w_pawn_bs] & PAWN));
    return num;
}

void Engine::find_best_move(const Board& b) 
{
    auto start_time=std::chrono::high_resolution_clock::now();
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) 
    {
        this->best_move = 0;
    }
    else
    {
        this->best_move=*(moveset.begin());
        U16 pref_move=give_preferred_move(b);
        if(pref_move!=0)
        {
            this->best_move=pref_move;
            return;
        }
        Board* temp_b=b.copy();
        int max_depth=2;
        if(num_powerful_piece(temp_b)<=2)  max_depth=4;
        auto ans=minimax(temp_b,max_depth,start_time,b.data.player_to_play);
        delete temp_b;
        this->best_move=ans[ans.size()-1].second;
        
        // std::cout<<"-----Processing Done-------"<<std::endl;
        // temp_b=b.copy();
        // temp_b->do_move(this->best_move);
        // temp_b->data.player_to_play = (PlayerColor)(temp_b->data.player_to_play ^ (WHITE | BLACK));
        // int num_moves=temp_b->get_legal_moves().size();        
        // temp_b->data.player_to_play = (PlayerColor)(temp_b->data.player_to_play ^ (WHITE | BLACK));
        // undo_last_move(this->best_move,temp_b);
        // for(int x=ans.size()-2;x>=0;x--)
        // {
        //     if(ans[x].first==ans[x+1].first)
        //     {
        //         temp_b->do_move(ans[x].second);
        //         temp_b->data.player_to_play = (PlayerColor)(temp_b->data.player_to_play ^ (WHITE | BLACK));
        //         int temp_num_moves=temp_b->get_legal_moves().size();        
        //         temp_b->data.player_to_play = (PlayerColor)(temp_b->data.player_to_play ^ (WHITE | BLACK));
        //         undo_last_move(ans[x].second,temp_b);
        //         if(temp_num_moves > num_moves)
        //         {
        //             num_moves=temp_num_moves;
        //             this->best_move=ans[x].second;
        //         }
        //     }
        //     else
        //         break;
        // }
        // delete temp_b;
    }

}
