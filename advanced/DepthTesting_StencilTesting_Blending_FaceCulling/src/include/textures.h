#pragma once
#ifndef LOAD_TEXTURE_H
#define LOAD_TEXTURE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <string>
#include <iostream>


unsigned int loadTexture(const char* texture_path);

#endif