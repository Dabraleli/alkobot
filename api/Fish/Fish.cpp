#include "Fish.h"
#include "../../Database.h"
#include <QDebug>
#include "../../Utils/Utils.h"

void API::Fish::fish(TgBot::Bot* bot, TgBot::Message::Ptr message) {
    Database db;
    auto lastDate = db.getLastPersonFishing(message->from->id);
    if(lastDate.date().day() == QDateTime::currentDateTime().date().day()){
        bot->getApi().sendMessage(message->chat->id, "Брат, ты сегодня уже рыбачил. Не жульничай", true,
                                  message->messageId);
        return;
    }
    QString textMessage = QString::fromStdString(message->text);
    auto arguments = textMessage.split(" ");
    bool isValid = true;
    isValid = arguments.size() == 2;
    if(isValid)
        isValid = arguments[1] == "посос" || arguments[1] == "поймал";
    if(!isValid){
        bot->getApi().sendMessage(message->chat->id, "Пиши либо /fish посос, либо /fish поймал", true,
                                  message->messageId);
    } else {
        TgBot::MessageEntity::Ptr entity = std::make_shared<TgBot::MessageEntity>();
        db.updateFishing(message->from->id, QDateTime::currentDateTime(), arguments[1] == "поймал");
        if(arguments[1] == "поймал")
            bot->getApi().sendMessage(message->chat->id, "Брат, молодец. Ходи на рыбалку почаще", true,
                                  message->messageId);
        else bot->getApi().sendMessage(message->chat->id, "Не расстраивайся брат, обязательно поймаешь в следующий раз", true,
                                       message->messageId);
    }
}

void API::Fish::last_fishing(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    QString result = "Сколько не рыбачили:\n";
    Database db;
    auto top = db.getLastFishings();
    auto currentTime = QDateTime::currentDateTime();
    int shift = 0;
    QList<std::pair<QString, LastFishing>> usersInTop;

    for(int i = 0; i < top.size(); i++) {
        try {
            auto member = bot->getApi().getChatMember(message->chat->id, top[i].user);
            if (member)
                usersInTop << std::make_pair(Utils::formatDisplayName(member), top[i]);

        } catch (TgBot::TgException &e) {}
    }

    for (int i = 0; i < usersInTop.size(); i++) {
        shift = std::max(shift, usersInTop[i].first.length());
    }
    for (int i = 0; i < usersInTop.size(); i++) {
        result += usersInTop[i].first.leftJustified(shift) + " не рыбачил " + Utils::formatDays(abs(currentTime.daysTo(usersInTop[i].second.lastFishing))) + "\r\n";
        result += "Всего " + Utils::formatFishingTotals(usersInTop[i].second.total) + ", " + Utils::formatSuccessfulFishings(usersInTop[i].second.success) + "\r\n";
    }
    if(result.isEmpty())
        bot->getApi().sendMessage(message->chat->id, "Никто не рыбачит", true, message->messageId);
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

void API::Fish::set_fish(TgBot::Bot * bot, TgBot::Message::Ptr message) {
    Database db;
    QString textMessage = QString::fromStdString(message->text);
    auto arguments = textMessage.split(" ");
    if(arguments.count() == 4) {
        qint64 user = arguments[1].toLongLong();
        int total = arguments[2].toInt();
        int success = arguments[3].toInt();
        db.setFishing(user, total, success);
        bot->getApi().sendMessage(message->chat->id, "Готово", true, message->messageId);
    } else
        bot->getApi().sendMessage(message->chat->id, "Накосячил с форматом", true, message->messageId);
}
