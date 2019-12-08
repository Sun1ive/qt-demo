#ifndef API_H
#define API_H

#include <functional>
#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonDocument>


class Api : public QObject
{
    Q_OBJECT
public:
    typedef std::function<void(const QJsonObject &)> handleFunc;
    typedef std::function<void()> finishFunc;

    static const QString KEY_QNETWORK_REPLY_ERROR;
    static const QString KEY_CONTENT_NOT_FOUND;

    enum class Type {
        POST,
        GET,
    };

    explicit Api(const QString url, QObject *parent = nullptr);

    void sendRequest(
            const QString &apiStr,
            const handleFunc &funcSuccess,
            const handleFunc &funcError,
            Api::Type type,
            const QVariantMap &data
            );

private slots:
    QNetworkRequest createRequest(const QString &apiStr);

    QNetworkReply *sendCustomRequest(
        QNetworkAccessManager *manager,
        QNetworkRequest &request,
        const QString &type,
        const QVariantMap &data
    );

    QByteArray variantMapToJson(QVariantMap data);

    QJsonObject parseReply(QNetworkReply *reply);

    bool onFinishRequest(QNetworkReply *reply);

    void handleQtNetworkErrors(QNetworkReply *reply, QJsonObject &obj);

    QNetworkAccessManager *manager;
};

#endif // API_H
