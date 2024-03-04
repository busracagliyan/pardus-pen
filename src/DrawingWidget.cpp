#include "DrawingWidget.h"
#include <stdio.h>

DrawingWidget::DrawingWidget(QWidget *parent): QWidget(parent) {
    initializeImage(size());
    QList<QScreen*> screens = QGuiApplication::screens();
    int screenWidth = 0;
    int screenHeight = 0;
    for (QScreen *screen : screens) {
        screenWidth  += screen->geometry().width();
        screenHeight += screen->geometry().height();
        break;
    }
    setFixedSize(screenWidth, screenHeight);
}

DrawingWidget::~DrawingWidget() {}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        lastPoint = event->pos();
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        drawLineTo(event->pos());
    }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && drawing) {
       drawLineTo(event->pos()+QPoint(0,1));
       drawing = false;
    }
}

void DrawingWidget::initializeImage(const QSize &size) {
    image = QImage(size, QImage::Format_ARGB32);
    image.fill(QColor("transparent"));
}

void DrawingWidget::resizeEvent(QResizeEvent *event) {
    initializeImage(event->size());
    QWidget::resizeEvent(event);
}

void DrawingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawImage(0, 0, image);
}

void DrawingWidget::setEraser(bool enabled) {
    eraser = enabled;
}

void DrawingWidget::clear() {
    image.fill(QColor("transparent"));
    update();
}


int rad = 0;

void DrawingWidget::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&image);
    if(eraser){
        painter.setCompositionMode(QPainter::CompositionMode_Clear);
        painter.setPen(QPen(penColor, eraserWidth, Qt::SolidLine, Qt::RoundCap));
        rad = eraserWidth;
    }else{
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap));
        rad = eraserWidth;
    }
 
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawLine(lastPoint, endPoint);
    update(QRect(lastPoint, endPoint).normalized()
           .adjusted(-rad, -rad, +rad, +rad));

    lastPoint = endPoint;
}

bool DrawingWidget::event(QEvent *ev) {
  switch (ev->type()) {
    case QEvent::TouchBegin:
        std::cout << "Event began." << std::endl;
        break;
    case QEvent::TouchEnd:
        std::cout << "Event end." << std::endl;
        break;
    case QEvent::TouchUpdate:
        std::cout << "Event update." << std::endl;
        break;
    default:
        //std::cout << "Misc event." << std::endl;
        break;
    }
    return QWidget::event(ev);
}


QColor convertColor(QColor color) {
    int red = color.red();
    int green = color.green();
    int blue = color.blue();
    int alpha = color.alpha();
    return QColor(255-blue, 255-red, 255-green, alpha);
}