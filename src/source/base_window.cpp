#include "base_window.h"
#include "ui_base_window.h"
#include "gis2d.h"
#include "tile_layer.h"
#include "planimetr_and_ruler_layer.h"
#include "point_layer.h"

//Qt
#include <QRadioButton>

CBaseWindow2D::CBaseWindow2D(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::BaseWindow),
    m_gis2d(new CGis2D(this))
{
    ui->setupUi(this);
    m_gis2d->installEventFilter(this);

    if (m_gis2d)
    {
        m_rulerBtn = new QRadioButton(this);


        CTileLayer* tileLayer = new CTileLayer();
        CPlanimetrAndRulerLayer* planAndRulerLayer = new CPlanimetrAndRulerLayer();
        CPointLayer* pointLayer = new CPointLayer();
        if (tileLayer)
            m_gis2d->addLayer(tileLayer);
        if (planAndRulerLayer)
        {
            m_gis2d->addLayer(planAndRulerLayer);
            CDistanceWidget* distanceWidget = new CDistanceWidget(m_gis2d);
            planAndRulerLayer->setDistanceWidget(distanceWidget);
            planAndRulerLayer->setLayerVisible(false);
        }
        if (pointLayer)
        {
            m_gis2d->addLayer(pointLayer);
            CPointDataWidget* pointDataWidget = new CPointDataWidget(m_gis2d);
            pointLayer->setPointDataWidget(pointDataWidget);
            pointLayer->setLayerVisible(true);
        }
        ui->baseScene->layout()->addWidget(m_gis2d);

        connect(m_rulerBtn, &QRadioButton::toggled, this, [ = ](bool visibility)
        {
            pointLayer->setLayerVisible(!visibility);
            planAndRulerLayer->setLayerVisible(visibility);
            m_gis2d->update();

        });
    }
    updateBtnRulerSize();
}

CBaseWindow2D::~CBaseWindow2D()
{
    delete ui;
}

bool CBaseWindow2D::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == m_gis2d)
    {
        switch (event->type())
        {
            case QEvent::Resize:
            {
                updateBtnRulerSize();
            }
            break;
            default:
                break;
        }
        return false;
    }
    return QWidget::eventFilter(watched, event);
}

void CBaseWindow2D::updateBtnRulerSize()
{
    const int MINIMUM_MARGIN_PX = 10; //Минимальный отступ от края
    //Берем размер родителя с учетом отступа
    const QRect parentRect = rect().marginsRemoved(
                                 QMargins(MINIMUM_MARGIN_PX,
                                          MINIMUM_MARGIN_PX,
                                          MINIMUM_MARGIN_PX,
                                          MINIMUM_MARGIN_PX));
    QPoint position;
    position.setX(parentRect.right() - m_rulerBtn->width() - 5);
    position.setY(parentRect.top() + 5);

    const QRect thisRect(position, QSize(20, m_rulerBtn->height()));
    m_rulerBtn->setGeometry(thisRect);
}