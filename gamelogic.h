#ifndef GAMELOGIC_H
#define GAMELOGIC_H

/* Logic is based on the following tic-tac-toe square numbering convention:
 *
 *    0  |  1  |  2
 * --------------------
 *    3  |  4  |  5
 * --------------------
 *    6  |  7  |  8
 *
 * **NOTE**: This convention is zero-based whereas the numbering in the interface object names is 1-based.
 * The interface object name convention is actually arbitrary, as UI objects are tied to the game logic
 * through the Square<> class template.
 *
 * */

#include <stdlib.h>
#include <algorithm>

#define GT_SQ_STATE_EMPTY  0
#define GT_SQ_STATE_LOGIC  0b01
#define GT_SQ_STATE_PLAYER 0b10

#define GT_GAME_STATE_PLAY          0b00
#define GT_GAME_STATE_NEWGAME       0b100
#define GT_GAME_STATE_PLAYER_WINS   0b10
#define GT_GAME_STATE_LOGIC_WINS    0b01
#define GT_GAME_STATE_TIE           0b1000
#define GT_GAME_STATE_ERROR         0b10000

#define GT_GRID_SIZE    9

#define GT_TEMPLATE template<class UIC, class Owner>
#define GT_LOGIC_CLASS GameLogic<UIC, Owner>

typedef unsigned short  GT_SQ_STATE_T;
typedef unsigned int    GT_GAME_STATE_T;
typedef unsigned short  GT_SQUARE_NO;

#include "square.h"

const struct PlayLogic {
    size_t       numvecs;
    GT_SQUARE_NO vecs[4][2];
} playlogic[GT_GRID_SIZE] = {               // Game logic indexes into this array using the square # of which the score is to be calculated.
    {3, {{1, 2}, {3, 6}, {4, 8}}},          // The numvecs specifies the number of possible wins/losses to be assessed per square
    {2, {{0, 2}, {4, 7}}},                  //  (needed because this number is not constant across the gameboard).
    {3, {{0, 1}, {5, 8}, {4, 6}}},          // The Square #'s of each pair need to be bitwised-AND'ed together with the indexed square
    {2, {{0, 6}, {4, 5}}},                  //   for the logic to detect a win in that direction.
    {4, {{0, 8}, {2, 6}, {3, 5}, {1, 7}}},
    {2, {{2, 8}, {3, 4}}},
    {3, {{0, 3}, {2, 4}, {7, 8}}},
    {2, {{1, 4}, {6, 8}}},
    {3, {{0, 4}, {2, 5}, {6, 7}}}
};

const double favoured_score[9] = {0.4, 0.25, 0.4, 0.25, 0.5, 0.25, 0.4, 0.25, 0.4}; // Favour corners and especially centre square if given the option.

GT_TEMPLATE class GameLogic
{
    GT_SQUARE_CLASS*        grid[GT_GRID_SIZE] = {};
    bool                    mPlayerFirst = true;
    GT_GAME_STATE_T         mGameState = GT_GAME_STATE_PLAY;
    void                    Play();
    GT_GAME_STATE_T         GetGameState(); // Determines if player or logic has won the game.
    GT_SQUARE_CLASS*        GetSquareWithMaxScore(double&); // Obtains the highest score from all squares.

public:
                            GameLogic<UIC, Owner>();
                            GameLogic<UIC, Owner>(GT_LOGIC_CLASS&) = delete;
    GT_LOGIC_CLASS          operator=(GT_LOGIC_CLASS&) = delete;
    virtual                 ~GameLogic();
    int                     SetSquare(GT_SQUARE_CLASS*); // Called by the square's creator to set its instance in the grid.
    GT_SQ_STATE_T           getSquareState(GT_SQUARE_NO); // May not be needed. Consider deleting.
    void                    SetPlayerFirstFlag(bool);
    GT_GAME_STATE_T         sigSquareClicked(GT_SQUARE_NO); // Called by the interface when a square is clicked.
    void                    redrawRequest(void); // Called by interface when redrawing all squares.
    void                    NewGame();

};


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
    auto predictOutcome = [&](GT_SQUARE_NO sqno) -> GT_GAME_STATE_T {
        const PlayLogic* pl = &playlogic[sqno];
        bool cumultest_win = false;
        bool cumultest_lose = false;
        for (size_t i = 0; i < pl->numvecs; i++)
        {
            //GT_SQUARE_NO [s1, s2] = std::tie(pl->vecs[i]); // Find where to put C++17 Flag in bat files to enable structured bindings.
            GT_SQUARE_NO s1 = pl->vecs[i][0];
            GT_SQUARE_NO s2 = pl->vecs[i][1];
            cumultest_win = cumultest_win || ((grid[s1]->GetState() & GT_SQ_STATE_LOGIC) &&
                                              (grid[s2]->GetState() & GT_SQ_STATE_LOGIC));
            cumultest_lose = cumultest_lose || ((grid[s1]->GetState() & GT_SQ_STATE_PLAYER) &&
                                                (grid[s2]->GetState() & GT_SQ_STATE_PLAYER));
        }

        return cumultest_win ? GT_GAME_STATE_LOGIC_WINS :
                    cumultest_lose ? GT_GAME_STATE_PLAYER_WINS : GT_GAME_STATE_PLAY;
    };

    for (Square<UIC, Owner>*& rsq : grid)
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
                        rsq->SetScore(0.999); // Only superseded by the option to win the game (=1.000).
                        break;
                    case GT_GAME_STATE_LOGIC_WINS:
                        rsq->SetScore(1.000);
                        break;
            }
        }
    }

    double dMaxScore = 0.0;
    Square<UIC, Owner>* elected_square = GetSquareWithMaxScore(dMaxScore);

    if (elected_square)
    {
        cout << "Elected #" << elected_square->GetSquareNo() << " scores= ";
        for (int i = 0; i < 9; i++)
                cout << i << ":" << grid[i]->GetScore() << "  ";
        cout << endl;
        elected_square->SetState(GT_SQ_STATE_LOGIC); // This square is mine!
    } else
        cout << "Elected square is NULL" << endl;

}

#endif // GAMELOGIC_H
