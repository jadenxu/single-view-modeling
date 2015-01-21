#ifndef PARA_H
#define PARA_H

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<Eigen/dense>

#include "Vrml.h"
using namespace Eigen;
using namespace cv;

class Para
{
public:
	Para();
	Mat image, tem1, tem2;
	Point l_start,l_end;
	vector<Point> starts, ends;
	vector<Point> reals;
	bool draw,exe,load;
	Vector3d vanish_x,vanish_y,vanish_z;
	vector<Vector3d> line_set;
	Vector3d origin, point_x, point_y, point_z;
	double ref_x, ref_y, ref_z;
	double coe_x, coe_y, coe_z;


	int poly_index;
	Vrml vrml;
};

#endif
