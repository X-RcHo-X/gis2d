#ifndef GIS2D_H
#define GIS2D_H

//Qt
#include <QWidget>
#include <qevent.h>

class CLayerManager;
class CBaseLayer;

class CGis2D : public QWidget
{
    Q_OBJECT

public:
    explicit CGis2D(QWidget* parent = 0);
    ~CGis2D();

    void addLayer(CBaseLayer* layer);

private:
    int  m_zoom = 1;
    bool m_isPresed = false;
    bool m_isMoved = false;

    QPoint m_currentPosition;
    QPoint m_screenMiddle;
    QPoint m_scroll;

    CLayerManager* m_layerManager = nullptr;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void showEvent(QShowEvent* event) override;
};

#endif //GIS2D_H
