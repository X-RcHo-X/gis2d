#include "point_layer.h"
#include "base_source_functions.h"

//Qt
#include <QPainter>
#include <QGridLayout>

CPointLayer::CPointLayer()
{

}

CPointLayer::~CPointLayer()
{

}

void CPointLayer::repaint(QPainter& painter)
{
    if (!m_layerInfo.layerVisible)
        return;

    if (!m_pointDataWidget)
        return;

    if (m_isPointHidden)
    {
        m_pointDataWidget->setVisible(false);
        return;
    }

    painter.setPen(QPen(Qt::red, 10, Qt::SolidLine));
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::HighQualityAntialiasing, true);

    QPoint center = earth::getCenter(m_layerInfo.screenMiddle, m_layerInfo.scroll);
    QPoint startPoint = earth::getStartPoint(center, m_layerInfo.zoom);
    if (std::get<0>(m_currentPoint) && std::get<1>(m_currentPoint))
    {
        auto[pixelX, pixelY] = earth::getPixelsFromLatLon(*std::get<0>(m_currentPoint), *std::get<1>(m_currentPoint), m_layerInfo.zoom);

        //Чтобы виджет-справка мог рисоваться при большом сдвиге вправо
        while (pixelX + startPoint.x() < 0)
        {
            pixelX += (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE;
        }

        //Чтобы виджет-справка мог рисоваться при большом сдвиге влево
        while (pixelX + startPoint.x() > (m_layerInfo.screenMiddle.x() * 2))
        {
            pixelX -= (1 << m_layerInfo.zoom) * constant::earth::TILE_SIZE;
        }

        painter.drawPoint(QPoint(pixelX + startPoint.x(), pixelY + startPoint.y()));

        QString convertedLat;
        {

            int nDeg_ = 0, nMin_ = 0;
            float fSec_ = 0.f;
            bool bNe = false;
            earth::Deg2DMS(*std::get<0>(m_currentPoint), nDeg_, nMin_, fSec_, bNe);
            fSec_ = round(fSec_ * 100.0) / 100.0;
            convertedLat = QString("%0° %1' %2\"").arg(nDeg_ % 180).arg(nMin_).arg(fSec_);
            convertedLat.append(bNe ? " С.Ш." : " Ю.Ш.");
        }

        QString convertedLon;
        {
            int nDeg_ = 0, nMin_ = 0;
            float fSec_ = 0.f;
            bool bNe = false;
            earth::Deg2DMS(*std::get<1>(m_currentPoint), nDeg_, nMin_, fSec_, bNe);
            fSec_ = round(fSec_ * 100.0) / 100.0;
            convertedLon = QString("%0° %1' %2\"").arg(nDeg_ % 360).arg(nMin_).arg(fSec_);
            convertedLon.append(bNe ? " В.Д." : " З.Д.");
        }

        m_pointDataWidget->moveWidget(QPoint(pixelX + startPoint.x(), pixelY + startPoint.y()));
        m_pointDataWidget->setText(convertedLat + "\n" + convertedLon);
        m_pointDataWidget->setVisible(true);
    }
}

void CPointLayer::setPointDataWidget(QWidget* widget)
{
    CPointDataWidget* wgt = qobject_cast<CPointDataWidget*>(widget);
    if (wgt)
        m_pointDataWidget = wgt;
}

void CPointLayer::setLayerName(const QString& layerName)
{
    m_layerInfo.layerName = layerName;
}

void CPointLayer::setLayerVisible(bool visibility)
{
    m_layerInfo.layerVisible = visibility;
    if (m_pointDataWidget)
        m_pointDataWidget->setVisible(visibility);
    m_isPointHidden = true;
}

void CPointLayer::resize(const QSize& size)
{
    m_layerInfo.screenMiddle = QPoint(size.width() / 2, size.height() / 2);
    m_isPointHidden = true;
}

void CPointLayer::setOfset(const QPoint& point)
{
    m_layerInfo.scroll = point;
    m_isPointHidden = true;
}

void CPointLayer::setZoom(const int& zoom)
{
    m_layerInfo.zoom = zoom;
    m_isPointHidden = true;
}

void CPointLayer::clicked(const System::coordTuple& coords)
{
    m_currentPoint = coords;

    if (!std::get<0>(coords) && !std::get<1>(coords))
        m_isPointHidden = true;
    else
        m_isPointHidden = !m_isPointHidden;
}

CPointDataWidget::CPointDataWidget(QWidget* parent)
{
    m_widget = new QFrame(parent);
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    m_pointDataLabel = new QLabel(m_widget);
    horizontalLayout->addWidget(m_pointDataLabel);

    m_widget->setLayout(horizontalLayout);
    m_widget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_widget->setAutoFillBackground(true);
    setVisible(false);
}

CPointDataWidget::~CPointDataWidget()
{

}

void CPointDataWidget::moveWidget(const QPoint& curPoint)
{
    m_widget->move(QPoint(curPoint.x() - m_widget->width() / 2, curPoint.y() - m_widget->height() - 10));
}

void CPointDataWidget::setText(const QString& coordText)
{
    m_pointDataLabel->setText(coordText);
    m_widget->adjustSize();
}

void CPointDataWidget::setVisible(bool visibility)
{
    m_widget->setVisible(visibility);
}

