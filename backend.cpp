#include "backend.h"

BackEnd::BackEnd(QString url, QString filialId, QObject *parent) : QObject(parent), _baseUrl(url), _filialId(filialId)
{

    _manager = new QNetworkAccessManager();

    connect(_manager, &QNetworkAccessManager::finished,
            this, [=](QNetworkReply *reply) {

        if (reply->error()) {
            qDebug() << "Error" << reply->errorString();
            return;
        }

        QString answer = reply->readAll();

        qDebug() << "response at callback" << answer;
    });

}
//void BackEnd::downloadImage()
//{
//    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//    connect(manager, &QNetworkAccessManager::finished, this, &BackEnd::downloadFinished);
//    const QUrl url = QUrl(this->_imageUrl);
//    QNetworkRequest request(url);
//    manager->get(request);
//}

//void BackEnd::downloadFinished(QNetworkReply *reply)
//{
//    if (reply->error())
//    {
//        qDebug() << "Error at request" << reply->error();
//    } else {
//        QPixmap pixmap;
//        pixmap.loadFromData(reply->readAll());
//        qDebug() << "Pic" << pixmap;
//    }

//}

PizzaList BackEnd::getPizzaList()
{
//    const QUrl url = QUrl(this->_url + "/api/PizzaLineWebAPI/GetApproximateTimeWait");
//    _request = new QNetworkRequest(url);
//    _request->setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");
}

void BackEnd::getApproximateTimeWait()
{
    const QUrl url = QUrl(this->_baseUrl + "/api/PizzaLineWebAPI/GetApproximateTimeWait?filialId=" + this->_filialId);
    qDebug() << url;

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::KnownHeaders::ContentTypeHeader, "application/json");

    auto response = _manager->get(request);

    qDebug() << "autores" << response->readAll();
}
