#pragma once

#include <QByteArray>
#include <QString>

namespace BSplineRenderer
{
    class Util
    {
      public:
        Util() = delete;

        static QByteArray GetBytes(const QString& path);
    };
}
