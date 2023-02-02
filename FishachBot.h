#ifndef ALKOBOT_FISHACHBOT_H
#define ALKOBOT_FISHACHBOT_H

#include <QObject>
#include "tgbot/Bot.h"
#include <QHash>

struct CustomCommand {
public:
    CustomCommand() = default;

    CustomCommand(std::function<void(TgBot::Bot *, TgBot::Message::Ptr)> handler, bool isAdminCommand = false) {
        this->commandHandler = handler;
        this->isAdmin = isAdminCommand;
    }

    CustomCommand(const CustomCommand &other) {
        this->commandHandler = other.commandHandler;
        this->isAdmin = other.isAdmin;
    }

    std::function<void(TgBot::Bot *, TgBot::Message::Ptr)> commandHandler;
    bool isAdmin;
};

class FishachBot : public QObject {
Q_OBJECT
public:
    FishachBot(std::string apiKey);

    void readAdmins();

    void startHandler();

private:
    void handleMessage(TgBot::Message::Ptr message);

    void handleJoinMessage(std::vector<TgBot::User::Ptr> &newMembers);

    static QHash<QString, CustomCommand> m_handlers;
    QList<int64_t> m_admins;
    TgBot::Bot m_bot;
};


#endif //ALKOBOT_FISHACHBOT_H
