//
// Created by ofarukdogancay on 5/20/24.
//

#ifndef COMP410_PROJECT_CURSORMOVEDEVENTADAPTER_H
#define COMP410_PROJECT_CURSORMOVEDEVENTADAPTER_H

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

#endif //COMP410_PROJECT_CURSORMOVEDEVENTADAPTER_H
