#include "utility.h"

// Read encoder values
intPair readEncoders() {
    
    intPair readClicks{};

    readClicks.int1 = encoders.getCountsAndResetLeft();
    readClicks.int2 = encoders.getCountsAndResetRight();

    return readClicks;
}