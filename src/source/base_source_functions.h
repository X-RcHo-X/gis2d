#ifndef BASE_SOURCE_FUNCTIONS_H
#define BASE_SOURCE_FUNCTIONS_H

#include "constants.h"

//Qt
#include <QPoint>

namespace earth
{
    //--------------------------------------------------------------------------
    ///Получение центра экрана
    inline QPoint getCenter(QPoint screenMiddle, QPoint srcoll)
    {
        return QPoint(screenMiddle + srcoll);
    }

    //--------------------------------------------------------------------------
    ///Получение левой верхней точки начала видимой карты
    inline QPoint getStartPoint(QPoint center, int zoom)
    {
        return QPoint(center.x() - (1 << zoom) / 2 * constant::earth::TILE_SIZE, center.y() - (1 << zoom) / 2 * constant::earth::TILE_SIZE);
    }

    //--------------------------------------------------------------------------
    ///Получение стартовой точки относительно центра экрана
    inline QPoint getStartPointRelativeCenter(QPoint screenMiddle, QPoint srcoll, int zoom)
    {
        QPoint center = getCenter(screenMiddle, srcoll);
        QPoint startPoint = earth::getStartPoint(center, zoom);

        return startPoint;
    }

    //--------------------------------------------------------------------------
    ///Получение долготы из радиан в градусы
    inline double normalizeLongitudeDeg(double longitudeRad)
    {
        longitudeRad = qDegreesToRadians(longitudeRad);
        qreal halfCircle = M_PI;

        if (longitudeRad > halfCircle)
        {
            int cycles = (int)((longitudeRad + halfCircle) / (2 * halfCircle));
            return qRadiansToDegrees(longitudeRad - (cycles * 2 * halfCircle));
        }
        if (longitudeRad < -halfCircle)
        {
            int cycles = (int)((longitudeRad - halfCircle) / (2 * halfCircle));
            return qRadiansToDegrees(longitudeRad - (cycles * 2 * halfCircle));
        }

        return qRadiansToDegrees(longitudeRad);
    }

    //--------------------------------------------------------------------------
    ///Получение широты из радиан в градусы
    inline double normalizeLatitudeDeg(double latitudeRad)
    {
        latitudeRad = qDegreesToRadians(latitudeRad);
        qreal halfCircle = M_PI;

        if (latitudeRad > (halfCircle / 2.0))
        {
            int cycles = (int)((latitudeRad + halfCircle) / (2 * halfCircle));
            qreal temp;
            if (cycles == 0)    // pi/2 < lat < pi
            {
                temp = halfCircle - latitudeRad;
            }
            else
            {
                temp = latitudeRad - (cycles * 2 * halfCircle);
            }
            if (temp > (halfCircle / 2.0))
            {
                return qRadiansToDegrees((halfCircle - temp));
            }
            if (temp < (-halfCircle / 2.0))
            {
                return qRadiansToDegrees((-halfCircle - temp));
            }
            return qRadiansToDegrees(temp);
        }
        if (latitudeRad < (-halfCircle / 2.0))
        {
            int cycles = (int)((latitudeRad - halfCircle) / (2 * halfCircle));
            qreal temp;
            if (cycles == 0)
            {
                temp = -halfCircle - latitudeRad;
            }
            else
            {
                temp = latitudeRad - (cycles * 2 * halfCircle);
            }
            if (temp > (+halfCircle / 2.0))
            {
                return qRadiansToDegrees((+halfCircle - temp));
            }
            if (temp < (-halfCircle / 2.0))
            {
                return qRadiansToDegrees((-halfCircle - temp));
            }
            return qRadiansToDegrees(temp);
        }
        return qRadiansToDegrees(latitudeRad);
    }

    //--------------------------------------------------------------------------
    ///Получение метров из экранных координат
    inline std::tuple<double, double> getMetersFromPixel(int pixelX, int pixelY, int zoom)
    {
        //Кол-во метров в 1 тайле
        auto res = constant::earth::INITIAL_RESOLUTION / (1 << zoom);

        double meterX = abs(pixelX * res - constant::earth::ORIGIN_SHIFT);
        double meterY = abs(pixelY * res - constant::earth::ORIGIN_SHIFT);

        //Половина пикселей на текущем уровне детализации
        auto halfSize = constant::earth::TILE_SIZE * (1 << zoom) / 2;

        //Если пиксель в западном полушарии, то долгота отрицательная
        if (pixelX < halfSize)
            meterX *= -1;

        //Если пиксель в южном полушарии, то широта отрицательная
        if (pixelY > halfSize)
            meterY *= -1;

        return { meterX, meterY };
    }

    //--------------------------------------------------------------------------
    ///Получение широты и долготы из метров
    inline std::tuple<double, double> getLatLonFromMeters(double meterX, double meterY)
    {
        double longitudeDeg = (meterX / constant::earth::ORIGIN_SHIFT) * 180.0;

        double latitudeRad = (meterY / constant::earth::ORIGIN_SHIFT) * 180.0;
        double latitudeDeg = 180 / M_PI * (2 * atan(exp(latitudeRad * M_PI / 180.0)) - M_PI / 2.0);

        longitudeDeg = normalizeLongitudeDeg(longitudeDeg);
        latitudeDeg =  normalizeLatitudeDeg(latitudeDeg);

        return { latitudeDeg, longitudeDeg };
    }

    //--------------------------------------------------------------------------
    ///Получение широты и долготы из экранных координат
    inline std::tuple<double, double> getLatLonFromPixel(int pixelX, int pixelY, int zoom)
    {
        auto [meterX, meterY] = getMetersFromPixel(pixelX, pixelY, zoom);

        //std::tuple<double, double> meters = getMetersFromPixel(pixelX, pixelY, zoom);

        return getLatLonFromMeters(meterX, meterY);
    }

    //--------------------------------------------------------------------------
    ///Получение пикселей из метров
    inline std::tuple<int, int> getsPixelsFromMeters(double meterX, double meterY, int zoom)
    {
        double res = constant::earth::INITIAL_RESOLUTION / (1 << zoom);
        auto pixelX = (meterX + constant::earth::ORIGIN_SHIFT) / res;
        auto pixelY = (meterY - constant::earth::ORIGIN_SHIFT) / res;

        return { std::abs(std::round(pixelX)), std::abs(std::round(pixelY)) };
    }

    //--------------------------------------------------------------------------
    ///Получение метров из широты и долготы
    inline std::tuple<double, double> getMetersFromLatLon(double latitude, double longitude)
    {
        double meterX = longitude * constant::earth::ORIGIN_SHIFT / 180.0;

        double meterY = std::log(std::tan((90.0 + latitude) * M_PI / 360.0)) / (M_PI / 180.0);
        meterY = meterY * constant::earth::ORIGIN_SHIFT / 180.0;

        return {meterX, meterY};
    }

    //--------------------------------------------------------------------------
    ///Получение экранных координат из широты и долготы
    inline std::tuple<double, double> getPixelsFromLatLon(double latitude, double longitude, int zoom)
    {
        auto [meterX, meterY] = getMetersFromLatLon(latitude, longitude);
        return getsPixelsFromMeters(meterX, meterY, zoom);
    }

    //--------------------------------------------------------------------------
    ///Математическое округление градусов, минут и секунд
    inline void RoundDMS(int& nDeg_, int& nMin_, float& nSec_, bool& bNe_)
    {
        if (nSec_ >= 60)
        {
            //Если округлились до 60 значит зануляем секунды и +1 к минутам
            nMin_ = nMin_ + (int)nSec_ / 60;
            nSec_ = nSec_ - (int)(nSec_ / 60) * 60;
            if (nMin_ >= 60)
            {
                //Если округлились до 60 значит зануляем минут и +1 к градусам
                nDeg_ = nDeg_ + nMin_ / 60;
                nMin_ = nMin_ % 60;
                if (nDeg_ >= 180)
                {
                    //Если округлились до 180 значит зануляем градусы и переходим в другую зону
                    nDeg_ = nDeg_ % 180;
                    bNe_ = !bNe_;
                }
            }
        }
    }

    //--------------------------------------------------------------------------
    ///Преобразование градусов в градусы, минуты, секунды и признак
    inline void Deg2DMS(double r_, int& nDeg_, int& nMin_, float& fSec_, bool& bNE_)
    {
        double dGrads = fabs(r_);
        nDeg_ = static_cast<int>(dGrads);
        double dMins = (dGrads - nDeg_) * 60.0;
        nMin_ = static_cast<int>(dMins);
        fSec_ = static_cast<float>((dMins - nMin_) * 60);
        bNE_ = r_ >= 0;
        //На всякий случай проверим
        RoundDMS(nDeg_, nMin_, fSec_, bNE_);
    }
}

#endif //BASE_SOURCE_FUNCTIONS_H