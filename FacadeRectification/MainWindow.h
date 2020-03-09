#pragma once

#include <QtWidgets/QMainWindow>
#include "Canvas.h"
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

public slots:
	void onOpen();
	void onRectify();

private:
	Ui::MainWindowClass ui;
	Canvas canvas;
};
