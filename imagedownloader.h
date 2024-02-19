#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H

#include <QObject>
#include <QByteArray>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>

/**
 * @brief The FileDownloader class
 * 1、利用接口随机获取一张bing壁纸信息
 * 2、通过http请求获取该壁纸的bytearray
 */

class ImageDownloader : public QObject
{
public:
    explicit ImageDownloader();
    virtual ~ImageDownloader();
    QByteArray getRandomImageData();

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;

    QString getRandomImageUrl();
};

#endif // IMAGEDOWNLOADER_H
