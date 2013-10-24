#include "thumbsvideos.h"
#include <QDebug>

Q_EXPORT_PLUGIN2(thumbsvideos, ThumbsVideos)

void
ThumbsVideos::init()
{
    m_vt.setThumbnailSize(256);
}

QImage
ThumbsVideos::thumb(const QString &file, const int size)
{
    try
    {
        std::vector<uint8_t> pixels;
        m_vt.generateThumbnail(file.toStdString(), Png, pixels);
        QImage img;
        if ( img.loadFromData(pixels.data(), pixels.size(), "PNG") )
            return img;
    }
    catch ( std::exception &e )
    {
        qDebug() << e.what();
    }

    return QImage();
}

#define END(_SUFFIX_) file.endsWith(_SUFFIX_, Qt::CaseInsensitive)

bool
ThumbsVideos::canRead(const QString &file) const
{
    return END("mkv")||END("avi")||END("mp4")||END("mpg");
}

#undef END
