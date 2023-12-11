#include <iostream>
// #include "board.hpp"
// #include "bdata.hpp"
// #include "butils.hpp"
// #include "engine.hpp"
using namespace std;
// void simulate_game(const Board b_temp, Engine& player1, Engine& player2, PlayerColor color)
// {
//     Board b(b_temp);
//     auto moveset = b.get_legal_moves();
//     int count=100;
//     while(!moveset.empty() && count>0)
//     {
//         count--;
//         if(b.data.player_to_play==color)
//         {
//             player1.find_best_move(b);
//             b.do_move_(player1.best_move);
//         }
//         else
//         {
//             player2.find_best_move(b);
//             b.do_move_(player2.best_move);
//         }
//         moveset=b.get_legal_moves();
//         std::cout<<board_to_str(&b.data)<<std::endl;
//     }

// }

// int main()
// {
//     Engine player1,player2;
//     Board b;
//     for(int x=0;x<20;x++)
//     {
        
//         std::cout<<board_to_str(&b.data)<<std::endl;
//         // simulate_game(b,player1,player2,WHITE);
//         std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
        
//     }
//     return 0;
// }

#define pos(x,y) (((y)<<3)|(x))

int main()
{
    float array[8][8] = 
    {
        {0, 0, 0.5, 0, 1, 0, 0, 0},
        {0, 0.75, 0, 1, 0, 0.75, 0, 0},
        {1, 0, 0, 0, 0, 0, 0.5, 0},
        {0, 1, 0, 0, 0, 0, 0, 0.25},
        {0.25, 0, 0, 0, 0, 0, 1, 0},
        {0, 0.5, 0, 0, 0, 0, 0, 1},
        {0, 0, 0.75, 1, 0, 0, 0.75, 0},
        {0, 0, 0, 1, 0, 0.5, 0, 0},
    };

    float new_array[64];
    for(int x=0;x<8;x++)
    {
        for(int y=0;y<8;y++)
        {
            new_array[pos(x,y)]=array[7-y][x];
        }
    }

    cout<<"White"<<endl;
    for(int x=0;x<64;x++)
    {        
        cout<<new_array[x]<<",";
    }

    for(int x=0;x<4;x++)
    {
        for(int y=0;y<8;y++)
        {
            float temp=array[x][y];
            array[x][y]=array[7-x][y];
            array[7-x][y]=temp;
        }
    }

    for(int y=0;y<4;y++)
    {
        for(int x=0;x<8;x++)
        {
            float temp=array[x][y];
            array[x][y]=array[x][7-y];
            array[x][7-y]=temp;
        }
    }

    cout<<"Flipped array"<<endl;
    for(int x=0;x<8;x++)
    {
        for(int y=0;y<8;y++)
        {
            cout<<array[x][y]<<" ";
        }
        cout<<endl;
    }

    for(int x=0;x<8;x++)
    {
        for(int y=0;y<8;y++)
        {
            new_array[pos(x,y)]=array[7-y][x];
        }
    }

    cout<<"Black"<<endl;
    for(int x=0;x<64;x++)
    {        
        cout<<new_array[x]<<",";
        // if(x%8==7)
        //     cout<<endl;
    }
}
