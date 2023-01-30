#include "Database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <cmath>

double fround(double val) {
    return (double((int)::round( val * 100.0 ))) / 100.0;
}


void Database::addLitresToUser(QString user, float amount) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM beer WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if(selectQuery.next()){
        float currentAmount = 0;
        currentAmount = selectQuery.value("amount").toFloat();
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE beer SET amount = :amount WHERE user = :user;");
        updateQuery.bindValue(":amount", fround(amount + currentAmount));
        updateQuery.bindValue(":user", user);
        updateQuery.exec();
    } else {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO beer (user, amount) VALUES (:user, :amount);");
        insertQuery.bindValue(":user", user);
        insertQuery.bindValue(":amount", fround(amount));
        insertQuery.exec();
    }
}

Database::Database() {
    QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
    sdb.setDatabaseName("database.sqlite");

    if (!sdb.open()) {
        qDebug() << "Database not open";
    }

}

std::vector<std::pair<QString, float>> Database::getBeerTop() {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT user, amount FROM beer ORDER BY amount DESC;");
    selectQuery.exec();
    std::vector<std::pair<QString, float>> result;
    while(selectQuery.next()){
        float amount = selectQuery.value("amount").toDouble();
        QString user = selectQuery.value("user").toString();
        result.push_back(std::make_pair(user, amount));
    }
    return result;
}

void Database::updateFishing(QString user, QDateTime lastFishing) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM fishing WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if(selectQuery.next()){
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE fishing SET date = :date WHERE user = :user;");
        updateQuery.bindValue(":date", lastFishing.toSecsSinceEpoch());
        updateQuery.bindValue(":user", user);
        updateQuery.exec();
    } else {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO fishing (user, date) VALUES (:user, :date);");
        insertQuery.bindValue(":user", user);
        insertQuery.bindValue(":date", lastFishing.toSecsSinceEpoch());
        insertQuery.exec();
    }
}

std::vector<std::pair<QString, QDateTime>> Database::getLastFishings() {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT user, date FROM fishing ORDER BY date DESC;");
    selectQuery.exec();
    std::vector<std::pair<QString, QDateTime>> result;
    while(selectQuery.next()){
        QDateTime date = QDateTime::fromSecsSinceEpoch(selectQuery.value("date").toInt());
        QString user = selectQuery.value("user").toString();
        result.push_back(std::make_pair(user, date));
    }
    return result;
}
