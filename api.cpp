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
        reply = manager->post(request, QByteArray("{'hello': 'world'}"));
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
