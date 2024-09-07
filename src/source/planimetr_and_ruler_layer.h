#ifndef PLANIMETR_AND_RULER_LAYER_H
#define PLANIMETR_AND_RULER_LAYER_H

#include "base_layer.h"

//Qt
#include <QLabel>
#include <QPushButton>

class CDistanceWidget : public QFrame
{
    Q_OBJECT

public:
    explicit CDistanceWidget(QWidget* parent = 0);
    ~CDistanceWidget();

    void moveWidget(const QPoint& curPoint);
    void setDistance(const int& distance);

    void setVisible(bool visibility) override;

private:
    QFrame* m_widget = nullptr;
    QLabel* m_distanceLabel = nullptr;
    QPushButton* m_trashBtn = nullptr;                  //Удалить точки
    QPushButton* m_finishEditingBtn = nullptr;          //Завершить редактирование
    QPushButton* m_deletePointsAndCloseBtn = nullptr;   //Удалить точки и закрыть
    QPushButton* m_addPointsBtn = nullptr;              //Добавить точки
};

class CPlanimetrAndRulerLayer : public CBaseLayer
{
public:
    explicit CPlanimetrAndRulerLayer();
    ~CPlanimetrAndRulerLayer();

    void repaint(QPainter& painter) override;
    void resize(const QSize& size) override;
    void setOfset(const QPoint& point) override;
    void setZoom(const int& zoom) override;
    void clicked(const System::coordTuple& coords) override;
    void setLayerName(const QString& layerName) override;
    void setLayerVisible(bool visibility) override;

    void setDistanceWidget(QWidget* widget);
    void duplicatePoint(QPainter& painter, const QPoint& point);
    void duplicateLine(QPainter& painter, QPoint point, QPoint curPoint);

private:
    QVector<System::coordTuple /*<latitude, longitude>*/> m_pointVec;
    CDistanceWidget* m_distanceWidget = nullptr;
};

#endif //PLANIMETR_AND_RULER_LAYER_H