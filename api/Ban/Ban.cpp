#include <QString>
#include "Ban.h"
#include "../../Database.h"
#include <QStringList>

void API::Ban::ban(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    QString messageText = QString::fromStdString(message->text);
    auto arguments = messageText.split(" ");
    if(arguments.count() == 3){
        Database db;
        qint64 user = arguments[1].toLongLong();
        QString prefix = arguments[2];
        db.banUser(user, prefix);
        bot->getApi().sendMessage(message->chat->id, "Довыебывался", true, message->messageId);
    } else
        bot->getApi().sendMessage(message->chat->id, "Накосячил с форматом", true, message->messageId);
}

void API::Ban::pardon(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    QString messageText = QString::fromStdString(message->text);
    auto arguments = messageText.split(" ");
    if(arguments.count() == 3){
        Database db;
        qint64 user = arguments[1].toLongLong();
        QString prefix = arguments[2];
        db.pardonUser(user, prefix);
        bot->getApi().sendMessage(message->chat->id, "Просим прощения", true, message->messageId);
    } else
        bot->getApi().sendMessage(message->chat->id, "Накосячил с форматом", true, message->messageId);
}
