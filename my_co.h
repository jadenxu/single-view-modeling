#ifndef MY_CO_H
#define MY_CO_H

#include <QDialog>
#include <QMessageBox>
#include "ui_my_co.h"

class my_co : public QDialog
{
	Q_OBJECT

public:
	my_co(QWidget *parent = 0);
	~my_co();
	int x;
	int y;

public slots:
	void send();
signals:
	void number(int x, int y);


private:
	Ui::my_co ui;
};

#endif // MY_CO_H
