#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QPixmap>
#include <QUrlQuery>
#include "api.h"

typedef struct {
    quint8 lagerId;
    quint8 lagerUnit;
    bool markWithOnion;
    QString photoLinkLager;
    QString pizzaName;
} PizzaList;

typedef struct {
    quint8 approximateTimeWait;
} TimeWait;

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);

public slots:
    void getPizzaList();
    void getApproximateTimeWait();

private:
    QString _baseUrl;
    Api _API;
};

#endif // BACKEND_H
