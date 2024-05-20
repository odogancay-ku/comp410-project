//
// Created by ofarukdogancay on 5/20/24.
//

#ifndef COMP410_PROJECT_MOUSEBUTTONEVENTADAPTER_H
#define COMP410_PROJECT_MOUSEBUTTONEVENTADAPTER_H

#include <functional>

class MouseButtonEventAdapter {
public:

    // Constructor with 3 functions

    std::function<void(int, double, double)> mouseButtonPressedFunction;
    std::function<void(int, double, double)> mouseButtonReleasedFunction;

    MouseButtonEventAdapter(const std::function<void(int, double, double)> &mouseButtonPressedFunction,
                            const std::function<void(int, double, double)> &mouseButtonReleasedFunction)
            : mouseButtonPressedFunction(
            mouseButtonPressedFunction),
              mouseButtonReleasedFunction(
                      mouseButtonReleasedFunction) {

    }

    void mouseButtonPressed(int button, double xpos, double ypos) {
        mouseButtonPressedFunction(button, xpos, ypos);
    }

    void mouseButtonReleased(int button, double xpos, double ypos) {
        mouseButtonReleasedFunction(button, xpos, ypos);
    }

};

#endif //COMP410_PROJECT_MOUSEBUTTONEVENTADAPTER_H
