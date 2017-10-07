#pragma once

#ifndef TEXTURE_LOADER
#define TEXTURE_LOADER

#include <glad/glad.h> 

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

static class TextureLoader
{
public:
	// utility function for loading a 2D texture from file
	static unsigned int Load(const char *path, const string &directory = string(), bool gamma = false);
		
private:

};


#endif
