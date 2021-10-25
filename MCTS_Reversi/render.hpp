#ifndef RENDER_H
#define RENDER_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ResourceManager.h"
#include "game.hpp"

extern GLFWwindow* window;

void init_GL();
void render(const Game &status); //渲染并返回指令
void framebuffer_size_callback(GLFWwindow* window, int width, int height);       // 窗口大小改变时回调该函数
void mouse_callback(GLFWwindow *window, double xpos, double ypos);               // 光标移动回调
void mouse_button_callback(GLFWwindow *window, int button, int act, int mods);   //鼠标按下回调
#endif