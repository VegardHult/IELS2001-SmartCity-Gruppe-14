#include "utility.h"

intPair readEncoders() {
    
    intPair readClicks{};

    readClicks.int1 = encoders.getCountsAndResetLeft();
    readClicks.int2 = encoders.getCountsAndResetRight();

    return readClicks;
}