#include "layer_manager.h"
#include "base_layer.h"

CLayerManager::CLayerManager()
{

}

CLayerManager::~CLayerManager()
{

}

void CLayerManager::repaint(QPainter& painter)
{
    for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
        //if ((*it)->m_layerInfo.layerVisible)
        (*it)->repaint(painter);
}

void CLayerManager::resize(const QSize& size)
{
    for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
        (*it)->resize(size);
}

void CLayerManager::setOfset(const QPoint& point)
{
    for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
        (*it)->setOfset(point);
}

void CLayerManager::setZoom(const int& zoom)
{
    for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
        (*it)->setZoom(zoom);
}

void CLayerManager::clicked(const System::coordTuple& coords)
{
    for (auto it = m_layers.begin(); it != m_layers.end(); ++it)
        if ((*it)->m_layerInfo.layerVisible)
            (*it)->clicked(coords);
}

void CLayerManager::addLayer(CBaseLayer* layer)
{
    m_layers.push_back(layer);
}