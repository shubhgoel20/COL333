#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <climits>
#include <map>
#include <assert.h>
#include <queue>
#include <unordered_map>
#include "board.hpp"
#include "engine.hpp"

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col);
int maxval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col);

bool time_check(std::chrono::high_resolution_clock::time_point& start_time){
    auto cur_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start_time);
    if(duration.count()>1900)
    {
        std::cout<<"Time up"<<std::endl;
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

    int weights[7][7]={
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
        {1,0,1,0,1,0,1},
    };


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

    int black_wt=0,white_wt=0;
    // if(board_copy->data.b_bishop!=DEAD)
    //     white_wt=1;
    // if(board_copy->data.w_bishop!=DEAD)
    //     black_wt=1;

    U8 *pieces = (U8*)board_copy;
    for (int i=0; i<12; i++) {
        U8 position = pieces[i];
        if(position==DEAD)  continue;
        U8 piece = board_copy->data.board_0[position];
        if(piece & WHITE)
            evaluation+=weights[getx(position)][gety(position)]*threat_weight(piece)*white_wt;
        else
            evaluation+=weights[getx(position)][gety(position)]*threat_weight(piece)*black_wt;
    }
   
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
        // std::cout<<"MAXVAL Output = "<<temp<<std::endl;
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
    // std::cout<<"MAXVAL Output = "<<ans<<std::endl;
    return ans;
}

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point& start_time,PlayerColor my_col)
{
    if(curr_depth<=0 || time_check(start_time)==0)
    {
        int temp=evaluation_func(b,my_col,start_time);
        // std::cout<<"MINVAL Output = "<<temp<<std::endl;
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
    // std::cout<<"MINVAL Output = "<<ans<<std::endl;
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

#define MOVE_PAIR std::pair<std::pair<int,int>,std::pair<U16,U16>>

struct PQ_Comparator
{
    bool operator()(MOVE_PAIR& p1, MOVE_PAIR& p2)
    {
        std::pair<int,int> &val1=p1.first;
        std::pair<int,int> &val2=p2.first;

        if(val1.first<val2.first)   return true;
        else if(val1.first>val2.first)  return false;
        else return val1.second>val2.second;
    }
};

bool compare_func(MOVE_PAIR& p1, MOVE_PAIR& p2)
{
    std::pair<int,int> &val1=p1.first;
    std::pair<int,int> &val2=p2.first;

    if(val1.first<val2.first)   return true;
    else if(val1.first>val2.first)  return false;
    else return val1.second>val2.second;
}

std::vector<MOVE_PAIR> evaluate_depth_2(Board *b,std::chrono::high_resolution_clock::time_point& start_time)
{
    std::vector<MOVE_PAIR> ans;
    std::unordered_set<U16> moveset1,moveset2;
    Board *board1,*board2;

    moveset1=b->get_legal_moves();
    for(auto move1:moveset1)
    {   
        board1=b->copy();
        board1->do_move(move1);
    
        moveset2=board1->get_legal_moves();
        int max_val=INT_MIN;
        int start=ans.size();   //start inclusive
        int end=ans.size();     //end not inclusive
        for(auto move2:moveset2)
        {
            if(time_check(start_time)==0)
            {
                delete board1;
                while(start<end)
                {
                    ans[start].first.first=max_val;
                    start++;
                }
                return ans;
            }


            board2=board1->copy();
            board2->do_move(move2);
            int value=evaluation_func(board2,b->data.player_to_play,start_time);
            if(value>max_val)   max_val=value;
            delete board2;
            ans.push_back(MOVE_PAIR(std::pair<int,int>(0,value),std::pair<U16,U16>(move1,move2)));
            end++;
        }
        if(start==end && board1->in_check())  //moveset 2 was empty
            ans.push_back(MOVE_PAIR(std::pair<int,int>(INT_MAX,0),std::pair<U16,U16>(move1,0)));
        else if(start==end)
            ans.push_back(MOVE_PAIR(std::pair<int,int>(0,0),std::pair<U16,U16>(move1,0)));
        while(start<end)
        {
            ans[start].first.first=max_val;
            start++;
        }
        delete board1;
    }
    return ans;
}

void Engine::find_best_move(const Board& b) 
{
    static int counter=-1;
    auto start_time=std::chrono::high_resolution_clock::now();
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) 
    {
        this->best_move = 0;
    }
    else
    {
        this->best_move=*(moveset.begin());
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
            if(counter>=5)
                opening=0;
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
                        // std::cout<<"----------------Doing Preferred Move--------------"<<std::endl;
                        this->best_move=preferred_move;
                        return;
                    }
                }
            }
            
        }

        int num_powerful_pieces=(b.data.b_bishop!=DEAD) + (b.data.b_rook_bs!=DEAD) +(b.data.b_rook_ws!=DEAD);
        num_powerful_pieces+=(b.data.w_bishop!=DEAD) + (b.data.w_rook_bs!=DEAD) +(b.data.w_rook_ws!=DEAD);
        num_powerful_pieces+= (b.data.b_pawn_ws!=DEAD && !(b.data.board_0[b.data.b_pawn_ws] & PAWN));
        num_powerful_pieces+= (b.data.b_pawn_bs!=DEAD && !(b.data.board_0[b.data.b_pawn_bs] & PAWN));
        num_powerful_pieces+= (b.data.w_pawn_ws!=DEAD && !(b.data.board_0[b.data.w_pawn_ws] & PAWN));
        num_powerful_pieces+= (b.data.w_pawn_bs!=DEAD && !(b.data.board_0[b.data.w_pawn_bs] & PAWN));
        

        Board* temp_b=b.copy();
        int max_depth=2;
        if(num_powerful_pieces<=2)  max_depth=4;
        auto ans=minimax(temp_b,max_depth,start_time,b.data.player_to_play);
        delete temp_b;
        this->best_move=ans[ans.size()-1].second;
        // auto ans=evaluate_depth_2(temp_b,start_time);
        
        // sort(ans.begin(),ans.end(),compare_func);
        
        // if(ans[0].first.first==INT_MAX) //checkmate move
        //     this->best_move=ans[0].second.first;
        // else if(time_check(start_time))
        // {
        //     unsigned long int x=0;
        //     int max_val=INT_MIN;
            
        //     while(x<ans.size() && time_check(start_time))            
        //     {
        //         U16 first_move=ans[x].second.first;
        //         unsigned long int y=x;
        //         int min_val=INT_MAX;
        //         while(y<ans.size() && ans[y].second.first==first_move && time_check(start_time))
        //         {
        //             temp_b=b.copy();
        //             temp_b->do_move(first_move);
        //             temp_b->do_move(ans[y].second.second);
        //             int curr_val=maxval(temp_b,2,INT_MIN,INT_MAX,start_time,b.data.player_to_play);
        //             if(curr_val<min_val)
        //             {
        //                 min_val=curr_val;
        //             }
        //             y++;
        //         }
        //         if(min_val>=max_val)
        //         {
        //             max_val=min_val;
        //             this->best_move=first_move;
        //         }
        //         x=y;
        //     }
        // }
        // else
        // {
            // this->best_move=ans[0].second.first;
        // }

        // if(time_check(start_time))
        // {
        //     int max_val=INT_MIN;
        //     long unsigned int begin=0;            
        //     long unsigned int x;
        //     for(x=0;x<ans.size()-1;x++)
        //     {
        //         temp_b=b.copy();
        //         temp_b->do_move(ans[x].second.first);
        //         temp_b->do_move(ans[x].second.second);
        //         int curr_val=maxval(temp_b,2,INT_MIN,INT_MAX,start_time,b.data.player_to_play);
        //         delete temp_b;
        //         ans[x].first.second=curr_val;
        //         if(curr_val>max_val)    max_val=curr_val;
        //         if(ans[x].second.first != ans[x+1].second.second)
        //         {
        //             for(long unsigned int y=begin;y<=x;y++)
        //             {
        //                 ans[y].first.first=max_val;
        //             }                        
        //             begin=x+1;
        //             max_val=INT_MIN;
        //         }               
        //         if(time_check(start_time)==0)
        //         {
        //             break;
        //         }
        //     }
        //     for(long unsigned int y=begin;y<=x;y++)
        //     {
        //         ans[y].first.first=max_val;
        //     }                        
        //     sort(ans.begin(),ans.begin()+x,compare_func);
        // }   
        
        // this->best_move=ans[0].second.first;
        // this->best_move=ans[0].second.first;
        // int x;
        // for(x=ans.size()-1;x>=0;x--)
        // {
        //     temp_b=b.copy();
        //     temp_b->do_move(ans[x].second);
        //     if(temp_b->in_check())
        // }
        // this->best_move=ans[ans.size()-1].second;

        // if(preferred_move==0)
        // {
        //     for(auto p:ans)
        //     {
        //         std::cout<<"Value="<<p.first<<" "<<"Move="<<move_to_str(p.second)<<std::endl;
        //     }
        // std::cout<<"Hello World"<<std::endl;
        // std::cout<<"selected move = "<<move_to_str(this->best_move)<<"   Value = "<<ans[ans.size()-1].first<<std::endl;
        //     // exit(0);
        // }
    }
}
