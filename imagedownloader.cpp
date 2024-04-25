#include "imagedownloader.h"
#include <QDebug>

ImageDownloader::ImageDownloader() { }

ImageDownloader::~ImageDownloader() { }


/**
 * @brief ImageDownloader::getRandomImageData
 * 获取随机的图片二进制数据
 * 1、首先调用bing接口获取随机一张图片的地址
 * 2、通过http请求获取二进制数组，并返回
 * @return
 */
QByteArray ImageDownloader::getRandomImageData(int screenId)
{
    QString imageUrl = getRandomImageUrl(screenId);
    //生成对应的网络请求
    QNetworkRequest request;
    request.setUrl(QUrl(imageUrl));

    //发送Get请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    //对请求的返回异常进行处理
    if(reply->error() != QNetworkReply::NoError)
    {
       return "error";
    }

    //解析返回的Json结果
    QByteArray replyData = reply->readAll();
    return replyData;
}


/**
 * @brief ImageDownloader::getRandomImageUrl
 * 随机获取一张图片地址
 * @return
 */
QString ImageDownloader::getRandomImageUrl(int screenId)
{
    srand(time(0));
    int offset = rand() % 7 + screenId;
    QString url = QString("https://cn.bing.com/HPImageArchive.aspx?format=js&idx=%1&n=8&mkt=zh-CN").arg(offset);
    //生成对应的网络请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));

    //发送Get请求
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(request);
    QEventLoop eventLoop;
    QObject::connect(reply, SIGNAL(finished()),&eventLoop, SLOT(quit()));
    eventLoop.exec(QEventLoop::ExcludeUserInputEvents);

    //对请求的返回异常进行处理
    if(reply->error() != QNetworkReply::NoError)
    {
       return "error";
    }

    //解析返回的Json结果
    QByteArray replyData = reply->readAll();
    QJsonParseError json_error;
    QJsonDocument  root_document = QJsonDocument::fromJson(replyData, &json_error);
    if(json_error.error != QJsonParseError::NoError)
    {
       return "error";
    }
    QJsonObject root_object = root_document.object();
    QJsonArray image_list_ptr = root_object.find("images").value().toArray();
    // 随机取一个值
    int r = rand() % image_list_ptr.size();

    qDebug("image_index:%d-%d", r, offset);
    QJsonObject image_object = image_list_ptr.at(r).toObject();
    QString image_url = image_object.find("url").value().toString();
    image_url = "https://cn.bing.com" + image_url;
    qDebug() << "image:" + image_url;
    return image_url;
}

