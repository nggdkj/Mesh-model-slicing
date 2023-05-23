#pragma once
#include <iostream>
#include <vector>
#include <math.h>
#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
using namespace std;

class Intersection
{
public:
    vector<GLfloat> v0;
    vector<GLfloat> v1;
    vector<GLfloat> v2;
    Intersection(vector<GLfloat> v_0, vector<GLfloat> v_1, vector<GLfloat> v_2);
};

bool pan(vector<GLfloat> start, vector<GLfloat> end, Intersection tri, vector<GLfloat>& intersection);
