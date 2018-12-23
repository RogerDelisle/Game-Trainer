// This file is included with its header file: square.h
// The cpp file should not be an independently-compiled part of the project due to the use of templates.
// It should only be included at the bottom of square.h and it should only contain member-function templates

template<class UIC, class Owner>
GT_SQUARE_CLASS::Square(GT_SQUARE_NO sqno, Owner* po, UIC* uie) :
    mSquareNo(sqno), mState(GT_SQ_STATE_EMPTY), pOwner(po), mUiElement(uie)
{
}

template<class UIC, class Owner>
void GT_SQUARE_CLASS::SetState(GT_SQ_STATE_T st)
{
    mState = st;
    pOwner->SetUIState(st, mUiElement);
}

template<class UIC, class Owner>
void GT_SQUARE_CLASS::ShowState(void)
{
    pOwner->SetUIState(mState, mUiElement);
}
