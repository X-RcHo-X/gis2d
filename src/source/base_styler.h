#ifndef BASE_STYLER_H
#define BASE_STYLER_H

//Qt
#include <QApplication>
#include <QDir>

class BaseStyler
{
public:
    static void setStyleSheet(const QString& filePath)
    {
        QDir directory(filePath);
        QStringList files = directory.entryList(QStringList() << "*.qss" << "*.QSS", QDir::Files);
        QString styleSheet;
        for (auto file : files)
        {
            QFile qss(filePath + "/" + file);
            if (qss.open(QFile::ReadOnly))
            {
                styleSheet += QLatin1String(qss.readAll());
            }
        }
        qApp->setStyleSheet(styleSheet);
    }
};

#endif //BASE_STYLER_H
