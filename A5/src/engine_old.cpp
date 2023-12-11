#include <algorithm>
#include <random>
#include <iostream>
#include <cstring>
#include <unordered_map>
// #include <fstream>

#include "board.hpp"
#include "engine_old.hpp"
#include "butils.hpp"

int Engine::curr_time_left()
{
    auto curr_time = std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(curr_time-start_time) - time_left;
    return duration.count();
}

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

void Engine::feature_values(const Board& b, std::vector<int>& ans)
{
    PawnInfo pi(b);
    int r1 = rook_count(b,my_player,pi);
    int b1 = bishop_count(b,my_player,pi);
    int p1 = pawn_count(b,my_player,pi);
    int k1 = knight_count(b,my_player,pi);
    
    int r2 = rook_count(b,oppcolor(my_player),pi);
    int b2 = bishop_count(b,oppcolor(my_player),pi);
    int p2 = pawn_count(b,oppcolor(my_player),pi);
    int k2 = knight_count(b,oppcolor(my_player),pi);
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
}

float Engine::state_value(const Board& b)
{
    float ans=0;
    std::vector<int> temp;
    feature_values(b,temp);
    for(int x=0;x<weights.value.size();x++)
    {
        ans+=weights.value[x]*temp[x];
    }
    if(b.data.board_type == SEVEN_THREE)
    {
        
    }
    else if(b.data.board_type == EIGHT_FOUR)
    {

    }
    else
    {

    }
    return ans;
}

bool Engine::should_cutoff(const Board& b,int curr_depth, int max_depth)
{
    return (curr_depth>=max_depth);
}


// std::pair<float,U16> Engine::transposition_table(const Board& b)
// {
//     std::pair<float,U16> ans;
//     ans.first=0;
//     ans.second=0;
//     if(b.data.player_to_play==WHITE)
//     {
//         auto itr = white_player.find(board_to_str(&b.data));
//         if(itr!=white_player.end())
//         {
//             ans.first=itr->second.first;
//             ans.second=itr->second.second;
//         }
//     }
//     else if(b.data.player_to_play==BLACK)
//     {
//         auto itr = black_player.find(board_to_str(&b.data));
//         if(itr!=black_player.end())
//         {
//             ans.first=itr->second.first;
//             ans.second=itr->second.second;
//         }
//     }
//     return ans;
// }


float Engine::maxval(const Board& b, int curr_depth,int max_depth, float alpha, float beta)
{
    if(should_cutoff(b,curr_depth,max_depth))
    {
        float temp=state_value(b);
        return temp;
    }   

    auto moveset=b.get_legal_moves();
    Board temp_b(b);
    float ans=FLOAT_MIN;
    for(auto move:moveset)
    {
        // if(time_check(start_time)==0)   break;
        temp_b.do_move_(move);
        float temp_ans=minval(temp_b,curr_depth+1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();

        ans=std::max(ans,temp_ans);
        alpha=std::max(alpha,ans);
        if(alpha>=beta)
            break;
    }
    return ans;
}

float Engine::minval(const Board& b, int curr_depth,int max_depth, float alpha, float beta)
{
    if(should_cutoff(b,curr_depth,max_depth))
    {
        float temp=state_value(b);
        return temp;
    }
    auto moveset=b.get_legal_moves();
    Board temp_b(b);
    float ans=FLOAT_MAX;
    for(auto move:moveset)
    {
        // if(time_check(start_time)==0)   break;
        temp_b.do_move_(move);
        float temp_ans=maxval(temp_b,curr_depth+1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();
        ans=std::min(ans,temp_ans);
        beta=std::min(beta,ans);
        if(alpha>=beta)
            break;
    }
    return ans;
}

std::vector<std::pair<float,U16>> Engine::minimax(const Board& b, int max_depth)
{
    Board temp_b(b);
    std::vector<std::pair<float,U16>> ans;
    auto moveset=b.get_legal_moves();
    float val;
    for(auto move : moveset)
    {
        temp_b.do_move_(move);
        val = minval(temp_b,1,max_depth,FLOAT_MIN,FLOAT_MAX);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();
        ans.push_back(std::pair<int,U16>(val,move));
    }
    sort(ans.begin(),ans.end());
    return ans;
}


void Engine::update_weights(const Board& b, float updated_cost, int num_games)
{
    float state_val = state_value(b);

    std::vector<int> temp_feature;
    feature_values(b,temp_feature);
    
    std::cout<<"Old wts"<<std::endl;
    for(int x=0;x<weights.value.size();x++)
    {
        std::cout<<weights.value[x]<<" ";
    }

    // float alpha=0.1/num_games;
    float alpha=0.01;
    for(int x=0;x<weights.value.size();x++)
    {
        weights.value[x]+=alpha*(updated_cost - state_val)*temp_feature[x];
    }

    std::cout<<"New wts"<<std::endl;
    for(int x=0;x<weights.value.size();x++)
    {
        std::cout<<weights.value[x]<<" ";
    }
    
}



void Engine::find_best_move(const Board& b) {
    static const int range_from  = 0;
    static const int range_to    = 1000;
    static std::random_device rand_dev;
    static std::mt19937 generator(rand_dev());
    static std::uniform_int_distribution<int>  distr(range_from, range_to);
    // pick a random move
    my_player=b.data.player_to_play;
    start_time=std::chrono::high_resolution_clock::now();
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        std::cout << "Could not get any moves from board!\n";
        std::cout << board_to_str(&b.data);
        this->best_move = 0;
    }
    else
    {
        if(learn_weights==false)
        {
            auto vect = minimax(b,4);
            this->best_move = vect.back().second;
            this->best_move_cost=vect.back().first;
        }
        else
        {
            float rand_val = distr(generator)/1000.0;
            if(rand_val>0.5)
            {
                auto vect = minimax(b,2);
                this->best_move = vect.back().second;
                this->best_move_cost=vect.back().first;
            }
            else
            {
                int rand_move = (int) (rand_val * moveset.size());
                if(rand_move==moveset.size())
                    rand_move--;
                for(auto move:moveset)
                {
                    if(rand_move==0)
                    {
                        this->best_move=move;                        
                        this->best_move_cost=0;
                        break;
                    }
                    rand_move--;
                }
            }
        }
    }
    /*
    else {
        std::vector<U16> moves;
        std::cout << show_moves(&b.data, moveset) << std::endl;
        for (auto m : moveset) {
            std::cout << move_to_str(m) << " ";
        }
        std::cout << std::endl;
        std::sample(
            moveset.begin(),
            moveset.end(),
            std::back_inserter(moves),
            1,
            std::mt19937{std::random_device{}()}
        );
        this->best_move = moves[0];
    }
    */
}


void Engine::print_weights()
{
    for(int x=0;x<weights.value.size();x++)
    {
        std::cout<<weights.value[x]<<std::endl;
    }
}

float Engine::minval_quescence(const Board& b, float alpha, float beta)
{
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
            return 0;//utility of stalemate.
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
            float temp_ans = maxval_quescence(temp_b,alpha,beta);
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
        return state_value(b);
    }
    return ans;
}

float Engine::maxval_quescence(const Board& b, float alpha, float beta)
{
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
            return 0;//utility of stalemate.
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
            float temp_ans = minval_quescence(temp_b,alpha,beta);
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
        return state_value(b);
    }
    return ans;
}

float Engine::quescence_search(const Board& b)
{
    if(b.data.player_to_play==my_player)
    {
        return maxval_quescence(b,FLOAT_MIN,FLOAT_MAX);
    }
    else
    {
        return minval_quescence(b,FLOAT_MIN,FLOAT_MAX);
    }
}