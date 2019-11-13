#pragma once

#include "GLCommon.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void error_callback(int error, const char* description);

// Mouse callbacks
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_enter_callback(GLFWwindow* window, int entered);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// Joystick...
//void joystick_callback(int joy, int event);