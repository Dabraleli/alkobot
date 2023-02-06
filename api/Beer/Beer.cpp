#include "Beer.h"
#include "../../Database.h"
#include "../../utils/Utils.h"
#include <QString>
#include <QStringList>
#include <QMap>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>

void API::Beer::beer(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    Database db;
    if(db.isUserBanned(message->from->id, "beer")){
        bot->getApi().sendMessage(message->chat->id, "От тебя не принимаю, заебал", true,
                                  message->messageId);
        return;
    }

    QFile adminFile(qApp->applicationDirPath() + "/config.json");
    adminFile.open(QIODevice::ReadOnly);
    QJsonObject config = QJsonDocument::fromJson(adminFile.readAll()).object();
    float beerMultipler = config["beer_multipler"].toDouble();
    int beerInterval = config["beer_interval"].toInt();
    adminFile.close();

    if(!db.canDrinkBeer(message->from->id, beerMultipler, beerInterval * 60)){
        bot->getApi().sendMessage(message->chat->id, "Брат, притормози. Много пьёшь", true,
                                  message->messageId);
        return;
    }
    QString qmessage = QString::fromStdString(message->text);
    auto values = qmessage.split(" ");
    if (values.count() != 2) {
        bot->getApi().sendMessage(message->chat->id, "Напиши /beer <значение в литрах>", true,
                                 message->messageId);
        return;
    }
    QString value = values[1].replace(",", ".");
    if (value.toFloat() < 0.1) {
        bot->getApi().sendMessage(message->chat->id, "Ты числа не умеешь вводить? Принимаю от 0.1 до 10", true,
                                 message->messageId);
        return;
    }
    float amount = value.toFloat();
    if(amount > 10){
        auto top = db.getBeerTop();
        auto user = top[0].first;
        bot->getApi().sendMessage(message->chat->id, QString("Ну ты и врунишка, столько даже %1 не выпьет").arg(user).toStdString(), true,
                                 message->messageId);
    } else {
        db.addLitresToUser(message->from->id, amount);
        bot->getApi().sendMessage(message->chat->id, "Брат, продолжай в таком же темпе", true,
                                 message->messageId);
    }
}

void API::Beer::beer_top(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    QString result = "";
    Database db;
    auto top = db.getBeerTop();
    int shift = 0;
    QList<std::pair<QString, float>> usersInTop;
    for(int i = 0; i < top.size(); i++) {
        try {
            auto member = bot->getApi().getChatMember(message->chat->id, top[i].first);
            if (member)
                usersInTop << std::make_pair(Utils::formatDisplayName(member), top[i].second);

        } catch (TgBot::TgException &e) {}
    }

    for (int i = 0; i < usersInTop.size(); i++) {
        shift = std::max(shift, usersInTop[i].first.length());
    }
    for (int i = 0; i < top.size(); i++) {
        result += usersInTop[i].first.leftJustified(shift) + " " + QString::number(top[i].second) + "\r\n";
    }
    if(result.isEmpty())
        bot->getApi().sendMessage(message->chat->id, "Алголиков нет, фишач пьёт иван-чай", true, message->messageId);
    else {
        TgBot::MessageEntity::Ptr entity = std::make_shared<TgBot::MessageEntity>();
        entity->offset = 0;
        entity->length = result.length();
        entity->type = TgBot::MessageEntity::Type::Code;
        std::vector<TgBot::MessageEntity::Ptr> test;
        test.push_back(entity);
        bot->getApi().sendMessage(message->chat->id, result.toStdString(), true, message->messageId, nullptr, "", false, test);
    }
}

void API::Beer::set(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    QString messageText = QString::fromStdString(message->text);
    auto arguments = messageText.split(" ");
    if(arguments.count() == 3){
        qint64 user = arguments[1].toLongLong();
        float amount = arguments[2].toFloat();
        Database db;
        db.setLitresToUser(user, amount);
        bot->getApi().sendMessage(message->chat->id, "Довыебывался", true, message->messageId);
    } else
        bot->getApi().sendMessage(message->chat->id, "Накосячил с форматом", true, message->messageId);
}
