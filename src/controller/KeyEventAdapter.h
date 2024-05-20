//
// Created by ofarukdogancay on 5/20/24.
//

#ifndef COMP410_PROJECT_KEYEVENTADAPTER_H
#define COMP410_PROJECT_KEYEVENTADAPTER_H


class KeyEventAdapter {
public:

    // Constructor with 3 functions

    std::function<void(int)> keyPressedFunction;
    std::function<void(int)> keyReleasedFunction;
    std::function<void(int)> keyHeldFunction;

    KeyEventAdapter(const std::function<void(int)> &keyPressedFunction,
                    const std::function<void(int)> &keyReleasedFunction,
                    const std::function<void(int)> &keyHeldFunction) : keyPressedFunction(keyPressedFunction),
                                                                       keyReleasedFunction(keyReleasedFunction),
                                                                       keyHeldFunction(keyHeldFunction) {

    }

    void keyPressed(int key) {
        keyPressedFunction(key);
    }

    void keyReleased(int key) {
        keyReleasedFunction(key);
    }

    void keyHeld(int key) {
        keyHeldFunction(key);
    }
};

#endif //COMP410_PROJECT_KEYEVENTADAPTER_H
