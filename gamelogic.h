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
    {2, {{0, 2}, {4, 7}}},                  //   (needed because this number is not constant across the gameboard).
    {3, {{0, 1}, {5, 8}, {4, 6}}},          // The value in the square # in each pair need to be bitwised-AND'ed together with the indexed square
    {2, {{0, 6}, {4, 5}}},                  //   for the logic to detect a win in that direction.
    {4, {{0, 8}, {2, 6}, {3, 5}, {1, 7}}},  // Example for first row: in Square 0, 3 possible combinations need to be assessed: top horizontal (1,2),
    {2, {{2, 8}, {3, 4}}},                  //   left vertical (3,6) and diagonal (4,8). Bits in each of these combinations can be set to 0 (available)
    {3, {{0, 3}, {2, 4}, {7, 8}}},          //   0b01 (logic), or 0b10 (player), so AND'ing them will yield a potential loss or win.
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

#include "gamelogic.cpp"

#endif // GAMELOGIC_H
