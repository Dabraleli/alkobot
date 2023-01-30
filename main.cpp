#include <QCoreApplication>
#include <QDebug>
#include "tgbot/Bot.h"
#include "tgbot/net/TgLongPoll.h"
#include "Database.h"
#include <QDebug>

using namespace TgBot;
using namespace std;

int main(int argc, char *argv[]) {
    Database db;
    TgBot::Bot bot("ПОШЁЛ НАХУЙ");

    vector<BotCommand::Ptr> commands;
    BotCommand::Ptr cmdArray(new BotCommand);
    cmdArray->command = "/beer";
    cmdArray->description = "укажи сколько выпил";
    commands.push_back(cmdArray);

    BotCommand::Ptr cmdArray2(new BotCommand);
    cmdArray2->command = "/beer_top";
    cmdArray2->description = "топ алкоголиков";
    commands.push_back(cmdArray2);

    BotCommand::Ptr cmdArray3(new BotCommand);
    cmdArray3->command = "/last_fishing";
    cmdArray3->description = "сколько рыбу не ловим";
    commands.push_back(cmdArray3);

    BotCommand::Ptr cmdArray4(new BotCommand);
    cmdArray4->command = "/fish";
    cmdArray4->description = "отметить рыбалку";
    commands.push_back(cmdArray4);


    bot.getApi().deleteMyCommands();
    bot.getApi().setMyCommands(commands);

    vector<BotCommand::Ptr> vectCmd;
    vectCmd = bot.getApi().getMyCommands();

    for(std::vector<BotCommand::Ptr>::iterator it = vectCmd.begin(); it != vectCmd.end(); ++it) {
        printf("cmd: %s -> %s\r\n",(*it)->command.c_str(),(*it)->description.c_str());
    }

    bot.getEvents().onAnyMessage([&bot, &db](TgBot::Message::Ptr message) {
        if(!message->newChatMembers.empty()){
            std::string fullString;
            fullString = message->newChatMembers[0]->firstName;
            if(!message->newChatMembers[0]->lastName.empty())
                fullString += " " + message->newChatMembers[0]->lastName;
            if(QString::fromStdString(message->newChatMembers[0]->username).contains("amstorx"))
                bot.getApi().sendMessage(message->chat->id, "Мумжа уходи", true, message->messageId);
            else
                bot.getApi().sendMessage(message->chat->id, fullString + ", присаживайся к огоньку!\n"
                                                                               "Не томи, пости своих чебаков, показывай снасти.\n"
                                                                               "Подпишись на @fishach и послушай наши байки или трави свой #отчёт, чувствуй себя как дома.", true, message->messageId);
        } else {
            if(!message->text.empty()) {
                QString qmessage = QString::fromStdString(message->text);
                if (qmessage.startsWith("/beer_top")) {
                    QString result = "";
                    auto top = db.getBeerTop();
                    int shift = 0;
                    for (int i = 0; i < top.size(); i++) {
                        shift = std::max(shift, top[i].first.length());
                    }
                    for (int i = 0; i < top.size(); i++) {
                        result += top[i].first.leftJustified(shift) + " " + QString::number(top[i].second) + "\r\n";
                    }
                    if(result.isEmpty())
                       bot.getApi().sendMessage(message->chat->id, "Алголиков нет, фишач пьёт иван-чай", true, message->messageId);
                    else {
                        MessageEntity::Ptr entity = make_shared<MessageEntity>();
                        entity->offset = 0;
                        entity->length = result.length();
                        entity->type = TgBot::MessageEntity::Type::Code;
                        std::vector<MessageEntity::Ptr> test;
                        test.push_back(entity);
                       bot.getApi().sendMessage(message->chat->id, result.toStdString(), true, message->messageId, nullptr, "", false, test);
                    }
                } else if (qmessage.startsWith("/beer")) {
                    auto values = qmessage.split(" ");
                    if (values.count() != 2) {
                        bot.getApi().sendMessage(message->chat->id, "Напиши /beer <значение в литрах>", true,
                                                 message->messageId);
                        return;
                    }
                    QString value = values[1].replace(",", ".");
                    if (value.toFloat() <= 0) {
                        bot.getApi().sendMessage(message->chat->id, "Ты числа не умеешь вводить?", true,
                                                 message->messageId);
                        return;
                    }
                    float amount = value.toFloat();
                    if(message->from->id == 692956963)
                    {
                        bot.getApi().sendMessage(message->chat->id, QString("От тебя не принимаю, заебал").toStdString(), true,
                                                 message->messageId);
                        return;
                    }
                    qDebug() << QString::fromStdString(message->from->username) << " " << message->from->id;
                    if(amount > 10){
                        auto top = db.getBeerTop();
                        auto user = top[0].first;
                        bot.getApi().sendMessage(message->chat->id, QString("Ну ты и врунишка, столько даже %1 не выпьет").arg(user).toStdString(), true,
                                                 message->messageId);
                    } else {
                        db.addLitresToUser(QString::fromStdString(message->from->username), amount);
                        bot.getApi().sendMessage(message->chat->id, "Брат, продолжай в таком же темпе", true,
                                                 message->messageId);
                    }
                }
                else if(qmessage.startsWith("/last_fishing")){
                    QString result = "Сколько не рыбачили:\n";
                    auto top = db.getLastFishings();
                    auto currentTime = QDateTime::currentDateTime();
                    int shift = 0;
                    for (int i = 0; i < top.size(); i++) {
                        shift = std::max(shift, top[i].first.length());
                    }
                    for (int i = 0; i < top.size(); i++) {
                        result += top[i].first.leftJustified(shift) + " " + QString::number(abs(currentTime.daysTo(top[i].second))) + " дня(ей)" + "\r\n";
                    }
                    if(result.isEmpty())
                        bot.getApi().sendMessage(message->chat->id, "Никто не рыбачит", true, message->messageId);
                    else {
                        MessageEntity::Ptr entity = make_shared<MessageEntity>();
                        entity->offset = 0;
                        entity->length = result.length();
                        entity->type = TgBot::MessageEntity::Type::Code;
                        std::vector<MessageEntity::Ptr> test;
                        test.push_back(entity);
                        bot.getApi().sendMessage(message->chat->id, result.toStdString(), true, message->messageId, nullptr, "", false, test);
                    }
                } else if(qmessage.startsWith("/fish")){
                    db.updateFishing(QString::fromStdString(message->from->username), QDateTime::currentDateTime());
                    bot.getApi().sendMessage(message->chat->id, "Брат, молодец. Ходи на рыбалку почаще", true,
                                             message->messageId);
                }
            }
        }
    });
    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        bot.getApi().deleteWebhook();

        TgBot::TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}
