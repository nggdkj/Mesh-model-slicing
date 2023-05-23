#pragma once
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Intersection.h"
#include <vector>
#include <string>
using namespace std;

class ObjLoader {
public:
	ObjLoader(string filename);
	void Draw();
	void slice(int num_of_layers, int layer);
	vector<vector<GLfloat>>vSets;
	vector<vector<GLint>>fSets;
	bool t;
};

