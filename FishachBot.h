#ifndef ALKOBOT_FISHACHBOT_H
#define ALKOBOT_FISHACHBOT_H

#include <QObject>
#include "tgbot/Bot.h"
#include <QHash>

struct CustomCommand {
public:
    CustomCommand() = default;

    CustomCommand(std::function<void(TgBot::Bot *, TgBot::Message::Ptr)> handler, bool chatRestricted = true, bool isAdminCommand = false) {
        this->commandHandler = handler;
        this->isAdmin = isAdminCommand;
        this->chatRestricted = chatRestricted;
    }

    CustomCommand(const CustomCommand &other) {
        this->commandHandler = other.commandHandler;
        this->isAdmin = other.isAdmin;
        this->chatRestricted = other.chatRestricted;
    }

    std::function<void(TgBot::Bot *, TgBot::Message::Ptr)> commandHandler;
    bool isAdmin;
    bool chatRestricted;
};

class FishachBot : public QObject {
Q_OBJECT
public:
    FishachBot(QString apiKey);

    void readAdmins();

    void readRestrictedChats();

    void startHandler();

private:
    void handleMessage(TgBot::Message::Ptr message);

    void handleJoinMessage(TgBot::Message::Ptr message);

    static QHash<QString, CustomCommand> m_handlers;
    QList<int64_t> m_admins;
    QList<int64_t> m_restrictedChats;
    TgBot::Bot m_bot;
};


#endif //ALKOBOT_FISHACHBOT_H
