#ifndef TILE_LAYER_H
#define TILE_LAYER_H

#include "base_layer.h"

class CTileLayer : public CBaseLayer
{
public:
    explicit CTileLayer();
    ~CTileLayer();

    void repaint(QPainter& painter) override;
    void resize(const QSize& size) override;
    void setOfset(const QPoint& point) override;
    void setZoom(const int& zoom) override;
    void setLayerName(const QString& layerName) override;
    void setLayerVisible(bool visibility) override;

private:
    int tilesX(const int& zoom);
    int tilesY(const int& zoom);

    int m_visibleTileX;
    int m_visibleTileY;
};

#endif //TILE_LAYER_H
