#ifndef MY_DIALOG_H
#define MY_DIALOG_H

#include <QDialog>
#include <QtCore>
#include <QtGui>
#include <QAction>
#include "ui_my_dialog.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<Eigen/dense>
using namespace Eigen;
using namespace cv;

class my_dialog : public QDialog
{
	Q_OBJECT

public:
	my_dialog(QWidget *parent = 0);
	~my_dialog();
	void setView();
	void createActions();

signals:
	void cal(const QString);

public slots:
	void button_down();
	void finish(Vector3d v);
	void finish_p(vector<Vector2i> d2_p, vector<Vector3i> d3_p);

private:
	QStandardItemModel* model1;
	QStandardItemModel* model2;
	QStandardItemModel* model3;
	Ui::my_dialog ui;
	QString status;
};

#endif // MY_DIALOG_H
