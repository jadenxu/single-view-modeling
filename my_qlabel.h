#ifndef MY_QLABEL_H
#define MY_QLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QDebug>
#include <QMessageBox>
#include "Para.h"
#include "my_co.h"
#include "Vrml.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<Eigen/dense>
#include <iostream>
#include<vector>
#include <math.h>
#include <algorithm>
#include <fstream>
using namespace std;
using namespace Eigen;
using namespace cv;

class my_qlabel : public QLabel
{
	Q_OBJECT

public:
	my_qlabel(QWidget *parent);
	~my_qlabel();
	void mousePressEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void mouseReleaseEvent(QMouseEvent *ev);
	QSize OpenImage(const QString& fileName);
	void draw_circle(Mat& img, Point x);
	void draw_line(int num);
	void showImage(Mat& show_img);
	double getValue(char axis, Vector3d t, Vector3d b);
	void homo(vector<Point> sets, vector<Point> map_sets, Matrix3d& h);
	void save();

	void generateTexture(vector<Vector2i>& image_pts, vector<Vector3i>& world_pts);		

	Para *para;
	int ok; 
	my_co* co;
	vector<Vector2i> d2_p;
	vector<Vector3i> d3_p;
	vector<Vector2i> d2_ver;
	vector<Vector3i> d3_ver;

signals:
	void Mouse_Pressed();
	void Mouse_Release();
	void Mouse_Move();
	void finished(Vector3d v);
	void finished_p(vector<Vector2i> d2_p, vector<Vector3i> d3_p);

public slots:
	void calculate(const QString status);
	void get_number(int x, int y);

};

#endif // MY_QLABEL_H
