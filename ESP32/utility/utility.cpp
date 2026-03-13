#include "utility.h"

// Read encoder values
intPair readEncoders() {
    
    intPair readClicks{};

    readClicks.int1 = encoders.getCountsAndResetLeft();
    readClicks.int2 = encoders.getCountsAndResetRight();

    return readClicks;
}

// Get string from action
String actionToString(actions action) {
    if (action == S) {
        return "S";
    } else if (action == T) {
        return "T";
    } else if (action == L) {
        return "L";
    } else if (action == R) {
        return "R";
    } else {
        return "I";
    }
}

String modeToString(modes mode) {
    if (mode == E) {
        return "E";
    } else if (mode == C) {
        return "C";
    } else {
        return "D";
    }
}

actions stringToAction(String str) {
    if (str == "S") {
        return actions::S;
    } else if (str == "T") {
        return actions::T;
    } else if (str == "L") {
        return actions::L;
    } else if (str == "R") {
        return actions::R;
    } else {
        return actions::I;
    }
}

modes stringToMode(String str) {
    if (str == "E") {
        return modes::E;
    } else if (str == "C") {
        return modes::C;
    } else {
        return modes::D;
    }
}

wireDataStruct parseWire(String recievedData) {
    actions recievedAction = stringToAction(recievedData.substring(0, 1));
    modes recievedMode = stringToMode(recievedData.substring(1, 2));
    wireDataStruct data;
    data.action = recievedAction;
    data.mode = recievedMode;
    return data;
}