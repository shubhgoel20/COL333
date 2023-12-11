#include <algorithm>
#include <random>
#include <iostream>
#include <cstring>
#include <unordered_map>
// #include <fstream>

#include "board.hpp"
#include "engine.hpp"
#include "butils.hpp"

// std::unordered_map<unsigned long long int,std::unordered_map<unsigned long long int, std::pair<U16,float>>> moveTable;
std::unordered_map<std::string,float> moveTable;

int tot_minimax,found_minimax;




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

void Engine::feature_values(const Board& b, std::vector<int>& ans, PawnInfo& pi)
{    
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

float Engine::state_value(const Board& b, int depth)
{
    PawnInfo pi(b);
    float ans=0;
    std::vector<int> temp;
    feature_values(b,temp,pi);
    for(int x=0;x<weights.value.size();x++)
    {
        ans+=weights.value[x]*temp[x];
    }
    ans+= 20*(pawn_structure(b,my_player,pi) - pawn_structure(b,oppcolor(my_player),pi));

    if(b.data.board_type == SEVEN_THREE)
    {
        
    }
    else if(b.data.board_type == EIGHT_FOUR)
    {

    }
    else
    {

    }
    return ans/depth;
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
    tot_minimax++;
    // unsigned long long int pawn_encod = pawn_Encoding(b);
    // unsigned long long int state_encod = state_Encoding(b,curr_depth,b.data.player_to_play);
    std::string board_str = state_Encoding(b,curr_depth,b.data.player_to_play);
    auto itr1 = moveTable.find(board_str);
    if(itr1!=moveTable.end())
    {
        found_minimax++;
        return itr1->second;
        // std::cout<<"Found in Table"<<std::endl;
        // auto itr2 = itr1->second.find(state_encod);
        // if(itr2!=itr1->second.end())
        // {
            // return itr2->second.second;
        // }
    }


    if(should_cutoff(b,curr_depth,max_depth))
    {
        // float temp=state_value(b);
        // return temp;
        return quescence_search(b,curr_depth);
    }   

    auto moveset=b.get_legal_moves();
    Board temp_b(b);
    float ans=FLOAT_MIN;
    U16 best_move;
    for(auto move:moveset)
    {
        // if(time_check(start_time)==0)   break;
        temp_b.do_move_(move);
        float temp_ans=minval(temp_b,curr_depth+1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();

        // ans=std::max(ans,temp_ans);
        if(temp_ans>ans)
        {
            ans=temp_ans;
            best_move=move;
        }
        alpha=std::max(alpha,ans);
        if(alpha>=beta)
            break;
    }

    moveTable.insert({board_str,ans});

    // if(itr1!=moveTable.end())
    // {
    //     itr1->second.insert({state_encod,std::pair<U16,float>(best_move,ans)});
    // }
    // else
    // {
    //     std::unordered_map<unsigned long long int,std::pair<U16,float>> temp;
    //     temp.insert({state_encod,std::pair<U16,float>(best_move,ans)});
    //     moveTable.insert({pawn_encod,temp});
    // }


    return ans;
}

float Engine::minval(const Board& b, int curr_depth,int max_depth, float alpha, float beta)
{
    tot_minimax++;
    // unsigned long long int pawn_encod = pawn_Encoding(b);
    // unsigned long long int state_encod = state_Encoding(b,curr_depth,b.data.player_to_play);
    std::string board_str = state_Encoding(b,curr_depth,b.data.player_to_play);
    auto itr1 = moveTable.find(board_str);
    if(itr1!=moveTable.end())
    {
        found_minimax++;
        return itr1->second;
        // auto itr2 = itr1->second.find(state_encod);
        // if(itr2!=itr1->second.end())
        // {
        //     // std::cout<<"Found in Table"<<std::endl;
        //     found_minimax++;
        //     return itr2->second.second;
        // }
    }

    if(should_cutoff(b,curr_depth,max_depth))
    {
        // float temp=state_value(b);
        // return temp;
        return quescence_search(b,curr_depth);
    }
    auto moveset=b.get_legal_moves();
    Board temp_b(b);
    float ans=FLOAT_MAX;
    U16 best_move;
    for(auto move:moveset)
    {
        // if(time_check(start_time)==0)   break;
        temp_b.do_move_(move);
        float temp_ans=maxval(temp_b,curr_depth+1,max_depth,alpha,beta);
        temp_b.undo_last_move_without_flip_(move);
        temp_b.flip_player_();
        // ans=std::min(ans,temp_ans);
        if(temp_ans<ans)
        {
            ans=temp_ans;
            best_move=move;
        }
        beta=std::min(beta,ans);
        if(alpha>=beta)
            break;
    }

    // if(itr1!=moveTable.end())
    // {
    //     itr1->second.insert({state_encod,std::pair<U16,float>(best_move,ans)});
    // }
    // else
    // {
    //     std::unordered_map<unsigned long long int,std::pair<U16,float>> temp;
    //     temp.insert({state_encod,std::pair<U16,float>(best_move,ans)});
    //     moveTable.insert({pawn_encod,temp});
    // }
    moveTable.insert({board_str,ans});
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

float Engine::minval_quescence(const Board& b, float alpha, float beta, int curr_depth)
{
    if(curr_depth>debug_max_depth)
        debug_max_depth=curr_depth;

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
        return state_value(b,curr_depth);
    }
    return ans;
}

float Engine::maxval_quescence(const Board& b, float alpha, float beta, int curr_depth)
{
    if(curr_depth>debug_max_depth)
        debug_max_depth=curr_depth;

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
        return state_value(b,curr_depth);
    }
    return ans;
}

float Engine::quescence_search(const Board& b, int curr_depth)
{
    debug_max_depth=0;
    float ans;
    if(b.data.player_to_play==my_player)
    {
        ans =  maxval_quescence(b,FLOAT_MIN,FLOAT_MAX,curr_depth);
    }
    else
    {
        ans = minval_quescence(b,FLOAT_MIN,FLOAT_MAX,curr_depth);
    }
    // std::cout<<"Quescence Search went till depth = "<<debug_max_depth<<std::endl;
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


void Engine::find_best_move(const Board& b) {
    
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
        moveTable.clear();     
        tot_minimax=0;
        found_minimax=0;   
        auto time1=std::chrono::high_resolution_clock::now();

        auto vect = minimax(b,4);
        
        this->best_move = vect.back().second;
        
        


        // U8 p0=getp0(this->best_move);
        // if(b.data.board_0[p0] & PAWN)
        // {
        //     auto itr = moveTable.find(pawn_Encoding(b));
        //     moveTable.erase(itr);
        //     for(auto move:moveset)
        //     {
        //         if( ( b.data.board_0[getp0(move)] & PAWN ) &&  move!=this->best_move )
        //         {
        //             Board temp_b(b);
        //             temp_b.do_move_(move);
        //             auto itr2 = moveTable.find(pawn_Encoding(temp_b));
        //             if(itr2!=moveTable.end())
        //             {
        //                 moveTable.erase(itr2);
        //             }
        //         }
        //     }
        // }

        auto time2=std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time2-time1);        
        std::cout<<"---------------------------------------------------------------------------------"<<std::endl;
        std::cout<<"Time taken in search = "<<duration.count()<<" ms"<<std::endl;
        std::cout<<"Table size = "<<moveTable.size()<<std::endl;
        std::cout<<"Total Searches = "<<tot_minimax<<std::endl;
        std::cout<<"Found searches = "<<found_minimax<<std::endl;
        std::cout<<"---------------------------------------------------------------------------------"<<std::endl;
        // this->best_move_cost=vect.back().first;   
        // while(true)
        // {
        //     auto time = std::chrono::high_resolution_clock::now();
        //     auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(time-start_time);
        //     if(duration.count() >1500)
        //         break;
        // }
    }
}

void Engine::printTable()
{
    for(auto p:moveTable)
    {
        std::cout<<std::endl;
        std::cout<<p.first<<std::endl;
        std::cout<<"Value = "<<p.second<<std::endl;
        std::cout<<std::endl;
    }
}
