#pragma once

#include <cstring>
#include <atomic>
#include "engine_base.hpp"
#define FLOAT_MIN -1e9
#define FLOAT_MAX 1e9

struct piece_data
{
    U8 w_rook_1 = DEAD;
    U8 w_rook_2 = DEAD;
    U8 w_king = DEAD;
    U8 w_bishop = DEAD;
    U8 w_knight_1 = DEAD;
    U8 w_knight_2 = DEAD;
    U8 w_pawn_1 = DEAD;
    U8 w_pawn_2 = DEAD;
    U8 w_pawn_3 = DEAD;
    U8 w_pawn_4 = DEAD;

    U8 b_rook_1 = DEAD;
    U8 b_rook_2 = DEAD;
    U8 b_king = DEAD;
    U8 b_bishop = DEAD;
    U8 b_knight_1 = DEAD;
    U8 b_knight_2 = DEAD;
    U8 b_pawn_1 = DEAD;
    U8 b_pawn_2 = DEAD;
    U8 b_pawn_3 = DEAD;
    U8 b_pawn_4 = DEAD;

    bool is_equal(void* buff)
    {
        if(memcmp(this,buff,sizeof(piece_data))==0)
            return true;
        else
            return false;
    }
};

struct FeatureWts
{
    // float bishop_cnt,rook_cnt,pawn_cnt,knight_cnt,check;
    // float bishop_thrt,rook_thrt,pawn_thrt,knight_thrt;
    // float bishop_rook,bishop_knight,bishop_pawn,rook_knight,rook_pawn,knight_pawn;
    // FeatureWts()
    // {
    //     bishop_cnt=5;rook_cnt=3;pawn_cnt=1;knight_cnt=3;check=-0.5;
    //     bishop_thrt=0;rook_thrt=0;pawn_thrt=0;knight_thrt=0;
    //     bishop_rook=5;bishop_knight=4.5;bishop_pawn=2;rook_knight=3;rook_pawn=1;knight_pawn=1;
    // }
    // ~FeatureWts()
    // {
    //     std::cerr<<"Feature Weights"<<std::endl;
    //     std::cerr<<bishop_cnt<<" "<<rook_cnt<<" "<<pawn_cnt<<" "<<knight_cnt<<" "<<check<<std::endl;
    //     std::cerr<<bishop_thrt<<" "<<rook_thrt<<" "<<pawn_thrt<<" "<<knight_thrt<<std::endl;
    //     std::cerr<<bishop_rook<<" "<<bishop_knight<<" "<<bishop_pawn<<" "<<rook_knight<<" "<<rook_pawn<<" "<<knight_pawn<<std::endl;
    //     std::cerr<<"-----------------------------------"<<std::endl;
    // }
    std::vector<float> value;
    FeatureWts()
    {
        value.push_back(-0.5);
        value.push_back(5);
        value.push_back(3);
        value.push_back(3);
        value.push_back(1);
        value.push_back(5);
        value.push_back(4.5);
        value.push_back(2);
        value.push_back(3);
        value.push_back(1);
        value.push_back(1);
    }
    // ~FeatureWts()
    // {
    //     std::cerr<<"Feature Weights"<<std::endl;
    //     for(float v:value)
    //         std::cerr<<v<<std::endl;
    //     std::cerr<<"------------------------"<<std::endl;
    // }
};

struct PawnInfo
{
    U8 black_pawn_1;
    U8 black_pawn_2;
    U8 black_pawn_3;
    U8 black_pawn_4;

    U8 white_pawn_1;
    U8 white_pawn_2;
    U8 white_pawn_3;
    U8 white_pawn_4;
    PawnInfo(const Board& b)
    {
        black_pawn_1 = (b.data.b_pawn_1!=DEAD) ? b.data.board_0[b.data.b_pawn_1] : 0;
        black_pawn_2 = (b.data.b_pawn_2!=DEAD) ? b.data.board_0[b.data.b_pawn_2] : 0;
        black_pawn_3 = (b.data.b_pawn_3!=DEAD) ? b.data.board_0[b.data.b_pawn_3] : 0;
        black_pawn_4 = (b.data.b_pawn_4!=DEAD) ? b.data.board_0[b.data.b_pawn_4] : 0;

        white_pawn_1 = (b.data.w_pawn_1!=DEAD) ? b.data.board_0[b.data.w_pawn_1] : 0;
        white_pawn_2 = (b.data.w_pawn_2!=DEAD) ? b.data.board_0[b.data.w_pawn_2] : 0;
        white_pawn_3 = (b.data.w_pawn_3!=DEAD) ? b.data.board_0[b.data.w_pawn_3] : 0;
        white_pawn_4 = (b.data.w_pawn_4!=DEAD) ? b.data.board_0[b.data.w_pawn_4] : 0;
    }

};

class Engine : public AbstractEngine {

    // add extra items here. 
    // Note that your engine will always be instantiated with the default 
    // constructor.
    std::chrono::high_resolution_clock::time_point start_time;
    PlayerColor my_player;
    
    //maps for transposition table, colours mean the turn of player
    // std::unordered_map<std::string,std::pair<float,U16>> black_player;
    // std::unordered_map<std::string,std::pair<float,U16>> white_player;
    FeatureWts weights;
    

    public:
    float best_move_cost;
    bool learn_weights=false;
    float (*opponent_func)(const Board& b);

    void find_best_move(const Board& b) override;
    std::pair<float,U16> transposition_table(const Board& bdata);  //bool specifies it entry is present, if present float specifies value
    bool should_cutoff(const Board& b,int curr_depth, int max_depth);
    float state_value(const Board& b);
    int curr_time_left();
    float maxval(const Board& b, int curr_depth,int max_depth, float alpha, float beta);
    float minval(const Board& b, int curr_depth,int max_depth, float alpha, float beta);
    std::vector<std::pair<float,U16>> minimax(const Board& b, int max_depth);

    
    int rook_count(const Board&b,PlayerColor color, PawnInfo& pi);
    int bishop_count(const Board&b,PlayerColor color, PawnInfo& pi);
    int pawn_count(const Board&b,PlayerColor color, PawnInfo& pi);
    int knight_count(const Board&b,PlayerColor color, PawnInfo& pi);
    int king_in_check(const Board&b, PlayerColor color);
    void feature_values(const Board& b, std::vector<int>& ans);
    void update_weights(const Board& b, float updated_cost, int num_games);
    void print_weights();
    float minval_quescence(const Board& b, float alpha, float beta);
    float maxval_quescence(const Board& b, float alpha, float beta);
    float quescence_search(const Board& b);
};
