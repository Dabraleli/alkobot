#include <QFile>
#include <QCoreApplication>
#include "FishachBot.h"
#include "api/Fish/Fish.h"
#include "api/Beer/Beer.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonObject>

QHash<QString, CustomCommand> FishachBot::m_handlers = {
        {"/fish", {&API::Fish::fish}},
        {"/fish_top", {&API::Fish::last_fishing}},
        {"/beer", {&API::Beer::beer}},
        {"/beer_set", {&API::Beer::set, true}},
        {"/beer_top", {&API::Beer::beer_top}}
};

void FishachBot::handleMessage(TgBot::Message::Ptr message) {
    QString messageText = QString::fromStdString(message->text);
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

FishachBot::FishachBot(std::string apiKey)
        : m_bot(apiKey) {
    m_bot.getEvents().onAnyMessage([&](TgBot::Message::Ptr message) {
        if (!message->newChatMembers.empty())
            handleJoinMessage(message->newChatMembers);
        else handleMessage(message);
    });
}

void FishachBot::handleJoinMessage(std::vector<TgBot::User::Ptr> &newMembers) {

}
