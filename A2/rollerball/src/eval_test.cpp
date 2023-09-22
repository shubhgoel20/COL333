#include "board.hpp"

#define WT_MULTIPLIER 1000

int evaluation_func(Board* board_copy,PlayerColor my_col)
{
    // int mult1=20000,mult2=500,mult3=1;
    int w_king=7*WT_MULTIPLIER,w_bishop=5*WT_MULTIPLIER,w_rook=3*WT_MULTIPLIER,w_pawn=1*WT_MULTIPLIER;    
    // int w_threat_bishop=3,w_threat_rook=2,w_threats_pawn=1;
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
    //  In stalement points pieces alive can determine the points obtained
    // else if( (curr_col==WHITE && moveset[0].size()==0) || (curr_col==BLACK && moveset[1].size()==0) )  //stalemate
    //     return Value(0,0,0);
    
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


    //threat calculation
    // int threat_pawns[2]={0};
    // int threat_rooks[2]={0};
    // int threat_bishops[2]={0};
    // for(int i=0;i<2;i++)
    // {
    //     for(auto move:moveset[1-i])
    //     {
    //         U8 position=getp1(move);           
    //         U8 piecetype=board_copy->data.board_0[position];
    //         if(piecetype!=EMPTY)
    //         {
    //             if(piecetype & PAWN)            threat_pawns[i]+=1;
    //             else if(piecetype & ROOK)       threat_rooks[i]+=1;
    //             else if(piecetype & BISHOP)     threat_bishops[i]+=1;
    //         }
    //     }
    // }



    int evaluation=0;
    
    evaluation+=(num_rooks[0]-num_rooks[1])*w_rook;
    evaluation+=(num_bishops[0]-num_bishops[1])*w_bishop;
    evaluation+=(num_pawns[0]-num_pawns[1])*w_pawn;

    // evaluation+=(threat_bishops[0]-threat_bishops[1])*w_threat_bishop;
    // evaluation+=(threat_rooks[0]-threat_rooks[1])*w_threat_rook;
    // evaluation+=(threat_pawns[0]-threat_pawns[1])*w_threats_pawn;
    
    // evaluation+=(moveset[0].size()-moveset[1].size())*w_moves;
    
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
        // std::cout<<"YAY Parinting"<<std::endl;
        // std::cout<<"Pawns = "<<num_pawns[0]<<","<<num_pawns[1]<<std::endl;
        // std::cout<<"Rooks = "<<num_rooks[0]<<","<<num_rooks[1]<<std::endl;
        // std::cout<<"Bishops = "<<num_bishops[0]<<","<<num_bishops[1]<<std::endl;
        // std::cout<<"Moves = "<<moveset[0].size()<<","<<moveset[1].size()<<std::endl;
    // }
    return (evaluation+(moveset[0].size()-moveset[1].size())*w_moves);      

}

