#include "gamelogic.h"
#include "square.h"

template<class UIC, class Owner>
Square<UIC, Owner>::Square(GT_SQUARE_NO sqno, Owner* po, UIC* uie, GameLogic<UIC, Owner>* inLogic) :
    mSquareNo(sqno), pOwner(po), mUiElement(uie), mrLogic(inLogic)
{
}

template<class UIC, class Owner>
void Square<UIC, Owner>::SetState(GT_STATE_T st)
{
    mState = st;
    pOwner->SetUIState(st, mUiElement);
}

template<class UIC, class Owner>
GT_SQUARE_NO Square<UIC, Owner>::GetSquareNo()
{
    return mSquareNo;
}

template<class UIC, class Owner>
GT_STATE_T Square<UIC, Owner>::GetState()
{
    return mState;
}

