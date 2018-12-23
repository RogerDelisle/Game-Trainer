
GT_TEMPLATE GT_LOGIC_CLASS::GameLogic()
{
}

GT_TEMPLATE GT_LOGIC_CLASS::~GameLogic()
{
    for (GT_SQUARE_CLASS*& rsq : grid)
        if (rsq)
            delete rsq;
}

GT_TEMPLATE int GT_LOGIC_CLASS::SetSquare(GT_SQUARE_CLASS* sq)
{
    int retsqno = sq->GetSquareNo();
    grid[sq->GetSquareNo()] = sq;
    return retsqno;
}

GT_TEMPLATE GT_SQ_STATE_T  GT_LOGIC_CLASS::getSquareState(GT_SQUARE_NO sqno)
{
    return grid[sqno].GetState();
}

GT_TEMPLATE GT_GAME_STATE_T GT_LOGIC_CLASS::sigSquareClicked(GT_SQUARE_NO sqno)
{
    if (mGameState != GT_GAME_STATE_PLAY)
        return mGameState;

    GT_SQUARE_CLASS& rsq = *(grid[sqno]);

    if (rsq.GetState() == GT_SQ_STATE_EMPTY)
    {
        rsq.SetState(GT_SQ_STATE_PLAYER);
        if ((mGameState = GetGameState()) == GT_GAME_STATE_PLAY)
        {
            Play();
            mGameState = GetGameState(); // Check game state again to see if logic's play made it win.
        }
    }
    return mGameState;
}

GT_TEMPLATE void GT_LOGIC_CLASS::redrawRequest(void)
{
    for (GT_SQUARE_CLASS*& rsq : grid)
        rsq->ShowState();
}

GT_TEMPLATE GT_SQUARE_CLASS* GT_LOGIC_CLASS::GetSquareWithMaxScore(double& maxscore)
{
    double d;
    maxscore = 0.0;
    Square<UIC, Owner>* sqwmax = nullptr;
    for (GT_SQUARE_CLASS*& rsq : grid)
        if (rsq->GetState() == GT_SQ_STATE_EMPTY &&
                (d = rsq->GetScore()) > maxscore)
        {
            maxscore = d;
            sqwmax = rsq;
        }
    return sqwmax;
}

GT_TEMPLATE void GT_LOGIC_CLASS::SetPlayerFirstFlag(bool bF)
{
    mPlayerFirst = bF;
    NewGame();
}

GT_TEMPLATE void GT_LOGIC_CLASS::NewGame()
{
    mGameState = GT_GAME_STATE_PLAY;
    for (GT_SQUARE_CLASS*& rsq : grid)
    {
        rsq->SetState(GT_SQ_STATE_EMPTY);
        rsq->SetScore(0.0);
    }
    if (!mPlayerFirst)
        Play();
}

GT_TEMPLATE GT_GAME_STATE_T GT_LOGIC_CLASS::GetGameState()
{
    bool isGameOver = true;
    // For a supplied row of squares, test if the specified player has won
    auto&& linetest = [this, &isGameOver] (GT_SQUARE_NO sqno1, GT_SQUARE_NO sqno2,
                        GT_SQUARE_NO sqno3, GT_SQ_STATE_T playermask) -> bool {
        GT_GAME_STATE_T sq1 = grid[sqno1]->GetState(), sq2 = grid[sqno2]->GetState(),
                sq3 = grid[sqno3]->GetState();
        isGameOver = isGameOver && !((sq1 == GT_SQ_STATE_EMPTY) || (sq2 == GT_SQ_STATE_EMPTY) || (sq3 == GT_SQ_STATE_EMPTY));
        return ((sq1 & playermask) &&
                (sq2 & playermask) &&
                (sq3 & playermask));
    };

    // Test all possible win/loss combinations for the specified player.
    auto&& istherewin = [&linetest] (GT_SQ_STATE_T playermask) -> bool {
        return (linetest(0,1,2, playermask) ||
                linetest(3,4,5, playermask) ||
                linetest(6,7,8, playermask) ||
                linetest(0,3,6, playermask) ||
                linetest(1,4,7, playermask) ||
                linetest(2,5,8, playermask) ||
                linetest(0,4,8, playermask) ||
                linetest(2,4,6, playermask));
    };

    // Test if the player or the logic has won.
    return(istherewin(GT_SQ_STATE_PLAYER) ? GT_GAME_STATE_PLAYER_WINS :
           istherewin(GT_SQ_STATE_LOGIC) ? GT_GAME_STATE_LOGIC_WINS : isGameOver ? GT_GAME_STATE_TIE : GT_GAME_STATE_PLAY);
}

GT_TEMPLATE void GT_LOGIC_CLASS::Play()
{
    const auto&& predictOutcome = [&](GT_SQUARE_NO sqno) -> GT_GAME_STATE_T {
        const PlayLogic* pl = &playlogic[sqno];
        bool cumultest_win = false;
        bool cumultest_lose = false;
        for (size_t i = 0; i < pl->numvecs; i++) // For each pair of squares to be tested for square # sqno...
        {
            //GT_SQUARE_NO [s1, s2] = std::tie(pl->vecs[i]); // Find where to put C++17 Flag in bat files to enable structured bindings.
            GT_SQUARE_NO s1 = pl->vecs[i][0]; // Retrieve squares to be tested.
            GT_SQUARE_NO s2 = pl->vecs[i][1];
            cumultest_win = cumultest_win || ((grid[s1]->GetState() & GT_SQ_STATE_LOGIC) &&
                                              (grid[s2]->GetState() & GT_SQ_STATE_LOGIC)); // Does the logic have squares in these 2 places?
            cumultest_lose = cumultest_lose || ((grid[s1]->GetState() & GT_SQ_STATE_PLAYER) &&
                                                (grid[s2]->GetState() & GT_SQ_STATE_PLAYER)); // Does the player have squares in these 2 places?
        }

        return cumultest_win ? GT_GAME_STATE_LOGIC_WINS :
                    cumultest_lose ? GT_GAME_STATE_PLAYER_WINS : GT_GAME_STATE_PLAY;
    };

    for (Square<UIC, Owner>*& rsq : grid) // Assign a priority score to each square in which logic can play.
    {
        if (rsq->GetState() == GT_SQ_STATE_EMPTY)
        {
            GT_SQUARE_NO sqno = rsq->GetSquareNo();
            switch (predictOutcome(sqno)) {
                    case GT_GAME_STATE_PLAY:
                        double r; // important to cast the random # into a double: divinding 2 int's always gives 0
                        rsq->SetScore(favoured_score[sqno] + (r = rand()) / RAND_MAX *0.298);
                        break;
                    case GT_GAME_STATE_PLAYER_WINS:
                        rsq->SetScore(0.999); // Should play here to avoid a loss. Only superseded by the option to win the game (=1.000).
                        break;
                    case GT_GAME_STATE_LOGIC_WINS:
                        rsq->SetScore(1.000); // Should play here to WIN! This is the most desirable option
                        break;
            }
        }
    }

    double dMaxScore = 0.0;
    Square<UIC, Owner>* elected_square = GetSquareWithMaxScore(dMaxScore); // Of all squares scored, which is highest?

    if (elected_square)
    {
        cout << "Elected #" << elected_square->GetSquareNo() << " scores= ";
        for (int i = 0; i < 9; i++)
                cout << i << ":" << grid[i]->GetScore() << "  ";
        cout << endl;
        elected_square->SetState(GT_SQ_STATE_LOGIC); // This square is mine!
    } else
        cout << "Elected square is NULL" << endl; // This should never happen.
}
