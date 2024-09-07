#ifndef BASE_WINDOW_H
#define BASE_WINDOW_H

//Qt
#include <QMainWindow>

class QRadioButton;
class CGis2D;

namespace Ui
{
    class BaseWindow;
}

class CBaseWindow2D : public QMainWindow
{
    Q_OBJECT

public:
    explicit CBaseWindow2D(QWidget* parent = 0);
    ~CBaseWindow2D();

    CGis2D* m_gis2d = nullptr;

private:
    Ui::BaseWindow* ui;

    QRadioButton* m_rulerBtn = nullptr;  //Кнопка управления линейкой и планиметром

    void updateBtnRulerSize();

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif //BASE_WINDOW_H