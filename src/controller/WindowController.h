//
// Created by ofaru on 16.03.2024.
//

#ifndef WINDOWCONTROLLER_H
#define WINDOWCONTROLLER_H


#include <vector>
#include <GLFW/glfw3.h>

class WindowController {

private:
    GLFWwindow *activeWindow;
    std::vector<GLFWwindow*> windows;

    int width;
    int height;

    static WindowController* instance;

public:

    static WindowController* getInstance() {
        if (instance == nullptr) {
            instance = new WindowController();
        }
        return instance;
    };

    void createFullscreenWindow(const char *title);

    void createWindow(const char *title, int width, int height);

    GLFWwindow *getActiveWindow() const;

    const std::vector<GLFWwindow *> &getWindows() const;

    int getWidth() const;

    int getHeight() const;

    void setWidth(int width);

    void setHeight(int height);

    void setActiveWindow(GLFWwindow *activeWindow);

};


#endif //WINDOWCONTROLLER_H
