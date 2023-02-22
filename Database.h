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

    void setLitresToUser(qint64 user, float amount);

    bool canDrinkBeer(qint64 user, float multipler, int interval);

    std::vector<std::pair<qint64, float>> getBeerTop();

    void updateFishing(qint64 user, QDateTime time, bool success);

    void setFishing(qint64 user, int total, int success);

    QDateTime getLastPersonFishing(qint64 userId);

    std::vector<LastFishing> getLastFishings();

    bool isUserBanned(qint64 userId, QString prefix);

    void banUser(qint64 user, QString prefix);

    void pardonUser(qint64 user, QString prefix);

    float getUserLitres(qint64 user);
};


#endif //ALKOBOT_DATABASE_H
