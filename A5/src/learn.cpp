#include <iostream>
#include "butils.hpp"
#include "engine_old.hpp"

void one_match(Board& b,Engine& player1,Engine& player2,int num_games)//p1 is learner, p2 is adversary
{
    std::cout<<board_to_str(&b.data)<<std::endl;
    auto moveset = b.get_legal_moves();
    int count_moves=200;
    while(!moveset.empty() && count_moves>0)
    {
        count_moves--;
        if(b.data.player_to_play==WHITE)
        {
            player1.find_best_move(b);
            b.do_move_(player1.best_move);

        }
        else
        {
            player2.find_best_move(b);
            b.do_move_(player2.best_move);
            player1.update_weights(b,player2.best_move_cost,num_games);
        }
        moveset = b.get_legal_moves();
        std::cout<<board_to_str(&b.data)<<std::endl;
        int temp;
        std::cin>>temp;
    }

    std::cout<<"Game "<<num_games<<".1 finished,   Result = ";

    
    if(b.get_legal_moves().size()==0)
    {
        if(b.in_check() && b.data.player_to_play==WHITE)
        {
            std::cout<<"Player 2 won";
        }
        else if(b.in_check())
            std::cout<<"Player 1 won";
        else
            std::cout<<"stalemate";
    }
    else
        std::cout<<"Draw";

    std::cout<<std::endl;

    // Board b2;
    // std::cout<<board_to_str(&b2.data)<<std::endl;
    // moveset=b2.get_legal_moves();
    // count_moves=200;
    // while(!moveset.empty() && count_moves>0)
    // {
    //     count_moves--;
    //     if(b2.data.player_to_play==BLACK)
    //     {
    //         player1.find_best_move(b2);
    //         b2.do_move_(player1.best_move);

    //     }
    //     else
    //     {
    //         player2.find_best_move(b2);
    //         b2.do_move_(player2.best_move);
    //         player1.update_weights(b2,player2.best_move_cost,num_games);
    //     }
    //     moveset = b2.get_legal_moves();
    // }
    // std::cout<<"Game "<<num_games<<".2 finished,   Result = ";

    // if(b2.get_legal_moves().size()==0)
    // {
    //     if(b2.in_check() && b2.data.player_to_play==WHITE)
    //     {
    //         std::cout<<"Player 2 won";
    //     }
    //     else if(b2.in_check())
    //         std::cout<<"Player 1 won";
    //     else
    //         std::cout<<"stalemate";
    // }
    // else
    //     std::cout<<"Draw";

    // std::cout<<std::endl;

}

int main()
{
    Engine player1, player2;
    for(int x=0;x<1;x++)
    {
        Board* b = new Board();
        one_match(*b,player1,player2,x+1);
        delete b;
    }
    player1.print_weights();
    // player1.learn_weights=false;
    // player2.learn_weights=false;
    // for(int x=0;x<20;x++)
    // {
    //     one_match(player1,player2,x+1);
    // }
    // std::cout<<"Learned Weights"<<std::endl;
    // player1.print_weights();
    return 0;
}