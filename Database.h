#ifndef ALKOBOT_DATABASE_H
#define ALKOBOT_DATABASE_H


#include <QString>
#include <vector>
#include <qpair.h>
#include <QDateTime>

struct LastFishing {
    qint64 user;
    QDateTime lastFishing;
    int success;
    int total;
};

class Database {
public:
    Database();

    void addLitresToUser(qint64 user, float amount);

    std::vector<std::pair<qint64, float>> getBeerTop();

    void updateFishing(qint64 user, QDateTime time, bool success);

    QDateTime getLastPersonFishing(qint64 userId);

    std::vector<LastFishing> getLastFishings();
};


#endif //ALKOBOT_DATABASE_H
