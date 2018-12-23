#ifndef SQUARE_H
#define SQUARE_H

#define GT_SQUARE_CLASS Square<UIC, Owner>

template<class UI, class Owner> class GameLogic;

template<class UIC, class Owner>
class Square
{
    GT_SQUARE_NO            mSquareNo;
    GT_SQ_STATE_T           mState;
    UIC                     *mUiElement;
    Owner*                  pOwner;
    double                  mdScore = 0.0;

public:
    explicit            GT_SQUARE_CLASS(GT_SQUARE_NO sqno, Owner* po, UIC* uie);
                        GT_SQUARE_CLASS(GT_LOGIC_CLASS&) = delete;
    GT_SQUARE_CLASS&    operator=(GT_SQUARE_CLASS&) = delete;
    void                SetState(GT_SQ_STATE_T); // GameLogic tells square to set its state and get UI to update through owner's public member call.
    void                ShowState(void); // GameLogic tells square to trigger UI update for square (redraw).
    GT_SQUARE_NO        GetSquareNo() {return mSquareNo;}
    double              GetScore() { return mdScore; }
    void                SetScore(double d) {mdScore = d;}
    GT_SQ_STATE_T       GetState() {return mState;}

};

#include "square.cpp"

#endif // SQUARE_H
