//
// Created by ofaru on 16.03.2024.
//

#ifndef HOMEWORK1_WINDOWCONTROLLER_H
#define HOMEWORK1_WINDOWCONTROLLER_H


#include <vector>
#include <GLFW/glfw3.h>

class WindowController {

private:
    GLFWwindow *activeWindow;
    std::vector<GLFWwindow*> windows;

    int width;
    int height;

public:

    static WindowController& getInstance() {
        static WindowController instance;
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


#endif //HOMEWORK1_WINDOWCONTROLLER_H
