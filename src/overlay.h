#ifndef OVERLAY_H
#define OVERLAY_H

#include <GLFW/glfw3.h>

class Overlay
{
public:
    Overlay();
    ~Overlay();

    void Init(GLFWwindow *window);
    void Render();
};

#endif