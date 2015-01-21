#include "my_qlabel.h"

my_qlabel::my_qlabel(QWidget *parent)
	: QLabel(parent)
{
	co = new my_co();
	co->setModal(true);
	connect(co, SIGNAL(number(int, int)), this, SLOT(get_number(int, int)));
	para = new Para();
	ok = -1;
	para->vrml.startVrml();
}

void my_qlabel::get_number(int x, int y)
{
	para->reals.push_back(Point(x,y));
}


QSize my_qlabel::OpenImage(const QString& fileName)
{
	para->image = imread(fileName.toAscii().data());
	para->image.copyTo(para->tem1);
	para->tem1.copyTo(para->tem2);
	para->load = true;

	showImage(para->image);
	this->resize(this->pixmap()->size());
	return this->pixmap()->size();
}

void my_qlabel::showImage(Mat& show_img)
{
	Mat tem;
	cvtColor(show_img,tem,CV_BGR2RGB);
	QImage img;
	img = QImage(tem.data, tem.cols, tem.rows, tem.cols*3, QImage::Format_RGB888);
	this->setPixmap(QPixmap::fromImage(img));
}

void my_qlabel::draw_circle(Mat& img, Point x)
{
	int thickness = -1;
	int lineType = 8;
	int radius = 4;
	circle(img, x , radius , Scalar(0,0,255), thickness, lineType);
}

void my_qlabel::draw_line(int num)
{
	int thickness = 1;
	int lineType = 8;
	Mat *img;
	if(num == 1)
	{
		img = & para->tem1;
	}
	else
	{
		para->tem1.copyTo(para->tem2);
		img = & para->tem2;
	}

	draw_circle(*img, para->l_start);
	line(*img, para->l_start, para->l_end, Scalar(0,255,255), thickness, lineType);
	draw_circle(*img, para->l_end);

	if(num == 1)
		para->tem1.copyTo(para->tem2);

	showImage(para->tem2);
}

void my_qlabel::mousePressEvent(QMouseEvent* ev)
{
	if(para->load && ok != 5)
	{
		para->l_start = Point(ev->x(),ev->y());
		para->l_end = Point(ev->x(),ev->y());
		para->draw = true;
		draw_line(2);
	}
	else if(para->load && ok == 5)
	{
		double min_dis = 1e30;
		double min_index = -1;
		double dis;
		for(int i = 0; i < d2_p.size(); i++)
		{
			dis = sqrt(double(d2_p[i](0) - ev->x()) * (d2_p[i](0) - ev->x()) + (d2_p[i](1) - ev->y()) * (d2_p[i](1) - ev->y()));
			if(min_dis > dis)
			{
				min_dis = dis;
				min_index= i;
			}
		}
		if(min_dis < 5)
		{
			d2_ver.push_back(d2_p[min_index]);
			d3_ver.push_back(d3_p[min_index]);
			circle(para->tem1, Point(d2_p[min_index](0), d2_p[min_index](1)) , 6 , Scalar(0,255,255), -1, 8);
			showImage(para->tem1);
		}
		else
		{
			showImage(para->tem1);
		}
	}

	emit Mouse_Pressed();
}

void my_qlabel::mouseMoveEvent(QMouseEvent *ev)
{
	if(para->load && para->draw)
	{
		para->l_end = Point(ev->x(),ev->y());
		draw_line(2);
		
		emit Mouse_Move();
	}
	else if(para->load && ok == 5)
	{
		double min_dis = 1e30;
		double min_index = -1;
		double dis;
		for(int i = 0; i < d2_p.size(); i++)
		{
			dis = sqrt(double(d2_p[i](0) - ev->x()) * (d2_p[i](0) - ev->x()) + (d2_p[i](1) - ev->y()) * (d2_p[i](1) - ev->y()));
			if(min_dis > dis)
			{
				min_dis = dis;
				min_index= i;
			}
		}
		if(min_dis < 5)
		{
			para->tem1.copyTo(para->tem2);
			circle(para->tem2, Point(d2_p[min_index](0), d2_p[min_index](1)) , 6 , Scalar(0,255,255), -1, 8);
			showImage(para->tem2);
		}
		else
		{
			showImage(para->tem1);
		}
	}
}

void my_qlabel::mouseReleaseEvent(QMouseEvent* ev)
{
	if(para->load && para->draw)
	{
		para->draw = false;
		draw_line(1);
		para->starts.push_back(para->l_start);
		para->ends.push_back(para->l_end);
		Vector3d point1, point2, line;
		point1(0) = para->l_start.x;
		point1(1) = para->l_start.y;
		point1(2) = 1;

		point2(0) = para->l_end.x;
		point2(1) = para->l_end.y;
		point2(2) = 1;
		line = point1.cross(point2);
		para->line_set.push_back(line);

		if(ok >= 0 && ok < 4)
		{
			co->exec();
			ok++;
		}

		emit Mouse_Release();
	}
}

void my_qlabel::save()
{
	para->vrml.endVrml();
	ofstream fout("output.wrl");
	fout << para->vrml.vrml_file;
	fout.close();
}

void my_qlabel::generateTexture(vector<Vector2i>& image_pts, vector<Vector3i>& world_pts){
	int x = 0, y = 1, z = 2;
	int x_average = 0, y_average = 0, z_average = 0;
	for (int i = 0; i < world_pts.size(); i++) {
		x_average += world_pts[i](x);
		y_average += world_pts[i](y);
		z_average += world_pts[i](z);
	}	
	x_average /= world_pts.size();
	y_average /= world_pts.size();
	z_average /= world_pts.size();

	int diff[3];
	for (int i = 0; i < 3; i++)
		diff[i] = 0;
	for (int i = 0; i < world_pts.size(); i++) {
		diff[x] += abs(world_pts[i](x) - x_average);
		diff[y] += abs(world_pts[i](y) - y_average);
		diff[z] += abs(world_pts[i](z) - z_average);
	}

	int min = diff[x], index = x;
	for (int i = 0; i < 3; i++){
		if (min > diff[i]) {
			min = diff[i];
			index = i;
		}
	}

	vector<Point> world_coor;
	for(int i = 0; i < world_pts.size(); i++)
	{
		vector<int> tem;
		for(int j = 0; j < 3; j++)
		{
			if(j != index)
			{
				tem.push_back(world_pts[i](j));
			}
		}

		world_coor.push_back(Point(tem[0],tem[1]));
	}

	int id[3];
	int threshold = 30;
	int max_x = world_coor[0].x, max_y = world_coor[0].y;

	for (int i = 1; i < world_coor.size(); i++) {
        if (world_coor[i].x > max_x)
            max_x = world_coor[i].x;
        
        if (world_coor[i].y > max_y)
            max_y = world_coor[i].y;
    }

	//cout << "Max_x: " << max_x << endl;
	//cout << "Max_y: " << max_y << endl;
    
    for (int i = world_coor.size()-1; i >= 0; i--) {
		world_coor[i].x = abs(world_coor[i].x - world_coor[0].x);
		world_coor[i].y = abs(world_coor[i].y - world_coor[0].y);

        int x_diff = (double)world_coor[i].x / (double)max_x * 100.0;
        int y_diff = (double)world_coor[i].y / (double)max_y * 100.0;
        
        if (x_diff > threshold && y_diff < threshold) {
            id[0] = i;
            world_coor[i].y = 0;
        }
        else if (x_diff < threshold && y_diff > threshold) {
            id[1] = i;
            world_coor[i].x = 0;
        }
        else if (x_diff > threshold && y_diff > threshold) {
            id[2] = i;
        }
    }

	/*
	cout<<id[0]<<" "<<id[1]<<" "<<id[2]<<endl<<endl;

	for(int i = 0; i < world_coor.size(); i++)
		cout<<world_coor[i].x<<" "<<world_coor[i].y<<endl;

	//cout << "------------------" << endl;
	*/
	int tem  = std::min(world_coor[id[0]].x, world_coor[id[2]].x);
	world_coor[id[0]].x = world_coor[id[2]].x = tem;
	tem = std::min(world_coor[id[1]].y, world_coor[id[2]].y);
	world_coor[id[1]].y = world_coor[id[2]].y = tem;

	vector<Point>img_coor;
	for(int i = 0; i < image_pts.size(); i++)
	{
		img_coor.push_back(Point(image_pts[i](0),image_pts[i](1)));
	}

	//for(int i = 0; i < world_coor.size(); i++)
		//cout<<world_coor[i].x<<" "<<world_coor[i].y<<endl;

	//cout<<endl;

	//for(int i = 0; i < img_coor.size(); i++)
		//cout<<img_coor[i].x<<" "<<img_coor[i].y<<endl;

	//cout<<endl;

	Matrix3d h;
	homo(world_coor, img_coor, h);

	for(int i = 0; i < world_coor.size(); i++)
	{
		Vector3d map = h * Vector3d(world_coor[i].x, world_coor[i].y, 1);
		map(0) = map(0) / map(2);
		map(1) = map(1) / map(2);
	
		/*
		cout << "Test for homography: " << endl;
		cout << world_coor[i].x << ' ' << world_coor[i].y << endl;
		cout<<map(0)<<" "<<map(1)<<endl;
		*/
	}
	Mat texture(world_coor[id[2]].y, world_coor[id[2]].x, CV_8UC3);
	/*
	cout << endl;
	cout << "texture cols & rows:" << endl;
	cout << texture.cols << ' ' << texture.rows << endl;
	cout << "---------------------" << endl;
	*/
	int x_min, x_max, y_min, y_max;
	Vec3d pixel_x1, pixel_x2;
	for(int i = 0; i < texture.cols; i++)
	{
		for(int j = 0; j < texture.rows; j++)
		{
			Vector3d pos(i,j,1);
			Vector3d map = h * pos;
			map(0) = map(0) / map(2);
			map(1) = map(1) / map(2);

			
			if(map(0)>= 0 && map(0) < para->image.cols && map(1) >= 0 && map(1) < para->image.rows)
			{
				
				x_min = floor(map(1));
				if(x_min < 0)
					x_min = 0;

				x_max = ceil(map(1));
				if(x_max >= para->image.rows)
					x_max = para->image.rows - 1;

				y_min = floor(map(0));
				if(y_min < 0)
					y_min = 0;

				y_max = ceil(map(0)); 
				if(y_max >= para->image.cols)
					y_max = para->image.cols - 1;

				pixel_x1 = (y_max - map(0)) * para->image.at<Vec3b>(x_min,y_min) + (map(0) - y_min) * para->image.at<Vec3b>(x_min,y_max);
				pixel_x2 = (y_max - map(0)) * para->image.at<Vec3b>(x_max,y_min) + (map(0) - y_min) * para->image.at<Vec3b>(x_max,y_max);
				texture.at<Vec3b>(j,i) = (x_max - map(1)) * pixel_x1 + (map(1) - x_min) * pixel_x2;
				
			}
			else
			{
				texture.at<Vec3b>(j,i)[0] = texture.at<Vec3b>(j,i)[1] = texture.at<Vec3b>(j,i)[2] = 0;
			}
		}
	}
	
	string filename = para->vrml.convertInt(para->poly_index) + ".jpg";
	para->vrml.appendPolygon(filename, world_pts, world_coor);
	imwrite(filename, texture);

	para->poly_index++;
	/*for(int i = 0; i < world_pts.size(); i++)
		cout<<world_pts[i](0)<<" "<<world_pts[i](1)<<" "<<world_pts[i](2)<<endl;

	cout<<world_coor[0].x<<" "<<world_coor[0].y<<endl;
	cout<<world_coor[id[2]].x<<" "<<world_coor[id[2]].y<<endl;*/
}

void my_qlabel::homo(vector<Point> sets, vector<Point> map_sets, Matrix3d& h)
{
	MatrixXd A(8,9);
	for(int i = 0; i < 4; i++)
	{
		int first = 2*i, second = 2*i+1;
		A(first,0) = A(second,3) = sets[i].x;
		A(first,1) = A(second,4) = sets[i].y;
		A(first,2) = A(second,5) =1;
		A(first,3) = A(first,4) = A(first,5) = A(second,0) = A(second,1) = A(second,2) = 0;
		A(first,6) = -1 * map_sets[i].x * sets[i].x;
		A(first,7) = -1 * map_sets[i].x * sets[i].y;
		A(first,8) = -1 * map_sets[i].x;
		A(second,6) = -1 * map_sets[i].y * sets[i].x;
		A(second,7) = -1 * map_sets[i].y * sets[i].y;
		A(second,8) = -1 * map_sets[i].y;
	}

	MatrixXd r_A(9,9);
	r_A =  A.transpose() * A;
	SelfAdjointEigenSolver<MatrixXd> eigensolver(r_A);
	VectorXd value = eigensolver.eigenvalues();
	MatrixXd matrix = eigensolver.eigenvectors();
	double min;
	int min_index;
	for(int i = 0; i < value.size(); i++)
	{
		if(i == 0 || value(i) < min)
		{
			min = value(i);
			min_index = i;
		}
	}

	VectorXd v_h(9);
	for(int i = 0; i < 9; i++)
		v_h(i) = matrix(i,min_index);

	for(int i = 0; i < 3; i++)
	{
		h(i,0) = v_h(i*3+0);
		h(i,1) = v_h(i*3+1);
		h(i,2) = v_h(i*3+2);
	}
}

void my_qlabel::calculate(const QString status)
{
	if(status == "X Vanishing Point" || status == "Y Vanishing Point" || status == "Z Vanishing Point")
	{
		Matrix3d m(3,3);
		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				m(i,j) = 0;
			}
		}

		for(int k = 0; k < para->line_set.size(); k++)
		{
			Matrix3d t(3,3);
			t(0,0) = para->line_set[k](0) * para->line_set[k](0);
			t(0,1) = t(1,0) = para->line_set[k](0) * para->line_set[k](1);
			t(1,1) = para->line_set[k](1) *para->line_set[k](1);
			t(0,2) = t(2,0) = para->line_set[k](0) * para->line_set[k](2);
			t(1,2) = t(2,1) = para->line_set[k](1) * para->line_set[k](2);
			t(2,2) = para->line_set[k](2) *para->line_set[k](2);

			for(int i = 0; i < 3; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					m(i,j) += t(i,j);
				}
			}
		}

		//eigenvalue & get vanishing poing
		SelfAdjointEigenSolver<Matrix3d> eigensolver(m);
		VectorXd value = eigensolver.eigenvalues();
		MatrixXd matrix = eigensolver.eigenvectors();
		double min;
		int min_index;
		for(int i = 0; i < value.size(); i++)
		{
			if(i == 0 || value(i) < min)
			{
				min = value(i);
				min_index = i;
			}
		}
		Vector3d vanish;
		for(int i = 0; i < 3; i++)
			vanish(i) = matrix(i,min_index);
		vanish(0) = vanish(0) / vanish(2);
		vanish(1) = vanish(1) / vanish(2);
		vanish(2) = 1;

		if(status == "X Vanishing Point")
			para->vanish_x = vanish;
		else if(status == "Y Vanishing Point")
			para->vanish_y = vanish;
		else
			para->vanish_z = vanish;

		para->line_set.clear();
		para->starts.clear();
		para->ends.clear();
		para->image.copyTo(para->tem1);
		showImage(para->tem1);
		emit finished(vanish);
	}
	else if(status == "Coefficient")
	{
		Vector3d coes;
		Point start,end;

		start = para->starts.back();
		end = para->ends.back();
		para->starts.pop_back();
		para->ends.pop_back();
		para->origin(0) = start.x;
		para->origin(1) = start.y;
		para->origin(2) = 1;
		para->point_z(0) = end.x;
		para->point_z(1) = end.y;
		para->point_z(2) = 1;
		para->coe_z = getValue('z',para->point_z, para->origin) / para->ref_z;
		coes(2) = para->coe_z;

		end = para->ends.back();
		para->ends.pop_back();
		para->point_y(0) = end.x;
		para->point_y(1) = end.y;
		para->point_y(2) = 1;
		para->coe_y = getValue('y',para->point_y, para->origin) / para->ref_y;
		coes(1) = para->coe_y;

		end = para->ends.back();
		para->point_x(0) = end.x;
		para->point_x(1) = end.y;
		para->point_x(2) = 1;
		para->coe_x = getValue('x',para->point_x, para->origin) / para->ref_x;
		coes(0) = para->coe_x;

		para->line_set.clear();
		para->starts.clear();
		para->ends.clear();
		para->image.copyTo(para->tem1);
		showImage(para->tem1);
		ok = 0;
		emit finished(coes);
	}
	else if(status == "Measuring Lines")
	{
		//calculate the homography
		Matrix3d h;
		homo(para->ends, para->reals, h);
		
		//find the 3D coordinate of reference plane
		Vector2i tem_d2;
		Vector3i tem_d3;
		for(int i = 0; i < para->ends.size(); i++)
		{
			Vector3d result;
			result = h * Vector3d(para->ends[i].x, para->ends[i].y, 1);
			result(0) = result(0) / result(2);
			result(1) = result(1) / result(2);
			
			tem_d2(0) = para->ends[i].x;
			tem_d2(1) = para->ends[i].y;
			tem_d3(0) = result(0);
			tem_d3(1) = result(1);
			tem_d3(2) = 0;
			d2_p.push_back(tem_d2);
			d3_p.push_back(tem_d3);

			tem_d2(0) = para->starts[i].x;
			tem_d2(1) = para->starts[i].y;
			tem_d3(0) = result(0);
			tem_d3(1) = result(1);
			tem_d3(2) = getValue('z', Vector3d(para->starts[i].x, para->starts[i].y, 1), Vector3d(para->ends[i].x, para->ends[i].y, 1)) / para->coe_z;
			d2_p.push_back(tem_d2);
			d3_p.push_back(tem_d3);
		}

		para->line_set.clear();
		para->starts.clear();
		para->ends.clear();
		para->image.copyTo(para->tem1);
		for(int i = 0; i < d2_p.size(); i++)
		{
			circle(para->tem1, Point(d2_p[i](0), d2_p[i](1)) , 4 , Scalar(0,0,255), -1, 8);
		}
		para->tem1.copyTo(para->tem2);
		showImage(para->tem2);
		emit finished_p(d2_p, d3_p);
		ok++;
	}
	else if (status == "Texture Polygon")
	{
		generateTexture(d2_ver, d3_ver);
		para->image.copyTo(para->tem1);
		for(int i = 0; i < d2_p.size(); i++)
		{
			circle(para->tem1, Point(d2_p[i](0), d2_p[i](1)) , 4 , Scalar(0,0,255), -1, 8);
		}
		showImage(para->tem1);
		d2_ver.clear();
		d3_ver.clear();
	}
}

double my_qlabel::getValue(char axis, Vector3d t, Vector3d b)
{
	Vector3d v,l,tem_v;
	Vector3d o = para->vanish_x.cross(para->vanish_y);
	o.normalize();
	double tem;
	if(axis == 'x')
	{
		v = para->vanish_x;
		l = para->vanish_y.cross(para->vanish_z);
	}
	else if(axis == 'y')
	{
		v = para->vanish_y;
		l = para->vanish_x.cross(para->vanish_z);
	}
	else
	{
		v = para->vanish_z;
		l = para->vanish_x.cross(para->vanish_y);
	}
	l.normalize();
	double value = o.dot(l) * (b.cross(t)).norm();
	tem = b.dot(l) *(v.cross(t)).norm();
	value = - value / tem;

	return value;
}


my_qlabel::~my_qlabel()
{

}
