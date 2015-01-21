#include "qtq.h"
#include "my_qlabel.h"

qtq::qtq(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	count = 0;
	ui.setupUi(this);
	dia = new my_dialog(this);
	dia->show();

	setCentralWidget(ui.label);
	this->setWindowTitle("Single View Modeling");
	this->resize(500,500);
	createActions();
}

void qtq::Mouse_Pressed()
{
	ui.statusBar->showMessage(QString("Press Start(%1,%2) End(%4,%5)").arg(ui.label->para->l_start.x).arg(ui.label->para->l_start.y)
		.arg(ui.label->para->l_end.x).arg(ui.label->para->l_end.y));
}

void qtq::Mouse_Release()
{
	ui.statusBar->showMessage(QString("Mouse Release"));
}

void qtq::Mouse_Move()
{
	ui.statusBar->showMessage(QString("Move Start(%1,%2) End(%4,%5)").arg(ui.label->para->l_start.x).arg(ui.label->para->l_start.y)
		.arg(ui.label->para->l_end.x).arg(ui.label->para->l_end.y));
}

void qtq::createActions()
{
	connect(ui.label, SIGNAL(Mouse_Pressed()),this, SLOT(Mouse_Pressed()));
	connect(ui.label, SIGNAL(Mouse_Release()),this, SLOT(Mouse_Release()));
	connect(ui.label, SIGNAL(Mouse_Move()),this, SLOT(Mouse_Move()));
	connect(ui.actionOpen_Image, SIGNAL(triggered()), this, SLOT(OpenImage()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(dia, SIGNAL(cal(const QString)), ui.label, SLOT(calculate(const QString)));
	connect(ui.label, SIGNAL(finished(Vector3d)), dia, SLOT(finish(Vector3d)));
	connect(ui.label, SIGNAL(finished_p(vector<Vector2i>, vector<Vector3i>)), dia, SLOT(finish_p(vector<Vector2i>, vector<Vector3i>)));
	connect(ui.actionSave_Image, SIGNAL(triggered()), this, SLOT(final()));
}

void qtq::final()
{
	ui.label->save();
}

void qtq::my_clear()
{
	qtq();
}

void qtq::OpenImage()
{
	QSize s;
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), ".", tr("Image Files (*.png *.jpg *.jpeg *.bmp)"));
	if(fileName.isEmpty())
		ui.statusBar->showMessage(QString("Cannot load image"));
	else
	{
		if(count != 0)
			my_clear();
		s = ui.label->OpenImage(fileName);
		this->resize(s);
		count++;
	}
}

void qtq::about()
 {
     QMessageBox::about(this, tr("About Single View Modeling"),
             tr("<p>This program will create 3D texture-mapped models from a single image using the single view modeling method</p>"));
 }


qtq::~qtq()
{

}
