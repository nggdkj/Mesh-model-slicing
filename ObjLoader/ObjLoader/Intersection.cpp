#include "Intersection.h"
using namespace std;

Intersection::Intersection(vector<GLfloat> v_0, vector<GLfloat> v_1, vector<GLfloat> v_2) {
    v0.resize(3);
    v1.resize(3);
    v2.resize(3);
    for (int i = 0; i < 3; i++) {
        v0[i] = v_0[i];
        v1[i] = v_1[i];
        v2[i] = v_2[i];
    }
}

GLfloat vector_dot(vector<GLfloat> v0, vector<GLfloat> v1)  //向量点乘
{
    return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

void vector_minus(vector<GLfloat> a, vector<GLfloat> b, vector<GLfloat>& res)  //向量减法
{
    res.resize(3);
    res[0] = a[0] - b[0];
    res[1] = a[1] - b[1];
    res[2] = a[2] - b[2];
}

void vector_cross(vector<GLfloat> a, vector<GLfloat> b, vector<GLfloat>& res)  //向量叉乘
{
    res.resize(3);
    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

//判断线段start_end与三角面片surface有无交点，有则存入intersection
bool pan(vector<GLfloat> start, vector<GLfloat> end, Intersection surface, vector<GLfloat>& intersection)
{
    const float epsilon = 0.000001f;
    vector<GLfloat> e1, e2, p, s, q;
    float x, y, z, tmp;
    vector<GLfloat> direction;
    vector_minus(end, start, direction);
    vector_minus(surface.v1, surface.v0, e1);
    vector_minus(surface.v2, surface.v0, e2);
    vector_cross(direction, e2, p);
    tmp = vector_dot(p, e1);
    if (tmp > -epsilon && tmp < epsilon)
        return false;
    tmp = 1.0f / tmp;
    vector_minus(start, surface.v0, s);
    y = tmp * vector_dot(s, p);
    if (y < 0.0 || y > 1.0)
        return false;
    vector_cross(s, e1, q);
    z = tmp * vector_dot(direction, q);
    if (z < 0.0 || z > 1.0)
        return false;
    if (y + z > 1.0)
        return false;
    x = tmp * vector_dot(e2, q);
    if (x < 0.0 || x > 1.0)
        return false;
    intersection.resize(3);
    intersection[0] = start[0] + x * direction[0];  //保存交点坐标
    intersection[1] = start[1] + x * direction[1];
    intersection[2] = start[2] + x * direction[2];
    return true;
}