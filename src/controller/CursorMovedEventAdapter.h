//
// Created by ofarukdogancay on 5/20/24.
//

#ifndef CURSORMOVEDEVENTADAPTER_H
#define CURSORMOVEDEVENTADAPTER_H

#include <functional>

class CursorMovedEventAdapter {
public:

    // Constructor with 3 functions

    std::function<void(double, double, double, double)> cursorMovedFunction;

    CursorMovedEventAdapter(const std::function<void(double, double, double, double)> &cursorMovedFunction)
            : cursorMovedFunction(
            cursorMovedFunction) {

    }

    void cursorMoved(double xpos, double ypos, double xoffset, double yoffset) {
        cursorMovedFunction(xpos, ypos, xoffset, yoffset);
    }


};

#endif //CURSORMOVEDEVENTADAPTER_H
