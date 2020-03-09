#include "MainWindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setCentralWidget(&canvas);
	setWindowTitle(tr("Rectification"));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
	connect(ui.actionRectify, SIGNAL(triggered()), this, SLOT(onRectify()));
}

void MainWindow::onOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open Image file..."), "", tr("Image Files (*.png *jpg)"));
	if (filename.isEmpty()) return;

	canvas.loadImage(filename);
}

void MainWindow::onRectify() {
	canvas.rectify();
}