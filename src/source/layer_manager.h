#ifndef LAYER_MANAGER_H
#define LAYER_MANAGER_H

#include "user_data_types.h"

//Qt
#include <QPainter>

class CBaseLayer;

class CLayerManager
{

public:
    explicit CLayerManager();
    ~CLayerManager();

    void repaint(QPainter& painter);
    void resize(const QSize& size);
    void setOfset(const QPoint& point);
    void setZoom(const int& zoom);
    void addLayer(CBaseLayer* layer);
    void clicked(const System::coordTuple& coords);

private:
    QList<CBaseLayer*> m_layers;  //Список всех слоёв
};

#endif //LAYER_MANAGER_H