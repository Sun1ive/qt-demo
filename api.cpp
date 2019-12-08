#include "api.h"

Api::Api(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
}

void Api::sendRequest(const QString &apiStr,
                      const handleFunc &funcSuccess,
                      const handleFunc &funcError,
                      Api::Type type,
                      const QVariantMap &data)
{
    QNetworkRequest request = createRequest(apiStr);

    QNetworkReply *reply;

    switch (type) {
    case Type::POST: {
        QByteArray postDataByteArray = variantMapToJson(data);
        reply = manager->post(request, postDataByteArray);

        break;
    } case Type::GET: {
        reply = manager->get(request);

        break;
    } default:
        reply = nullptr;
        Q_ASSERT(false);
    }

    connect(reply, &QNetworkReply::finished, this,
            [this, funcSuccess, funcError, reply]() {

        QJsonObject obj = parseReply(reply);

        if (onFinishRequest(reply)) {
            if (funcSuccess != nullptr)
                funcSuccess(obj);
        } else {
            if (funcError != nullptr) {
                handleQtNetworkErrors(reply, obj);
                funcError(obj);
            }
        }

        reply->close();
        reply->deleteLater();
    } );
}


QByteArray Api::variantMapToJson(QVariantMap data)
{
    QJsonDocument postDataDoc = QJsonDocument::fromVariant(data);
    QByteArray postDataByteArray = postDataDoc.toJson();

    return postDataByteArray;
}

QNetworkRequest Api::createRequest(const QString &apiStr)
{
    QNetworkRequest request;
    request.setUrl(QUrl(apiStr));
    request.setRawHeader("Content-Type","application/json");

    return request;
}

QJsonObject Api::parseReply(QNetworkReply *reply)
{
    QJsonObject jsonObj;
    QJsonDocument jsonDoc;
    QJsonParseError parseError;

    auto replyText = reply->readAll();

    jsonDoc = QJsonDocument::fromJson(replyText, &parseError);

    if(parseError.error != QJsonParseError::NoError){
        qDebug() << replyText;
        qWarning() << "Json parse error: " << parseError.errorString();
    }else{
        if(jsonDoc.isObject())
            jsonObj  = jsonDoc.object();
        else if (jsonDoc.isArray())
            jsonObj["non_field_errors"] = jsonDoc.array();
    }
    return jsonObj;
}

bool Api::onFinishRequest(QNetworkReply *reply)
{
    auto replyError = reply->error();

    if (replyError == QNetworkReply::NoError ) {
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if ((code >=200) && (code < 300)) {
            return true;
        }
    }

    return false;
}

void Api::handleQtNetworkErrors(QNetworkReply *reply, QJsonObject &obj)
{
    auto replyError = reply->error();
    if (!(replyError == QNetworkReply::NoError ||
          replyError == QNetworkReply::ContentNotFoundError ||
          replyError == QNetworkReply::ContentAccessDenied ||
          replyError == QNetworkReply::ProtocolInvalidOperationError
          ) ) {
        qDebug() << reply->error();

        obj[KEY_QNETWORK_REPLY_ERROR] = reply->errorString();
    } else if (replyError == QNetworkReply::ContentNotFoundError) {
        obj[KEY_CONTENT_NOT_FOUND] = reply->errorString();
    }
}

