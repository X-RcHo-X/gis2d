#ifndef BASE_LAYER_H
#define BASE_LAYER_H

#include "user_data_types.h"

//Qt
#include <QPainter>
#include <QSize>
#include <QPoint>

class CBaseLayer
{
public:
    virtual ~CBaseLayer() {}

    virtual void repaint(QPainter& painter)                    = 0;   //Перерисовка всего на слое
    virtual void resize(const QSize& size)                     = 0;   //Изменение размера окна
    virtual void setOfset(const QPoint& point)                 = 0;   //Установка сдвига карты после MouseMoveEvent
    virtual void setZoom(const int& zoom)                      = 0;   //Установка уровня детализации карты
    virtual void setLayerName(const QString& layerName)        = 0;   //Установка имени слоя
    virtual void setLayerVisible(bool visibility)              = 0;   //Установка видимости слоя

    virtual void clicked(const System::coordTuple& coords) {}   //Нажатие мышкой на карте

    //Общая информация о создаваемом слое
    struct SLayerInfo
    {
        QPoint currentPosition;     //Текущее нажатие мыши
        QPoint screenMiddle;        //Центр экрана относительно которого производятся расчёты
        QPoint scroll;              //Точка для отслеживания MouseMoveEvent
        QString layerName;          //Имя слоя
        int zoom = 1;               //Уровень детализации карты
        bool layerVisible = true;   //Видимость уровня
    };
    SLayerInfo m_layerInfo;
};

#endif //BASE_LAYER_H
