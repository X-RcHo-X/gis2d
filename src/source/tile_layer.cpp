#include "tile_layer.h"
#include "base_source_functions.h"

//Qt
#include <QPixmapCache>

CTileLayer::CTileLayer()
{

}

CTileLayer::~CTileLayer()
{

}

int CTileLayer::tilesX(const int& zoom)
{
    return pow(2, zoom);
}

int CTileLayer::tilesY(const int& zoom)
{
    return pow(2, zoom);
}

void CTileLayer::repaint(QPainter& painter)
{
    painter.setPen(QPen(Qt::red, 1, Qt::SolidLine));

    int tileX = tilesX(m_layerInfo.zoom);
    int tileY = tilesY(m_layerInfo.zoom);

    QPoint center = earth::getCenter(m_layerInfo.screenMiddle, m_layerInfo.scroll);
    QPoint startPoint = earth::getStartPoint(center, m_layerInfo.zoom);

    int iStart;
    if (startPoint.x() < 0)
        iStart = -startPoint.x() / 256;
    else
        iStart = -startPoint.x() / 256 - 1;

    int jStart = 0;

    if (startPoint.y() < 0)
        jStart = abs(startPoint.y() / 256);

    for (int i = iStart; i <= iStart + m_visibleTileX; ++i)
    {
        for (int j = jStart; j <= jStart + m_visibleTileY && j < tileY; ++j)
        {
            QPoint tileStartPoint = QPoint(startPoint.x() + constant::earth::TILE_SIZE * i, startPoint.y() + constant::earth::TILE_SIZE * j);
            QPoint tileEndPoint = QPoint(startPoint.x() + constant::earth::TILE_SIZE * (i + 1), startPoint.y() + constant::earth::TILE_SIZE * (j + 1));

            int n = i;
            if (i < 0)
                n = tileX - abs(i) % tileX;

            QPixmap img;
            QString key = QString::number(m_layerInfo.zoom) + "_" + QString::number(n % tileX) + "_" + QString::number(j);
            if (!QPixmapCache::find(key, &img))
            {
                img = QPixmap(QString("D:\\EmptyMapStub\\%1\\%2\\%3.png").arg(m_layerInfo.zoom).arg(n % tileX).arg(j));
                QPixmapCache::insert(key, img);
                painter.drawImage(tileStartPoint, img.toImage());
            }
            else
                painter.drawImage(tileStartPoint, img.toImage());

            painter.drawText(QRect(tileStartPoint, tileEndPoint), Qt::AlignHCenter | Qt::AlignVCenter,
                             QString("x = %1, y = %2, z = %3").arg(n % tileX).arg(j).arg(m_layerInfo.zoom));

            painter.drawRect(QRect(tileStartPoint, tileEndPoint));
        }
    }
}

void CTileLayer::resize(const QSize& size)
{
    m_layerInfo.screenMiddle = QPoint(size.width() / 2, size.height() / 2);
    m_visibleTileX = size.width() / constant::earth::TILE_SIZE + 1;
    m_visibleTileY = size.height() / constant::earth::TILE_SIZE + 1;
}

void CTileLayer::setOfset(const QPoint& point)
{
    m_layerInfo.scroll = point;
}

void CTileLayer::setZoom(const int& zoom)
{
    m_layerInfo.zoom = zoom;
}

void CTileLayer::setLayerName(const QString& layerName)
{
    m_layerInfo.layerName = layerName;
}

void CTileLayer::setLayerVisible(bool visibility)
{
    m_layerInfo.layerVisible = visibility;
}
