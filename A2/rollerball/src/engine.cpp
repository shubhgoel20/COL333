#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <climits>
#include <map>
#include <assert.h>

#include "board.hpp"
#include "engine.hpp"

struct Node
{
    Node* next;
    U16 move;
};

bool abbort = 0;
double w_king=58.82,w_bishop=36.78,w_rook=55.87,w_pawn=22.27,w_moves=0;

bool time_check(std::chrono::high_resolution_clock::time_point start_time){
    auto cur_time=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::milliseconds>(cur_time-start_time);
    if(duration.count()>1900) return 0;
    return 1;
}

void update_weights(Board* board_copy, double backed_up_value, PlayerColor my_col, bool train){
    if(!train) return;
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
        return;

    }
    else if( (curr_col==WHITE && moveset[0].size()==0) || (curr_col==BLACK && moveset[1].size()==0) )  //stalemate
        return;
    
    U8 black_pawn_1= (d->b_pawn_bs!=DEAD) ? d->board_0[d->b_pawn_bs] : 0;
    U8 black_pawn_2= (d->b_pawn_ws!=DEAD) ? d->board_0[d->b_pawn_ws] : 0;
    U8 white_pawn_1= (d->w_pawn_bs!=DEAD) ? d->board_0[d->w_pawn_bs] : 0;
    U8 white_pawn_2= (d->w_pawn_ws!=DEAD) ? d->board_0[d->w_pawn_ws] : 0;
    
    double num_rooks[2]={0};
    double num_bishops[2]={0};
    double num_pawns[2]={0};
    num_rooks[0]=(d->w_rook_bs!=DEAD) + (d->w_rook_ws!=DEAD) + ((white_pawn_1 & ROOK)!=0) + ((white_pawn_2 & ROOK)!=0);
    num_rooks[1]=(d->b_rook_bs!=DEAD) + (d->b_rook_ws!=DEAD) + ((black_pawn_1 & ROOK)!=0) + ((black_pawn_2 & ROOK)!=0);
    num_bishops[0]=(d->w_bishop!=DEAD) + ((white_pawn_1 & BISHOP)!=0) + ((white_pawn_2 & BISHOP)!=0);
    num_bishops[1]=(d->b_bishop!=DEAD) + ((black_pawn_1 & BISHOP)!=0) + ((black_pawn_2 & BISHOP)!=0);
    num_pawns[0]=((white_pawn_1 & PAWN)!=0) + ((white_pawn_2 & PAWN)!=0);
    num_pawns[1]=((black_pawn_1 & PAWN)!=0) + ((black_pawn_2 & PAWN)!=0);
    
    double evaluation=0;
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

    if(backed_up_value == evaluation) return;

    int del = (backed_up_value - evaluation);
    int del_rook = (num_rooks[0] - num_rooks[1]);
    int del_bishop = (num_bishops[0] - num_bishops[1]);
    int del_pawn = (num_pawns[0] - num_pawns[1]);
    int del_mv = (moveset[0].size()-moveset[1].size());

    if(my_col == BLACK){
        del_rook = -del_rook;
        del_bishop = -del_bishop;
        del_pawn = -del_pawn;
        del_mv = -del_mv;
    }
    // if del > 0, the weights of the terms contributing positively should increase
    if(del_rook != 0) w_rook = w_rook + 0.001*abs(w_rook)*(del_rook/abs(del_rook))*(del/abs(del));
    if(del_bishop != 0) w_bishop = w_bishop + 0.001*abs(w_bishop)*(del_bishop/abs(del_bishop))*(del/abs(del));
    if(del_pawn != 0) w_pawn = w_pawn + 0.001*abs(w_pawn)*(del_pawn/abs(del_pawn))*(del/abs(del));
    // if(del_mv != 0) w_moves = w_moves + 0.000001*(del_mv/abs(del_mv))*del;

    int del_king = (my_col != curr_col)?1:-1;
    w_king = w_king + 0.001*(del/abs(del))*(board_copy->in_check())*del_king*abs(w_king);

}
 

double evaluation_func(Board* board_copy,PlayerColor my_col)
{
    // std::cout<<"Hello world"<<std::endl;
    // static int count=-1;
    // count++;
    
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
    
    double num_rooks[2]={0};
    double num_bishops[2]={0};
    double num_pawns[2]={0};
    num_rooks[0]=(d->w_rook_bs!=DEAD) + (d->w_rook_ws!=DEAD) + ((white_pawn_1 & ROOK)!=0) + ((white_pawn_2 & ROOK)!=0);
    num_rooks[1]=(d->b_rook_bs!=DEAD) + (d->b_rook_ws!=DEAD) + ((black_pawn_1 & ROOK)!=0) + ((black_pawn_2 & ROOK)!=0);
    num_bishops[0]=(d->w_bishop!=DEAD) + ((white_pawn_1 & BISHOP)!=0) + ((white_pawn_2 & BISHOP)!=0);
    num_bishops[1]=(d->b_bishop!=DEAD) + ((black_pawn_1 & BISHOP)!=0) + ((black_pawn_2 & BISHOP)!=0);
    num_pawns[0]=((white_pawn_1 & PAWN)!=0) + ((white_pawn_2 & PAWN)!=0);
    num_pawns[1]=((black_pawn_1 & PAWN)!=0) + ((black_pawn_2 & PAWN)!=0);
    
    double evaluation=0;
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
    // std::cout<<evaluation<<"\n";
    return evaluation;      

}
std::pair<double, Node*> min_value(Board *b, int curr_depth, double alpha, double beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col, bool train);
std::pair<double, Node*> max_value(Board *b, int curr_depth, double alpha, double beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col, bool train){
    assert(b->data.player_to_play == my_col);
    // std::cout<<"dbgmax"<<"\n";
    if(curr_depth == 0) return {evaluation_func(b,my_col),nullptr};
    int value = INT_MIN;
    Node* best_node = nullptr;;
    auto moveset = b -> get_legal_moves();
    if (moveset.size() == 0) {
        return {value,nullptr};
    }
    
    auto itr = moveset.begin();
    for(;itr!=moveset.end();itr++){
        if(!time_check(start_time)){
            std::cout<<"No time left to think"<<"\n";
            abbort = 1;
            if(value == INT_MIN) return {INT_MAX,best_node};
            return {value,best_node};
        }
        Node* curr_node = new Node;
        curr_node->move = *itr;
        Board* b_copy;
        b_copy=b->copy();
        b_copy->do_move(*itr);
        auto val_node = min_value(b_copy,curr_depth-1,alpha, beta, start_time,my_col, train);
        curr_node->next = val_node.second;
        if((!abbort) && (val_node.first > value)){
            value = val_node.first;
            best_node = curr_node;
        }
        // delete curr_node;
        // value = std::max(value,curr_value);
        alpha = std::max(alpha,val_node.first);
        delete b_copy;
        if(alpha>=beta){
            // update_weights(b,value,my_col,1);
            return {value,best_node};
        }
        // b_copy = nullptr;
    }
    update_weights(b,value,my_col,train);
    return {value,best_node};

}

std::pair<double, Node*> min_value(Board *b, int curr_depth, double alpha, double beta, std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col, bool train){
    assert(b->data.player_to_play != my_col);
    // std::cout<<"dbgmin"<<"\n";
    if(curr_depth == 0) return {evaluation_func(b,my_col),nullptr};
    int value = INT_MAX;
    Node* best_node = nullptr;
    auto moveset = b -> get_legal_moves();
    if (moveset.size() == 0) {
        return {value,nullptr};
    }
    
    auto itr = moveset.begin();
    for(;itr!=moveset.end();itr++){
        if(!time_check(start_time)){
            std::cout<<"No time left to think"<<"\n";
            abbort = 1;
            if(value == INT_MAX) return {INT_MIN,best_node};
            return {value,best_node};
        }
        Node* curr_node = new Node;
        curr_node->move = *itr;
        Board* b_copy;
        b_copy=b->copy();
        b_copy->do_move(*itr);
        auto val_node = max_value(b_copy,curr_depth-1,alpha, beta, start_time,my_col, train);
        curr_node->next = val_node.second;
        if((!abbort) && (val_node.first < value)){
            value = val_node.first;
            best_node = curr_node;
        }
        // delete curr_node;
        beta = std::min(beta,val_node.first);
        delete b_copy;
        if(alpha >= beta){
            // update_weights(b,value,my_col,1);
            return {value,best_node};
        }
        
        // b_copy = nullptr;
    }
    update_weights(b,value,my_col,train);
    return {value,best_node};
}

std::vector<Node*> minimax(Board *b, int curr_depth, std::unordered_set<U16> &moveset,std::chrono::high_resolution_clock::time_point start_time,PlayerColor my_col, bool train){
    assert(b->data.player_to_play == my_col);
    std::cout<<"dbg2"<<"\n";
    int value = INT_MIN; //for best moves
    int value2 = INT_MIN; //for 2nd best moves
    // U16 move = *(moveset.begin());
    std::map<int,std::vector<Node*>> moves;
    auto itr = moveset.begin();
    double alpha = INT_MIN;
    double beta = INT_MAX;
    for(;itr!=moveset.end();itr++){
        // std::cout<<"dbgloop"<<"\n";
        if(!time_check(start_time)){
            std::cout<<"No time left to think"<<"\n";
            // if(move == 0){
            //     return *(moveset.begin());
            // }
            abbort = 1;
            return moves[value];
        }
        Node* curr_node = new Node;
        curr_node-> move = *itr;
        Board* b_copy;
        b_copy=b->copy();
        b_copy->do_move(*itr);
        auto val_node = min_value(b_copy,curr_depth-1,alpha, beta, start_time, my_col,train);
        alpha = std::max(alpha, val_node.first);
        curr_node -> next = val_node.second;
        if((!abbort) && (val_node.first > value)){
            value = val_node.first;
            // move = *itr;
            moves[value].push_back(curr_node);
        }
        else if(val_node.first > value2){
            value2 = val_node.first;
            moves[value2].push_back(curr_node);
        }
        else if(val_node.first == value || val_node.first == value2){
            moves[val_node.first].push_back(curr_node);
        }
        // delete curr_node;
        // value = std::min(value,curr_value);
        delete b_copy;
        // b_copy = nullptr;
    }
    // if(value == INT_MIN || value == INT_MAX) return *(moveset.begin());
    
    // int len = moves[value].size();
    std::vector<Node*> temp = moves[value];
    // for(auto mv: moves[value2]){
    //     temp.push_back(mv);
    // }
    std::cout<<temp.size()<<"\n";
    return (temp);
}

void Engine::find_best_move(const Board& b) {

    abbort = 0;
    bool do_minimax = true;
    PlayerColor my_col = b.data.player_to_play;
    auto moveset = b.get_legal_moves();
    if (moveset.size() == 0) {
        this->best_move = 0;
    }
    else if(do_minimax){
        std::cout<<"dbg1"<<"\n";
        int depth = 4;
        Board* b_copy1;
        Board* b_copy2;
        b_copy1=b.copy();
        b_copy2=b.copy();
        auto start_time = std::chrono::high_resolution_clock::now();
        // U16 get_best_move = minimax(b_copy,depth,moveset,start_time,my_col);
        auto best_moves = minimax(b_copy1,depth,moveset,start_time,my_col,0);
        delete b_copy1;
        this->best_move = (best_moves[rand()%best_moves.size()]->move);
        std::cout<<"after best move"<<"\n";
        std::cout<<"########## New Weights #############"<<std::endl;
        std::cout<<w_king<<" "<<w_bishop<<" "<<w_rook<<" "<<w_pawn<<" "<<w_moves<<std::endl;
        // secondary check
        // if(time_check(start_time)){
        //     // std::cout<<"here"<<"\n";
        //     int best_value = INT_MIN;
        //     for(auto curr: best_moves){
        //         Board* b_copy;
        //         b_copy = b_copy2->copy();
        //         Node* temp = curr;
        //         // std::cout<<"here2"<<"\n";
        //         while(temp != nullptr){
        //             b_copy->do_move(temp->move);
        //             temp = temp->next;
        //         }
        //         auto val_node = b_copy->data.player_to_play == my_col? max_value(b_copy,4,INT_MIN,INT_MAX,start_time,my_col,0):min_value(b_copy,4,INT_MIN,INT_MAX,start_time,my_col,0);
        //         if((!abbort) && (val_node.first > best_value)){
        //             this->best_move = curr->move;
        //         }
        //     }
        //     // std::unordered_set<U16>temp(best_moves.begin(),best_moves.end());
        //     // std::vector<U16> get_best_move = minimax(b_copy2,1,temp,start_time,my_col);
        //     // this->best_move = get_best_move[rand()%best_moves.size()];
        //     // delete b_copy2;
        // }
        

        // assert(get_best_move != (U16)0);
        // std::cout<<get_best_move<<"\n";
        
        
    }
    else {
        Board* b_copy;
        // std::vector<U16> moves(moveset.begin(),moveset.end());
    
        std::unordered_set<U16>::iterator itr=moveset.begin();
        U16 curr_best_move=*itr;

        b_copy=b.copy();
        b_copy->do_move(curr_best_move);
        int max_eval=evaluation_func(b_copy,my_col);
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
                int temp_eval=evaluation_func(b_copy,my_col);
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
