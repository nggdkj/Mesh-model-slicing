#include "ObjLoader.h"
#include <fstream>
#include <iostream>
using namespace std;

float x_max = -10000;
float x_min = 10000;

float y_max = -10000;
float y_min = 10000;

float z_max = -10000;
float z_min = 10000;

ObjLoader::ObjLoader(string filename)  //读入obj文件
{
	string line;
	fstream f;
	t = false;
	f.open(filename, ios::in);
	while (!f.eof()) {
		getline(f, line);
		vector<string>parameters;
		string tailMark = " ";
		string ans = "";
		line = line.append(tailMark);
		for (int i = 0; i < line.length(); i++) {
			char ch = line[i];
			if (ch != ' ') {
				ans += ch;
			}
			else {
				parameters.push_back(ans);
				ans = "";
			}
		}
		if (parameters[0] == "v")  //读取点
		{
			vector<GLfloat>Point;
			for (int i = 1; i < 4; i++) {
				GLfloat xyz = atof(parameters[i].c_str());
				Point.push_back(xyz);
				//求出该模型的x_max, x_min, y_max, y_min, z_max, z_min，用于后续切割平面的设置
				if (i == 1)
				{
					if (x_max <= xyz) x_max = xyz;
					else if (x_min >= xyz) x_min = xyz;
				}
				else if (i == 2)
				{
					if (y_max <= xyz) y_max = xyz;
					else if (y_min >= xyz) y_min = xyz;
				}
				else if (i == 3)
				{
					if (z_max <= xyz) z_max = xyz;
					else if (z_min >= xyz) z_min = xyz;
				}
			}
			vSets.push_back(Point);  //点集
		}
		else if (parameters[0] == "f") //读取面
		{
			vector<GLint>vIndexSets;
			for (int i = 1; i < 4; i++) {
				string x = parameters[i];
				string ans = "";
				for (int j = 0; j < x.length(); j++) {
					char ch = x[j];
					ans += ch;
				}
				GLint index = atof(ans.c_str());  //索引
				index = index--;
				vIndexSets.push_back(index);
			}
			fSets.push_back(vIndexSets);  //面集
		}
	}
	f.close();
}

void ObjLoader::Draw()  //绘制
{
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < fSets.size(); i++) {

		GLfloat VN[3];
		GLfloat SV1[3];
		GLfloat	SV2[3];
		GLfloat SV3[3];

		//三点绘制一个三角面片
		GLint f_v_index = (fSets[i])[0];
		GLint s_v_index = (fSets[i])[1];
		GLint t_v_index = (fSets[i])[2];
		SV1[0] = (vSets[f_v_index])[0];
		SV1[1] = (vSets[f_v_index])[1];
		SV1[2] = (vSets[f_v_index])[2];//点的x,y,z具体坐标
		SV2[0] = (vSets[s_v_index])[0];
		SV2[1] = (vSets[s_v_index])[1];
		SV2[2] = (vSets[s_v_index])[2];
		SV3[0] = (vSets[t_v_index])[0];
		SV3[1] = (vSets[t_v_index])[1];
		SV3[2] = (vSets[t_v_index])[2];

		//计算法向量
		GLfloat vec1[3], vec2[3], vec3[3];
		//p1p2向量
		vec1[0] = SV1[0] - SV2[0];
		vec1[1] = SV1[1] - SV2[1];
		vec1[2] = SV1[2] - SV2[2];
		//p1p3向量
		vec2[0] = SV1[0] - SV3[0];
		vec2[1] = SV1[1] - SV3[1];
		vec2[2] = SV1[2] - SV3[2];
		//法向量
		vec3[0] = vec1[1] * vec2[2] - vec1[2] * vec2[1];
		vec3[1] = vec2[0] * vec1[2] - vec2[2] * vec1[0];
		vec3[2] = vec2[1] * vec1[0] - vec2[0] * vec1[1];

		GLfloat D = sqrt(pow(vec3[0], 2) + pow(vec3[1], 2) + pow(vec3[2], 2));
		//法向量标准化
		VN[0] = vec3[0] / D;
		VN[1] = vec3[1] / D;
		VN[2] = vec3[2] / D;
		glNormal3f(VN[0], VN[1], VN[2]);
		//三角形面片的三个顶点
		glVertex3f(SV1[0], SV1[1], SV1[2]);
		glVertex3f(SV2[0], SV2[1], SV2[2]);
		glVertex3f(SV3[0], SV3[1], SV3[2]);
	}
	glEnd();
}

void ObjLoader::slice(int num_of_layers, int layer) //layer为第几层截面
{
	glBegin(GL_LINES);
	vector<GLfloat> export_obj;

	// 按照z轴平面切割
	float difference = (z_max - z_min) / num_of_layers;
	vector<GLfloat> S1{ 10000,10000,difference * layer + z_min };
	vector<GLfloat> S2{ -10000,10000,difference * layer + z_min };
	vector<GLfloat> S3{ 0,-10000,difference * layer + z_min };

	/*按照y轴平面切割
	float difference = (y_max - y_min) / 101;
	vector<GLfloat> S1{ 10000,difference * layer + y_min,-10000 };
	vector<GLfloat> S2{ -10000,difference * layer + y_min,-10000 };
	vector<GLfloat> S3{ 0,difference * layer + y_min,10000 };
	*/

	/*按照x轴平面切割
	float difference = (x_max - x_min) / 101;
	vector<GLfloat> S1{ difference * layer + x_min,10000,-10000 };
	vector<GLfloat> S2{ difference * layer + x_min,-10000,-10000 };
	vector<GLfloat> S3{ difference * layer + x_min,0,10000 };
	*/

	Intersection cut_surface(S1, S2, S3);  //用于切割的大平面

	for (int i = 0; i < fSets.size(); i++)
	{
		GLint Index[3];  //三角面片的三个顶点的索引
		for (int k = 0; k < 3; k++)
		{
			Index[k] = (fSets[i])[k];
		}
		vector<GLfloat> V1 = vSets[Index[0]], V2 = vSets[Index[1]], V3 = vSets[Index[2]];//三个顶点v1v2v3的坐标
		vector<GLfloat> node;
		vector<vector<GLfloat>> line;
		if (pan(V1, V2, cut_surface, node) == true)  //v1v2与切割面有交点
			line.push_back(node);
		if (pan(V2, V3, cut_surface, node) == true)
			line.push_back(node);
		if (pan(V1, V3, cut_surface, node) == true)
			line.push_back(node);

		if (line.size() != 0)   //三角面片与切割面有交点，此处需改进（有可能交点是顶点，需要加个判断）
		{
			glVertex3f(line[0][0], line[0][1], line[0][2]);  //两点画线
			glVertex3f(line[1][0], line[1][1], line[1][2]);
			export_obj.push_back(line[0][0]);
			export_obj.push_back(line[0][1]);
			export_obj.push_back(line[0][2]);
			export_obj.push_back(line[1][0]);
			export_obj.push_back(line[1][1]);
			export_obj.push_back(line[1][2]);
		}
	}
	glEnd();

	//导出为obj文件
	string path = "F:\\ObjLoader\\output\\";  //切面文件的输出路径
	string fname = path + to_string(layer) + ".obj";
	ofstream out_obj;
	out_obj.open(fname, ios::out);
	for (int i = 0; i < export_obj.size() - 2; i += 3)
	{
		out_obj << "v" << " " << export_obj[i] << " " << export_obj[i + 1] << " " << export_obj[i + 2] << endl;
	}
	for (int j = 0; j < export_obj.size() - 1; j += 2)
	{
		out_obj << "e" << " " << j + 1 << " " << j + 2 << endl;
	}
	out_obj.close();
}

