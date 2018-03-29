#include "gamelogic.h"

template <class UIC, class Owner>
GameLogic<UIC, Owner>::GameLogic()
{

}

template <class UIC, class Owner>
int GameLogic<UIC, Owner>::SetSquare(Square<UIC, Owner>* sq)
{
    grid[sq->GetSquareNo()] = sq;
}

template <class UIC, class Owner>
GT_STATE_T  GameLogic<UIC, Owner>::getState(GT_SQUARE_NO sqno)
{
    return grid[sqno].GetState();
}

template <class UIC, class Owner>
void GameLogic<UIC, Owner>::sigSquareClicked(GT_SQUARE_NO sqno)
{
    Square& rsq = *(grid[sqno]);
    if (rsq.GetState() == GT_STATE_EMPTY)
        rsq.SetState(GT_STATE_PLAYER);
}



