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


constexpr U8 cw_90[64] = {
    48, 40, 32, 24, 16, 8,  0,  7,
    49, 41, 33, 25, 17, 9,  1,  15,
    50, 42, 18, 19, 20, 10, 2,  23,
    51, 43, 26, 27, 28, 11, 3,  31,
    52, 44, 34, 35, 36, 12, 4,  39,
    53, 45, 37, 29, 21, 13, 5,  47,
    54, 46, 38, 30, 22, 14, 6,  55,
    56, 57, 58, 59, 60, 61, 62, 63
};

constexpr U8 acw_90[64] = {
     6, 14, 22, 30, 38, 46, 54, 7,
     5, 13, 21, 29, 37, 45, 53, 15,
     4, 12, 18, 19, 20, 44, 52, 23,
     3, 11, 26, 27, 28, 43, 51, 31,
     2, 10, 34, 35, 36, 42, 50, 39,
     1,  9, 17, 25, 33, 41, 49, 47,
     0,  8, 16, 24, 32, 40, 48, 55,
    56, 57, 58, 59, 60, 61, 62, 63
};

constexpr U8 cw_180[64] = {
    54, 53, 52, 51, 50, 49, 48, 7,
    46, 45, 44, 43, 42, 41, 40, 15,
    38, 37, 18, 19, 20, 33, 32, 23,
    30, 29, 26, 27, 28, 25, 24, 31,
    22, 21, 34, 35, 36, 17, 16, 39,
    14, 13, 12, 11, 10,  9,  8, 47,
     6,  5,  4,  3,  2,  1,  0, 55,
    56, 57, 58, 59, 60, 61, 62, 63
};

constexpr U8 id[64] = {
     0,  1,  2,  3,  4,  5,  6,  7,
     8,  9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23,
    24, 25, 26, 27, 28, 29, 30, 31,
    32, 33, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 45, 46, 47,
    48, 49, 50, 51, 52, 53, 54, 55,
    56, 57, 58, 59, 60, 61, 62, 63
};

#define cw_90_pos(p) cw_90[p]
#define cw_180_pos(p) cw_180[p]
#define acw_90_pos(p) acw_90[p]
#define cw_90_move(m) move_promo(cw_90[getp0(m)], cw_90[getp1(m)], getpromo(m))
#define acw_90_move(m) move_promo(acw_90[getp0(m)], acw_90[getp1(m)], getpromo(m))
#define cw_180_move(p) move_promo(cw_180[getp0(m)], cw_180[getp1(m)], getpromo(m))
#define color(p) ((PlayerColor)(p & (WHITE | BLACK)))


void undo_last_move(U16 move, Board* board)
{
    
    U8 p0 = getp0(move);
    U8 p1 = getp1(move);
    U8 promo = getpromo(move);

    U8 piecetype = board->data.board_0[p1];
    U8 deadpiece = board->data.last_killed_piece;
    board->data.last_killed_piece = 0;

    // scan and get piece from coord
    U8 *pieces = (U8*)(&(board->data));
    for (int i=0; i<12; i++) {
        if (pieces[i] == p1) {
            pieces[i] = p0;
            break;
        }
    }
    if (board->data.last_killed_piece_idx >= 0) {
        pieces[board->data.last_killed_piece_idx] = p1;
        board->data.last_killed_piece_idx = -1;
    }

    if (promo == PAWN_ROOK) {
        piecetype = ((piecetype & (WHITE | BLACK)) ^ ROOK) | PAWN;
    }
    else if (promo == PAWN_BISHOP) {
        piecetype = ((piecetype & (WHITE | BLACK)) ^ BISHOP) | PAWN;
    }

    board->data.board_0[p0]           = piecetype;
    board->data.board_90[cw_90[p0]]   = piecetype;
    board->data.board_180[cw_180[p0]] = piecetype;
    board->data.board_270[acw_90[p0]] = piecetype;

    board->data.board_0[p1]           = deadpiece;
    board->data.board_90[cw_90[p1]]   = deadpiece;
    board->data.board_180[cw_180[p1]] = deadpiece;
    board->data.board_270[acw_90[p1]] = deadpiece;

}


bool time_check(std::chrono::high_resolution_clock::time_point start_time){
    auto cur_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start_time);
    if(duration.count()>1900)
    {
        std::cout<<"Time up"<<std::endl;
        return 0;
    }
    return 1;
}

int evaluation_func(Board* board_copy,PlayerColor my_col)
{
    int w_king=7,w_bishop=5,w_rook=4,w_pawn=2,w_moves=0;
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
            return -1*INTMAX;
        else
            return INTMAX;

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
    // if(count>=3)
    // {
    //     std::cout<<"YAY Parinting"<<std::endl;
    //     std::cout<<"Pawns = "<<num_pawns[0]<<","<<num_pawns[1]<<std::endl;
    //     std::cout<<"Rooks = "<<num_rooks[0]<<","<<num_rooks[1]<<std::endl;
    //     std::cout<<"Bishops = "<<num_bishops[0]<<","<<num_bishops[1]<<std::endl;
    //     std::cout<<"Moves = "<<moveset[0].size()<<","<<moveset[1].size()<<std::endl;
    // }
    return evaluation;      

}

int minval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col);

int maxval(Board *b, int curr_depth, int alpha, int beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col)
{
    if(curr_depth==0  || time_check(start_time)==0)
    {
        return evaluation_func(b,my_col);
    }
    auto moveset=b->get_legal_moves();
    Board* temp_board=b->copy();
    std::vector<std::pair<int,U16>> moveset_ordered;
    for(auto move:moveset)
    {
        temp_board->do_move(move);
        moveset_ordered.push_back(std::pair<int,U16>(evaluation_func(temp_board,my_col),move));
        undo_last_move(move,temp_board);
    }
    delete temp_board;
    sort(moveset_ordered.begin(),moveset_ordered.end());
    reverse(moveset_ordered.begin(),moveset_ordered.end());
    for(auto p:moveset_ordered)
    {
        U16 move=p.second;
        temp_board=b->copy();
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
    if(curr_depth==0 || time_check(start_time)==0)
    {
        return evaluation_func(b,my_col);
    }
    auto moveset=b->get_legal_moves();

    Board* temp_board=b->copy();
    std::vector<std::pair<int,U16>> moveset_ordered;
    for(auto move:moveset)
    {
        temp_board->do_move(move);
        moveset_ordered.push_back(std::pair<int,U16>(evaluation_func(temp_board,my_col),move));
        undo_last_move(move,temp_board);
    }
    delete temp_board;
    sort(moveset_ordered.begin(),moveset_ordered.end());

    for(auto p:moveset_ordered)
    {
        U16 move=p.second;
        temp_board=b->copy();
        temp_board->do_move(move);
        beta=std::min(beta,maxval(temp_board,curr_depth-1,alpha,beta,start_time,my_col));
        delete temp_board;
        if(alpha>=beta)
            return beta;
    }
    return beta;
}

std::vector<std::pair<int,U16>> minimax(Board *b, int curr_depth,std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col, std::atomic<U16>& best_move)
{
    std::vector<std::pair<int,U16>> ans;
    auto moveset=b->get_legal_moves();
    int max_val=INT_MIN;
    for(auto move : moveset)
    {
        Board * temp_board=b->copy();
        temp_board->do_move(move);
        int val = minval(temp_board,curr_depth-1,-1*INTMAX,INTMAX,start_time,my_col);
        delete temp_board;
        if(val>max_val)
        {
            best_move=move;
            max_val=val;
        }
        ans.push_back(std::pair<int,U16>(val,move));
    }
    sort(ans.begin(),ans.end());
    return ans;
}



void test2(Board * board)
{
    auto moveset = board->get_legal_moves();
    for(auto move:moveset)
    {
        // std::cout<<"----------------Doing New Move---------------"<<std::endl;
        std::string s1=board_to_str(board->data.board_0);
        board->do_move(move);
        std::string s2=board_to_str(board->data.board_0);
        undo_last_move(move,board);
        std::string s3=board_to_str(board->data.board_0);
        if(s1==s2)
        {    
            std::cout<<"Do failed as s1==s2, move = "<<move<<std::endl;
            exit(0);
        }
        else if(s2==s3)
        {
            std::cout<<"undo failed as s2==s3, move = "<<move<<std::endl;
            exit(0);
        }
        if(!(s1==s3))
        {
            std::cout<<"Undo failed as s1!=s3, move = "<<move<<std::endl;   
            exit(0);
        }
    }
}

void test(Board * board)
{
    auto moveset = board->get_legal_moves();
    for(auto move:moveset)
    {
        // std::cout<<"----------------Doing New Move---------------"<<std::endl;
        // std::string s1=all_boards_to_str(*board);
        std::string s1=board_to_str(board->data.board_0);
        board->do_move(move);
        std::string s2=board_to_str(board->data.board_0);
        // Board * temp=board->copy();
        test2(board);
        // delete temp;
        undo_last_move(move,board);
        std::string s3=board_to_str(board->data.board_0);
        if(s1==s2)
        {    
            std::cout<<"Do failed as s1==s2, move = "<<move<<std::endl;
            exit(0);
        }
        else if(s2==s3)
        {
            std::cout<<"undo failed as s2==s3, move = "<<move<<std::endl;
            exit(0);
        }
        if(!(s1==s3))
        {
            std::cout<<"Undo failed as s1!=s3, move = "<<move<<std::endl;   
            exit(0);
        }
    }
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
        // this->best_move=*(moveset.begin());
        //Test 0
        // std::string s=board_to_str(b.data.board_0);
        // std::cout<<s<<std::endl;
        // Test 1
        // Board* temp=b.copy();
        // test(temp);
        // delete temp;

        auto start_time=std::chrono::high_resolution_clock::now();
        Board* temp_b=b.copy();
        auto ans=minimax(temp_b,3,start_time,b.data.player_to_play,this->best_move);
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
