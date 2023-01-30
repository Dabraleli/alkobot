#ifndef ALKOBOT_DATABASE_H
#define ALKOBOT_DATABASE_H


#include <QString>
#include <vector>
#include <qpair.h>
#include <QDateTime>

class Database {
public:
    Database();
    void addLitresToUser(QString user, float amount);
    std::vector<std::pair<QString, float>> getBeerTop();
    void updateFishing(QString user, QDateTime time);
    std::vector<std::pair<QString, QDateTime>> getLastFishings();
};


#endif //ALKOBOT_DATABASE_H
