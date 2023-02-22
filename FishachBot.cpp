#include <QFile>
#include <QCoreApplication>
#include "FishachBot.h"
#include "api/Fish/Fish.h"
#include "api/Beer/Beer.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>
#include "api/Ban/Ban.h"

QHash<QString, CustomCommand> FishachBot::m_handlers = {
        {"/fish",         {&API::Fish::fish,         true}},
        {"/fish_top",     {&API::Fish::last_fishing, true}},
        {"/last_fishing", {&API::Fish::last_fishing, true}},
        {"/set_fish",     {&API::Fish::set_fish,     false,  true}},
        {"/beer",         {&API::Beer::beer,         true}},
        {"/beer_set",     {&API::Beer::set,          false,  true}},
        {"/beer_top",     {&API::Beer::beer_top,     true}},
        {"/k_ban",        {&API::Ban::ban,           false, true}},
        {"/k_pardon",     {&API::Ban::pardon,        false, true}}
};

void FishachBot::handleMessage(TgBot::Message::Ptr message) {
    QString messageText = QString::fromStdString(message->text);
    if (message->forwardFrom != nullptr) {
        return;
    }
    if (messageText.startsWith("/")) {
        QStringList messageParts = messageText.split(" ");
        QString botCommand = messageParts[0].split("@")[0];
        if (m_handlers.contains(botCommand)) {
            auto command = m_handlers.value(botCommand);
            auto user = message->from;
            qDebug().noquote().nospace() << "Command " << messageText << " received from " << user->id << ":"
                                         << QString::fromStdString(user->username);
            if (command.isAdmin) {
                if (!m_admins.contains(user->id)) {
                    m_bot.getApi().sendMessage(
                            message->chat->id,
                            "Тебе нельзя такую команду",
                            false,
                            message->messageId
                    );
                    return;
                }
            }
            if (command.chatRestricted){
                if(!m_restrictedChats.contains(message->chat->id)){
                    m_bot.getApi().sendMessage(
                            message->chat->id,
                            "Эту команду можно ввести только в /fi",
                            false,
                            message->messageId
                    );
                    qDebug() << "Command received from wrong chat " << message->chat->id;
                    return;
                }
            }
            command.commandHandler(&m_bot, message);
        }
    }
}

void FishachBot::readAdmins() {
    QFile adminFile(qApp->applicationDirPath() + "/config.json");
    adminFile.open(QIODevice::ReadOnly);
    QJsonObject config = QJsonDocument::fromJson(adminFile.readAll()).object();
    QJsonArray admins = config["admins"].toArray();
    adminFile.close();
    for (auto v: admins)
        m_admins << v.toInt();
}

void FishachBot::startHandler() {
    qDebug() << m_bot.getApi().getMe()->username.c_str();
    while (true) {
        try {
            m_bot.getApi().deleteWebhook();
            TgBot::TgLongPoll longPoll(m_bot);
            while (true) {
                longPoll.start();
            }
        } catch (TgBot::TgException &e) {
            qCritical() << QString("error: %1").arg(e.what());
        }
    }
}

FishachBot::FishachBot(QString apiKey)
        : m_bot(apiKey.toStdString()) {
    m_bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        if (!message->newChatMembers.empty())
            handleJoinMessage(message);
        else handleMessage(message);
    });
}

void FishachBot::handleJoinMessage(TgBot::Message::Ptr message) {
    std::string fullString;
    fullString = message->newChatMembers[0]->firstName;
    if(!message->newChatMembers[0]->lastName.empty())
        fullString += " " + message->newChatMembers[0]->lastName;
    qDebug() << "New member " << QString::fromStdString(message->newChatMembers[0]->username);
    if(QString::fromStdString(message->newChatMembers[0]->username).contains("amstorx"))
        m_bot.getApi().sendMessage(message->chat->id, "Мумжа уходи", true, message->messageId);
    else
        m_bot.getApi().sendMessage(message->chat->id, fullString + ", присаживайся к огоньку!\n"
                                                                 "Не томи, пости своих чебаков, показывай снасти.\n"
                                                                 "Подпишись на @fishach и послушай наши байки или трави свой #отчёт, чувствуй себя как дома.", true, message->messageId);
    //m_bot.getApi().sendMessage(message->chat->id, "[спаилился](tg://user?id=5469954622)", true, 0, nullptr, "Markdown", true);

}

void FishachBot::readRestrictedChats() {
    QFile configFile(qApp->applicationDirPath() + "/config.json");
    configFile.open(QIODevice::ReadOnly);
    QJsonObject config = QJsonDocument::fromJson(configFile.readAll()).object();
    QJsonArray chats = config["restricted_chats"].toArray();
    configFile.close();
    for (auto v: chats)
        m_restrictedChats << v.toString().toLongLong();
}

