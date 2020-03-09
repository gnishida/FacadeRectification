#include "Canvas.h"
#include <QPainter>
#include <iostream>
#include <QFileInfoList>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QResizeEvent>
#include <algorithm>


Canvas::Canvas(QWidget *parent) : QWidget(parent) {
	ctrlPressed = false;
	shiftPressed = false;
}

void Canvas::paintEvent(QPaintEvent *event) {
	if (!image.isNull()) {
		QPainter painter(this);
		painter.drawImage(0, 0, image);

		QPolygon polygon;
		for (const auto& p : points) {
			polygon.append(QPoint(p.x * image.width(), p.y * image.height()));
		}
		painter.setPen(QPen(QBrush(QColor(0, 0, 255)), 3));
		painter.drawPolygon(polygon);
	}
}

void Canvas::mousePressEvent(QMouseEvent* e) {
	if (points.size() >= 4) points.clear();

	points.push_back(cv::Point2f((float)e->x() / image.width(), (float)e->y() / image.height()));

	update();
}

void Canvas::resizeEvent(QResizeEvent *e) {
	if (!orig_image.isNull()) {
		float scale = std::min((float)width() / orig_image.width(), (float)height() / orig_image.height());
		image = orig_image.scaled(orig_image.width() * scale, orig_image.height() * scale);
	}
}

void Canvas::loadImage(const QString& filename) {
	orig_image = QImage(filename).convertToFormat(QImage::Format_RGB888);
	float scale = std::min((float)width() / orig_image.width(), (float)height() / orig_image.height());
	image = orig_image.scaled(orig_image.width() * scale, orig_image.height() * scale);
	
	points.clear();

	update();
}

void Canvas::rectify() {
	if (points.size() < 4) return;

	std::vector<cv::Point2f> src;
	for (const auto& p : points) {
		src.push_back(cv::Point2f(p.x * orig_image.width(), p.y * orig_image.height()));
	}

	// Calculate the perspective transformation matrix
	std::vector<cv::Point2f> dest;
	dest.push_back(cv::Point2f(0, 0));
	dest.push_back(cv::Point2f(orig_image.width(), 0));
	dest.push_back(cv::Point2f(orig_image.width(), orig_image.height()));
	dest.push_back(cv::Point2f(0, orig_image.height()));
	cv::Mat transMat = cv::getPerspectiveTransform(src, dest);

	// Convert QImage to cv::Mat
	cv::Mat inImg = cv::Mat(orig_image.height(), orig_image.width(), CV_8UC3, const_cast<uchar*>(orig_image.bits()), orig_image.bytesPerLine()).clone();

	// Warp the image
	cv::Mat outImg;
	cv::warpPerspective(inImg, outImg, transMat, cv::Size(orig_image.width(), orig_image.height()));

	// Convert cv::Mat to QImage
	orig_image = QImage(outImg.data, outImg.cols, outImg.rows, outImg.step, QImage::Format_RGB888).copy();
	
	float scale = std::min((float)width() / orig_image.width(), (float)height() / orig_image.height());
	image = orig_image.scaled(orig_image.width() * scale, orig_image.height() * scale);

	points.clear();

	update();
}

void Canvas::keyPressEvent(QKeyEvent* e) {
	ctrlPressed = false;
	shiftPressed = false;

	if (e->modifiers() & Qt::ControlModifier) {
		ctrlPressed = true;
	}
	if (e->modifiers() & Qt::ShiftModifier) {
		shiftPressed = true;
	}

	switch (e->key()) {
	case Qt::Key_Space:
		break;
	}

	update();
}

void Canvas::keyReleaseEvent(QKeyEvent* e) {
	switch (e->key()) {
	case Qt::Key_Control:
		ctrlPressed = false;
		break;
	case Qt::Key_Shift:
		shiftPressed = false;
		break;
	default:
		break;
	}
}

