#include "my_dialog.h"

my_dialog::my_dialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setView();
	createActions();
}

void my_dialog::createActions()
{
	connect(ui.pushButton, SIGNAL(clicked()),this, SLOT(button_down()));
}

void my_dialog::button_down()
{
	status = ui.comboBox->currentText();
	emit cal(status);
}

void my_dialog::finish(Vector3d v)
{
	QString type[3];
	type[0] = QString("X");
	type[1] = QString("Y");
	type[2] = QString("Z");
	//QMessageBox::information(this, "Debug", QString("(%1,%2,%3)").arg(v(0)).arg(v(1)).arg(v(2)));
	if(status == "X Vanishing Point" || status == "Y Vanishing Point" || status == "Z Vanishing Point")
	{
		int r_index;
		if(status == "X Vanishing Point")
			r_index = 0;
		else if(status == "Y Vanishing Point")
			r_index = 1;
		else
			r_index = 2;

		for(int i = 0; i < 3; i++)
		{
			QModelIndex index = model1->index(r_index, i, QModelIndex());
			if(i == 0)
				model1->setData(index,type[r_index]);
			else
				model1->setData(index, v(i-1));
		}
	}
	else if(status == "Coefficient")
	{

		for(int i = 0; i < 3; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				QModelIndex index = model2->index(i, j, QModelIndex());
				if(j == 0)
					model2->setData(index,type[i]);
				else if(j == 1)
					model2->setData(index, 100);
				else
					model2->setData(index,v(i));
			}
		}
	}
}

void my_dialog::finish_p(vector<Vector2i> d2_p, vector<Vector3i> d3_p)
{
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 6; j++)
		{
			QModelIndex index = model3->index(i, j, QModelIndex());
			if(j == 0)
				model3->setData(index,i);
			else if(j == 1 || j == 2)
			{
				model3->setData(index,d2_p[i](j-1));
			}
			else
			{
				model3->setData(index,d3_p[i](j-3));
			}
		}
	}
}

void my_dialog::setView()
{
	ui.comboBox->addItem("X Vanishing Point");
	ui.comboBox->addItem("Y Vanishing Point");
	ui.comboBox->addItem("Z Vanishing Point");
	ui.comboBox->addItem("Coefficient");
	ui.comboBox->addItem("Measuring Lines");
	ui.comboBox->addItem("Texture Polygon");

	model1 = new QStandardItemModel(3,3,this);
	ui.tableView->setModel(model1);
	model1->setHeaderData(0,Qt::Horizontal,QObject::tr("Vanishing Point"));
	model1->setHeaderData(1,Qt::Horizontal,QObject::tr("Img-x"));
	model1->setHeaderData(2,Qt::Horizontal,QObject::tr("Img-y"));

	model2 = new QStandardItemModel(3,3,this);
	ui.tableView_2->setModel(model2);
	model2->setHeaderData(0,Qt::Horizontal,QObject::tr("Axis"));
	model2->setHeaderData(1,Qt::Horizontal,QObject::tr("Ref. Length"));
	model2->setHeaderData(2,Qt::Horizontal,QObject::tr("Alpha"));

	model3 = new QStandardItemModel(8,6,this);
	ui.tableView_3->setModel(model3);
	model3->setHeaderData(0,Qt::Horizontal,QObject::tr("Pair No."));
	model3->setHeaderData(1,Qt::Horizontal,QObject::tr("Img-x"));
	model3->setHeaderData(2,Qt::Horizontal,QObject::tr("Img-y"));
	model3->setHeaderData(3,Qt::Horizontal,QObject::tr("3D-X"));
	model3->setHeaderData(4,Qt::Horizontal,QObject::tr("3D-Y"));
	model3->setHeaderData(5,Qt::Horizontal,QObject::tr("3D-Z"));
}

my_dialog::~my_dialog()
{

}
