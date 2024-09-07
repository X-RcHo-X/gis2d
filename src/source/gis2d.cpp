#include "gis2d.h"
#include "layer_manager.h"
#include "base_source_functions.h"

//Qt
#include <QPainter>

CGis2D::CGis2D(QWidget* parent) :
    m_layerManager(new CLayerManager())
{

}

CGis2D::~CGis2D()
{

}

void CGis2D::addLayer(CBaseLayer* layer)
{
    m_layerManager->addLayer(layer);
}

void CGis2D::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    m_layerManager->repaint(painter);
    painter.end();
}

void CGis2D::mouseMoveEvent(QMouseEvent* event)
{
    if (m_isPresed)
    {
        m_isMoved = true;
        QPoint del;
        del = event->pos() - m_currentPosition;
        m_currentPosition = event->pos();
        m_scroll += del;
        m_layerManager->setOfset(m_scroll);
        update();
    }
}

void CGis2D::mousePressEvent(QMouseEvent* event)
{
    m_isPresed = true;
    m_currentPosition = event->pos();
}

void CGis2D::mouseReleaseEvent(QMouseEvent* event)
{
    m_isPresed = false;

    if (!m_isMoved)
    {
        QPoint startPoint = earth::getStartPointRelativeCenter(m_screenMiddle, m_scroll, m_zoom);

        int pixelX = event->pos().x() - startPoint.x();
        int pixelY = event->pos().y() - startPoint.y();

        System::coordTuple coords;
        if (pixelY > 0 && pixelY < (1 << m_zoom) * constant::earth::TILE_SIZE)
        {
            coords = earth::getLatLonFromPixel(pixelX, pixelY, m_zoom);
        }

        m_layerManager->clicked(coords);
    }
    m_isMoved = false;
    update();
}

void CGis2D::wheelEvent(QWheelEvent* event)
{
    bool isPositive = event->angleDelta().y() > 0;

    //Получение пиксельной координаты левой верхней точки начала видимой карты
    QPoint curZoomPixelPoint = earth::getStartPointRelativeCenter(m_screenMiddle, m_scroll, m_zoom);
    //Получение текущих курсорных координат
    QPoint curMousePos = mapFromGlobal(QCursor::pos());
    //Считаем пиксели курсорных координат отностильно стартовой точки
    int curPixelX = curMousePos.x() - curZoomPixelPoint.x();
    int curPixelY = curMousePos.y() - curZoomPixelPoint.y();

    System::coordTuple curCoords;
    curCoords = earth::getLatLonFromPixel(curPixelX, curPixelY, m_zoom);

    if (isPositive)
        m_zoom += 1;
    else if (m_zoom >= 1)
        m_zoom -= 1;

    //Получение пиксельной координаты левой верхней точки начала видимой карты после скрола
    QPoint newZoomPixelPoint = earth::getStartPointRelativeCenter(m_screenMiddle, m_scroll, m_zoom);
    //Пересчитываем пиксели курсорных координат отностильно новой стартовой точки
    int newPixelX = curMousePos.x() - newZoomPixelPoint.x();
    int newPixelY = curMousePos.y() - newZoomPixelPoint.y();

    //Перевеод текущих географических координат в пиксели на новом зуме
    System::coordTuple newPixel;
    newPixel = earth::getPixelsFromLatLon(*std::get<0>(curCoords), *std::get<1>(curCoords), m_zoom);

    //Сдвигаем географические пиксели под курсор
    m_scroll += QPoint(newPixelX - *std::get<0>(newPixel), newPixelY - *std::get<1>(newPixel));
    m_layerManager->setOfset(m_scroll);

    m_layerManager->setZoom(m_zoom);
    update();
}

void CGis2D::resizeEvent(QResizeEvent* event)
{
    m_layerManager->resize(QSize(width(), height()));
    m_screenMiddle = QPoint(width() / 2, height() / 2);
}

void CGis2D::showEvent(QShowEvent* event)
{
    m_layerManager->resize(QSize(width(), height()));
    m_screenMiddle = QPoint(width() / 2, height() / 2);
}
