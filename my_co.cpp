#include "my_co.h"

my_co::my_co(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(send()));
}

void my_co::send()
{
	emit number(ui.textEdit->toPlainText().toInt(), ui.textEdit_2->toPlainText().toInt());
	this->close();
}

my_co::~my_co()
{

}
