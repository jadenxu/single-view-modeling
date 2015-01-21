#ifndef VRML_H
#define VRML_H
#include <string>
#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <Eigen/dense>

using namespace std;
using namespace cv;
using namespace Eigen;

class Vrml
{
public:
	Vrml(void);
	~Vrml(void);

	string vrml_file;
	string header;
	string initial_position;


	void startVrml();
	void appendPolygon(string filename, vector<Vector3i> world_pts, vector<Point> texture_pts);
	void endVrml();
	void debug();
	string convertInt(int number);

};

#endif
