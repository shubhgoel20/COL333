
unsigned long long Engine::piece_Encoding(const Board& b)
{
    unsigned long long ans=0;
    ans = ans|b.data.w_king;    ans=ans<<6;
    ans = ans|b.data.w_bishop;    ans=ans<<6;
    unsigned char val1,val2;
    if(b.data.w_rook_1 > b.data.w_rook_2)
    {
        val1=b.data.w_rook_1;
        val2=b.data.w_rook_2;
    }
    else
    {
        val1=b.data.w_rook_2;
        val2=b.data.w_rook_1;
    }
    ans = ans|val1; ans=ans<<6;
    ans = ans|val2; ans=ans<<6;


    ans = ans|b.data.b_king;    ans=ans<<6;
    ans = ans|b.data.b_bishop;    ans=ans<<6;
    
    if(b.data.b_rook_1 > b.data.b_rook_2)
    {
        val1=b.data.b_rook_1;
        val2=b.data.b_rook_2;
    }
    else
    {
        val1=b.data.b_rook_2;
        val2=b.data.b_rook_1;
    }
    ans = ans|val1; ans=ans<<6;
    ans = ans|val2;

    return ans;
}


unsigned long long Engine::pawn_Encoding(const Board& b)
{
    unsigned char val1,val2,val3,val4;
    val1=b.data.w_pawn_1;
    val2=b.data.w_pawn_2;
    val3=b.data.w_pawn_3;
    val4=b.data.w_pawn_4;
    val1=std::min(val1,val2);   val2=std::max(val1,val2);
    val2=std::min(val2,val3);   val3=std::max(val2,val3);
    val3=std::min(val3,val4);   val4=std::max(val3,val4);

    val1=std::min(val1,val2);   val2=std::max(val1,val2);
    val2=std::min(val2,val3);   val3=std::max(val2,val3);

    val1=std::min(val1,val2);   val2=std::max(val1,val2);

    unsigned long long int ans=0;
    ans=ans|val1;   ans=ans<<6;
    ans=ans|val2;   ans=ans<<6;
    ans=ans|val3;   ans=ans<<6;
    ans=ans|val4;   ans=ans<<6;

    val1=b.data.b_pawn_1;
    val2=b.data.b_pawn_2;
    val3=b.data.b_pawn_3;
    val4=b.data.b_pawn_4;
    val1=std::min(val1,val2);   val2=std::max(val1,val2);
    val2=std::min(val2,val3);   val3=std::max(val2,val3);
    val3=std::min(val3,val4);   val4=std::max(val3,val4);

    val1=std::min(val1,val2);   val2=std::max(val1,val2);
    val2=std::min(val2,val3);   val3=std::max(val2,val3);

    val1=std::min(val1,val2);   val2=std::max(val1,val2);

    
    ans=ans|val1;   ans=ans<<6;
    ans=ans|val2;   ans=ans<<6;
    ans=ans|val3;   ans=ans<<6;
    ans=ans|val4;   ans=ans<<6;



    return ans;
}


unsigned long long knightRookEncoding(const Board& b)
{
    unsigned long long int ans=0;

    unsigned char val1,val2;


    if(b.data.w_rook_1 > b.data.w_rook_2)
    {
        val1=b.data.w_rook_1;
        val2=b.data.w_rook_2;
    }
    else
    {
        val1=b.data.w_rook_2;
        val2=b.data.w_rook_1;
    }   
    ans = ans|val1; ans=ans<<6;
    ans = ans|val2; ans=ans<<6;



    if(b.data.b_rook_1 > b.data.b_rook_2)
    {
        val1=b.data.b_rook_1;
        val2=b.data.b_rook_2;
    }
    else
    {
        val1=b.data.b_rook_2;
        val2=b.data.b_rook_1;
    }   
    ans = ans|val1; ans=ans<<6;
    ans = ans|val2; ans=ans<<6;




    if(b.data.w_knight_1 > b.data.w_knight_2)
    {
        val1=b.data.w_knight_1;
        val2=b.data.w_knight_2;
    }
    else
    {
        val1=b.data.w_knight_2;
        val2=b.data.w_knight_1;
    }   
    ans = ans|val1; ans=ans<<6;
    ans = ans|val2; ans=ans<<6;




    if(b.data.b_knight_1 > b.data.b_knight_2)
    {
        val1=b.data.b_knight_1;
        val2=b.data.b_knight_2;
    }
    else
    {
        val1=b.data.b_knight_2;
        val2=b.data.b_knight_1;
    }   
    ans = ans|val1; ans=ans<<6;
    ans = ans|val2;

    return ans;
}


unsigned long long bishopKingEncoding(const Board& b)
{
    unsigned long long int ans=0;
    ans = ans|b.data.w_king;    ans=ans<<6;
    ans = ans|b.data.w_bishop;  ans=ans<<6;
    
    ans = ans|b.data.b_king;    ans=ans<<6;
    ans = ans|b.data.b_bishop;  ans;
    return ans;
}


void Engine::printTableSize()
{
    std::cout<<"Num of table entries = "<<moveTableNew.size()<<std::endl;
    // std::cout<<"Num of pawn encodings = "<<moveTable.size()<<std::endl;
    // int count=0;
    // for(auto p:moveTable)
    // {
    //     count+=p.second.size();
    //     // std::cout<<"Num of piece encoding = "<<p.second.size()<<std::endl;
    // }
    // std::cout<<"Total num of entries = "<<count<<std::endl;
}