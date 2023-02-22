#include "Database.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <cmath>

double fround(double val) {
    return (double((int) ::round(val * 100.0))) / 100.0;
}

void Database::setLitresToUser(qint64 user, float amount) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM beer WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if (selectQuery.next()) {
        float currentAmount = 0;
        currentAmount = selectQuery.value("amount").toFloat();
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE beer SET amount = :amount, lastbeer = 2, lastdate = :lastdate WHERE user = :user;");
        updateQuery.bindValue(":amount", fround(amount));
        updateQuery.bindValue(":user", user);
        updateQuery.bindValue(":lastdate", QDateTime::currentDateTime().toSecsSinceEpoch());
        updateQuery.exec();
    }
}

void Database::addLitresToUser(qint64 user, float amount) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT * FROM beer WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if (selectQuery.next()) {
        float currentAmount = 0;
        currentAmount = selectQuery.value("amount").toFloat();
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE beer SET amount = :amount, lastbeer = :lastbeer, lastdate = :lastdate  WHERE user = :user;");
        updateQuery.bindValue(":amount", fround(amount + currentAmount));
        updateQuery.bindValue(":user", user);
        updateQuery.bindValue(":lastbeer", amount);
        updateQuery.bindValue(":lastdate", QDateTime::currentDateTime().toSecsSinceEpoch());
        updateQuery.exec();
    } else {
        QSqlQuery insertQuery;
        insertQuery.prepare("INSERT INTO beer (user, amount, lastbeer, lastdate) VALUES (:user, :amount, :lastbeer, :lastdate);");
        insertQuery.bindValue(":user", user);
        insertQuery.bindValue(":amount", fround(amount));
        insertQuery.bindValue(":lastbeer", amount);
        insertQuery.bindValue(":lastdate", QDateTime::currentDateTime().toSecsSinceEpoch());
        insertQuery.exec();
    }
}

Database::Database() {
    if (!QSqlDatabase::contains(QSqlDatabase::defaultConnection)) {
        QSqlDatabase sdb = QSqlDatabase::addDatabase("QSQLITE");
        sdb.setDatabaseName("database.sqlite");

        if (!sdb.open()) {
            qDebug() << "Database not open";
        }
    }
}

std::vector<std::pair<qint64, float>> Database::getBeerTop() {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT user, amount FROM beer ORDER BY amount DESC;");
    selectQuery.exec();
    std::vector<std::pair<qint64, float>> result;
    while (selectQuery.next()) {
        float amount = selectQuery.value("amount").toDouble();
        qint64 user = selectQuery.value("user").toLongLong();
        result.push_back(std::make_pair(user, amount));
    }
    return result;
}

void Database::updateFishing(qint64 user, QDateTime lastFishing, bool success) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT date, user, success, total FROM fishing WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if (selectQuery.next()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE fishing SET date = :date, total = :total, success = :success WHERE user = :user;");
        updateQuery.bindValue(":date", lastFishing.toSecsSinceEpoch());
        updateQuery.bindValue(":user", user);
        updateQuery.bindValue(":total", selectQuery.value("total").toInt() + 1);
        updateQuery.bindValue(":success", success ? selectQuery.value("success").toInt() + 1 : selectQuery.value(
                "success").toInt());
        updateQuery.exec();
    } else {
        QSqlQuery insertQuery;
        insertQuery.prepare(
                "INSERT INTO fishing (user, date, total, success) VALUES (:user, :date, :total, :success);");
        insertQuery.bindValue(":user", user);
        insertQuery.bindValue(":date", lastFishing.toSecsSinceEpoch());
        insertQuery.bindValue(":success", success ? 1 : 0);
        insertQuery.bindValue(":total", 1);
        insertQuery.exec();
    }
}

std::vector<LastFishing> Database::getLastFishings() {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT user, date, total, success FROM fishing ORDER BY date DESC;");
    selectQuery.exec();
    std::vector<LastFishing> result;
    while (selectQuery.next()) {
        LastFishing last;
        last.lastFishing = QDateTime::fromSecsSinceEpoch(selectQuery.value("date").toInt());
        last.user = selectQuery.value("user").toLongLong();
        last.total = selectQuery.value("total").toInt();
        last.success = selectQuery.value("success").toInt();
        result.push_back(last);
    }
    return result;
}

QDateTime Database::getLastPersonFishing(qint64 user) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT date, user FROM fishing WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if (selectQuery.next())
        return QDateTime::fromSecsSinceEpoch(selectQuery.value("date").toLongLong());
    else return QDateTime(QDate(2000, 01, 01), QTime(0, 0, 0, 0));
}

void Database::setFishing(qint64 user, int total, int success) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT date, user, success, total FROM fishing WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if (selectQuery.next()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE fishing SET total = :total, success = :success WHERE user = :user;");
        updateQuery.bindValue(":user", user);
        updateQuery.bindValue(":total", total);
        updateQuery.bindValue(":success", success);
        updateQuery.exec();
    }
}

bool Database::isUserBanned(qint64 user, QString prefix) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT user, prefix FROM bans WHERE user = :user AND prefix = :prefix;");
    selectQuery.bindValue(":user", user);
    selectQuery.bindValue(":prefix", prefix);
    selectQuery.exec();
    return selectQuery.next();
}

void Database::banUser(qint64 user, QString prefix) {
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO bans (user, prefix) VALUES (:user, :prefix);");
    insertQuery.bindValue(":user", user);
    insertQuery.bindValue(":prefix", prefix);
    insertQuery.exec();
}

void Database::pardonUser(qint64 user, QString prefix) {
    QSqlQuery deleteQuery;
    deleteQuery.prepare("DELETE FROM bans WHERE user = :user AND prefix = :prefix;");
    deleteQuery.bindValue(":user", user);
    deleteQuery.bindValue(":prefix", prefix);
    deleteQuery.exec();
}

bool Database::canDrinkBeer(qint64 user, float multipler, int interval) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT lastbeer, lastdate FROM beer WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if(selectQuery.next()){
        auto lastbeer = selectQuery.value("lastbeer").toFloat();
        QDateTime lastdate = QDateTime::fromSecsSinceEpoch(selectQuery.value("lastdate").toInt());
        int secsShouldHappen = (lastbeer / multipler) * interval;
        if(abs(lastdate.secsTo(QDateTime::currentDateTime())) > secsShouldHappen)
            return true;
        return false;
    } else return true;
}

float Database::getUserLitres(qint64 user) {
    QSqlQuery selectQuery;
    selectQuery.prepare("SELECT amount FROM beer WHERE user = :user;");
    selectQuery.bindValue(":user", user);
    selectQuery.exec();
    if(selectQuery.next())
        return selectQuery.value("amount").toFloat();
    return 0;
}
