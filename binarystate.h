#ifndef BINARYSTATE_H
#define BINARYSTATE_H

#include <map>

using namespace std;

#define ML_VAR_TYPE unsigned short
#define BINARY_STATE_T BinaryState<StateType, N,V,val_to_play>

/* state_vector = unsigned integer type that will hold all bitwise variables concatenated in one state.
 * N            = number of variables
 * V            = number of bitwise values in each variable. One bit per value, with "1"
 * val_to_play  = the bit value in each variable (e.g. 010) that this logic algorithm can play.
 *
 * The Binary State class supports N/V variables. Likely to be either unsigned long, or unsigned long long
 */

template<typename StateType, int N, unsigned short V,
         ML_VAR_TYPE val_to_play>
class BinaryState {
    StateType                           state_vector;
    unsigned short                      winlossflag;
    map<StateType, BinaryState*>        subsequent;

public:
    BinaryState<N,V,val_to_play>();

    BINARY_STATE_T& BINARY_STATE_T(BINARY_STATE_T& that) :
        state_vector(that.state_vector), winlossflag(that.winlossflag), subsequent(that.subsequent) {}
};

#endif // BINARYSTATE_H
