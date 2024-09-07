#ifndef POINT_LAYER_H
#define POINT_LAYER_H

#include "base_layer.h"

//Qt
#include <QLabel>

class CPointDataWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CPointDataWidget(QWidget* parent = 0);
    ~CPointDataWidget();

    void moveWidget(const QPoint& curPoint);
    void setText(const QString& coordText);

    void setVisible(bool visibility) override;

private:
    QFrame* m_widget = nullptr;
    QLabel* m_pointDataLabel = nullptr;
};

class CPointLayer : public CBaseLayer
{
public:
    explicit CPointLayer();
    ~CPointLayer();

    void repaint(QPainter& painter) override;
    void resize(const QSize& size) override;
    void setOfset(const QPoint& point) override;
    void setZoom(const int& zoom) override;
    void clicked(const System::coordTuple& coords) override;
    void setLayerName(const QString& layerName) override;
    void setLayerVisible(bool visibility) override;

    void setPointDataWidget(QWidget* widget);

private:
    bool m_isPointHidden = true;  //Булик для отслеживания видимости точки

    CPointDataWidget* m_pointDataWidget = nullptr;
    System::coordTuple /*<latitude, longitude>*/ m_currentPoint;
};

#endif //POINT_LAYER_H