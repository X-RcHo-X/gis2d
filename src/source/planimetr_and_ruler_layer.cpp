#include "planimetr_and_ruler_layer.h"
#include "base_source_functions.h"

//WIN
#include <math.h>

//Qt
#include <QGridLayout>
#include <QPainter>

CPlanimetrAndRulerLayer::CPlanimetrAndRulerLayer()
{

}

CPlanimetrAndRulerLayer::~CPlanimetrAndRulerLayer()
{

}

void CPlanimetrAndRulerLayer::repaint(QPainter& painter)
{
    painter.setPen(QPen(Qt::red, 10, Qt::SolidLine));
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPoint center = earth::getCenter(m_layerInfo.screenMiddle, m_layerInfo.scroll);
    QPoint startPoint = earth::getStartPoint(center, m_layerInfo.zoom);
    QPoint pointForDistanceRect;

    for (auto coords : m_pointVec)
    {
        auto [pixelX, pixelY] = earth::getPixelsFromLatLon(*std::get<0>(coords), *std::get<1>(coords), m_layerInfo.zoom);
        if (m_pointVec.size() != 1)
            pointForDistanceRect = QPoint(pixelX + startPoint.x(), pixelY + startPoint.y());
        duplicatePoint(painter, QPoint(pixelX + startPoint.x(), pixelY + startPoint.y()));
    }

    double ortodromiaLenght = 0;
    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine));
    for (int i = 0; i < m_pointVec.size() - 1; ++i)
    {
        m_distanceWidget->setVisible(true);

        //Костыльный булёк, который нужен для того,
        //чтобы рисовать прямоугольник с измеренным расстоянием
        bool isReverse = false;

        auto startPointCoords = m_pointVec[i];
        auto endPointCoords = m_pointVec[i + 1];

        double fiStart = *std::get<0>(startPointCoords);
        double lambdaStart = *std::get<1>(startPointCoords);

        double fiEnd = *std::get<0>(endPointCoords);
        double lambdaEnd = *std::get<1>(endPointCoords);

        if (lambdaStart > lambdaEnd)
        {
            if (lambdaStart - lambdaEnd > 180)
            {
                lambdaEnd += 360;
            }
            else
            {
                std::swap(lambdaStart, lambdaEnd);
                std::swap(fiStart, fiEnd);
                isReverse = true;
            }
        }
        else if (lambdaEnd - lambdaStart > 180)
        {
            lambdaStart += 360;

            std::swap(lambdaStart, lambdaEnd);
            std::swap(fiStart, fiEnd);
            isReverse = true;
        }

        double latCurrent;
        double lonCurrent;

        double latBegin = qDegreesToRadians(fiStart);
        double lonBegin = qDegreesToRadians(lambdaStart);

        double latEnd = qDegreesToRadians(fiEnd);
        double lonEnd = qDegreesToRadians(lambdaEnd);

        //Вычисление ородромии по формулам из методички МИИГАиК'а
        double radOrtodromia = abs(acos(sin(latBegin) * sin(latEnd) + cos(latBegin) * cos(latEnd) * cos(lonEnd - lonBegin)));
        ortodromiaLenght += (radOrtodromia * constant::earth::EARTH_RADIUS) / 1000;

        auto[prevPixelX, prevPixelY] = earth::getPixelsFromLatLon(fiStart, lambdaStart, m_layerInfo.zoom);
        QPoint prevPoint = QPoint(prevPixelX + startPoint.x(), prevPixelY + startPoint.y());

        if (isReverse)
            pointForDistanceRect = QPoint(prevPixelX + startPoint.x(), prevPixelY + startPoint.y());

        if (lonBegin == lonEnd)
        {
            auto[pixelX, pixelY] = earth::getPixelsFromLatLon(fiEnd, lambdaEnd, m_layerInfo.zoom);
            painter.drawLine(prevPoint, QPoint(pixelX + startPoint.x(), pixelY + startPoint.y()));
            duplicateLine(painter, prevPoint, QPoint(pixelX + startPoint.x(), pixelY + startPoint.y()));
            continue;
        }

        for (double i = lambdaStart; i < lambdaEnd + 1;)
        {
            lonCurrent = i > lambdaEnd ? lambdaEnd : i;

            //Формулы нахождения ортодромии взял отсюда https://ru.wikipedia.org/wiki/Ортодромия
            latCurrent = atan(((tan(latBegin) * sin(lonEnd - qDegreesToRadians(lonCurrent)) / (sin(lonEnd - lonBegin)))
                               + (tan(latEnd) * sin(qDegreesToRadians(lonCurrent) - lonBegin)) / (sin(lonEnd - lonBegin))));
            i += 1;

            double degLat = qRadiansToDegrees(latCurrent);
            if (degLat > 85.)
                degLat = 84.99;
            if (degLat < -85.)
                degLat = -84.99;

            auto[curPixelX, curPixelY] = earth::getPixelsFromLatLon(degLat, lonCurrent, m_layerInfo.zoom);
            QPoint curPoint = QPoint(curPixelX + startPoint.x(), curPixelY + startPoint.y());

            if (!isReverse)
                pointForDistanceRect = QPoint(curPixelX + startPoint.x(), curPixelY + startPoint.y());

            painter.drawLine(prevPoint, QPoint(curPixelX + startPoint.x(), curPixelY + startPoint.y()));

            duplicateLine(painter, prevPoint, curPoint);

            prevPoint = QPoint(curPixelX + startPoint.x(), curPixelY + startPoint.y());
        }
    }
    if (m_distanceWidget)
    {
        //Чтобы виджет-справка мог рисоваться при большом сдвиге вправо
        while (pointForDistanceRect.x() < 0)
        {
            pointForDistanceRect.setX(pointForDistanceRect.x() + (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
        }

        //Чтобы виджет-справка мог рисоваться при большом сдвиге влево
        while (pointForDistanceRect.x() > (m_layerInfo.screenMiddle.x() * 2))
        {
            pointForDistanceRect.setX(pointForDistanceRect.x() - (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
        }

        m_distanceWidget->moveWidget(pointForDistanceRect);
        m_distanceWidget->setDistance(ortodromiaLenght);
    }
}

void CPlanimetrAndRulerLayer::duplicatePoint(QPainter& painter, const QPoint& point)
{
    QPoint pointToRight(point);
    while (pointToRight.x() < m_layerInfo.screenMiddle.x() * 2)
    {
        painter.drawPoint(pointToRight);
        pointToRight.setX(pointToRight.x() + (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
    }

    QPoint pointToLeft(point);
    while (pointToLeft.x() > 0)
    {
        painter.drawPoint(pointToLeft);
        pointToLeft.setX(pointToLeft.x() - (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
    }
}

void CPlanimetrAndRulerLayer::duplicateLine(QPainter& painter, QPoint prevPoint, QPoint curPoint)
{
    QPoint pointToRight;
    if (prevPoint.x() < curPoint.x())
        pointToRight = prevPoint;
    else
        pointToRight = curPoint;

    while (pointToRight.x() < m_layerInfo.screenMiddle.x() * 2)
    {
        painter.drawLine(prevPoint, curPoint);
        pointToRight.setX(pointToRight.x() + (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
        prevPoint.setX(prevPoint.x() + (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
        curPoint.setX(curPoint.x() + (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
    }

    QPoint pointToLeft;
    if (prevPoint.x() > curPoint.x())
        pointToLeft = prevPoint;
    else
        pointToLeft = curPoint;

    while (pointToLeft.x() > 0)
    {
        painter.drawLine(prevPoint, curPoint);
        pointToLeft.setX(pointToLeft.x() - (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
        prevPoint.setX(prevPoint.x() - (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
        curPoint.setX(curPoint.x() - (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE);
    }
}

void CPlanimetrAndRulerLayer::setLayerName(const QString& layerName)
{
    m_layerInfo.layerName = layerName;
}

void CPlanimetrAndRulerLayer::setLayerVisible(bool visibility)
{
    m_layerInfo.layerVisible = visibility;
    if (m_distanceWidget)
        m_distanceWidget->setVisible(visibility);
}

void CPlanimetrAndRulerLayer::resize(const QSize& size)
{
    m_layerInfo.screenMiddle = QPoint(size.width() / 2, size.height() / 2);
}

void CPlanimetrAndRulerLayer::setOfset(const QPoint& point)
{
    m_layerInfo.scroll = point;
}

void CPlanimetrAndRulerLayer::setZoom(const int& zoom)
{
    m_layerInfo.zoom = zoom;
}

void CPlanimetrAndRulerLayer::clicked(const System::coordTuple& coords)
{
    m_pointVec.push_back(coords);
}

void CPlanimetrAndRulerLayer::setDistanceWidget(QWidget* widget)
{
    CDistanceWidget* wgt = qobject_cast<CDistanceWidget*>(widget);
    if (wgt)
        m_distanceWidget = wgt;
}

CDistanceWidget::CDistanceWidget(QWidget* parent)
{
    m_widget = new QFrame(parent);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    m_distanceLabel = new QLabel(m_widget);
    horizontalLayout->addWidget(m_distanceLabel);

    m_trashBtn = new QPushButton(m_widget);

    horizontalLayout->addWidget(m_trashBtn);

    m_finishEditingBtn = new QPushButton(m_widget);
    horizontalLayout->addWidget(m_finishEditingBtn);

    m_deletePointsAndCloseBtn = new QPushButton(m_widget);
    horizontalLayout->addWidget(m_deletePointsAndCloseBtn);

    m_addPointsBtn = new QPushButton(m_widget);
    horizontalLayout->addWidget(m_addPointsBtn);

    m_widget->setLayout(horizontalLayout);
    m_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_widget->setAutoFillBackground(true);
    setVisible(false);
}

CDistanceWidget::~CDistanceWidget()
{

}

void CDistanceWidget::moveWidget(const QPoint& curPoint)
{
    m_widget->move(QPoint(curPoint.x() - m_widget->width() / 2, curPoint.y() - m_widget->height() - 10));
}

void CDistanceWidget::setDistance(const int& distance)
{
    m_distanceLabel->setText(QString::number(distance) + " км");
    m_widget->adjustSize();
}

void CDistanceWidget::setVisible(bool visibility)
{
    m_widget->setVisible(visibility);
}