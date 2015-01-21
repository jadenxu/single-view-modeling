#ifndef QTQ_H
#define QTQ_H

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QtGui/QMainWindow>
#include "ui_qtq.h"
#include "my_qlabel.h"
#include "my_dialog.h"

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<Eigen/dense>
using namespace Eigen;
using namespace cv;

class qtq : public QMainWindow
{
	Q_OBJECT

public:
	qtq(QWidget *parent = 0, Qt::WFlags flags = 0);
	~qtq();
	void createActions();
	void createMenus();
	void my_clear();

private slots:
	void Mouse_Pressed();
	void Mouse_Release();
	void Mouse_Move();
	void OpenImage();
	void about();
	void final();

private:
	Ui::qtqClass ui;
	my_dialog *dia;
	int count;
};

#endif // QTQ_H
