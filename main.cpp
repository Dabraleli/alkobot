#include <QCoreApplication>
#include <QDebug>
#include "tgbot/Bot.h"
#include "tgbot/net/TgLongPoll.h"
#include "Database.h"
#include "FishachBot.h"
#include <QDebug>
#include <QMutexLocker>
#include <QDir>

using namespace TgBot;
using namespace std;

void customMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    auto datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("dd.MM.yy hh:mm:ss.zzz");

    if (!QDir(qApp->applicationDirPath() + "/logs").exists())
        QDir(qApp->applicationDirPath()).mkdir("logs");
    QFile logfile(QString("logs/%1-%2-%3.txt").arg(datetime.date().year()).arg(datetime.date().month()).arg(
            datetime.date().day()));
    logfile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&logfile);
    stream.setCodec("UTF-8");
    switch (type) {
        case QtDebugMsg:
            fprintf(stdout, "[%s] [%s@%u] [INFO] %s\n", date.toStdString().c_str(), context.function, context.line,
                    localMsg.constData());
            stream << "[" << date.toStdString().c_str() << "] [" << context.function << "@" << context.line << "] "
                   << msg.toStdString().c_str() << endl;
            break;
        case QtInfoMsg:
            fprintf(stdout, "[%s] [%s@%u] [INFO] %s\n", date.toStdString().c_str(), context.function, context.line,
                    localMsg.constData());
            stream << "[" << date.toStdString().c_str() << "] [" << context.function << "@" << context.line
                   << "] [INFO] " << localMsg.constData() << endl;
            break;
        case QtWarningMsg:
            fprintf(stdout, "[%s] [%s@%u] [WARNING] %s\n", date.toStdString().c_str(), context.function, context.line,
                    localMsg.constData());
            stream << "[" << date.toStdString().c_str() << "] [" << context.function << "@" << context.line
                   << "] [WARNING] " << localMsg.constData() << endl;
            break;
        case QtCriticalMsg:
            fprintf(stderr, "[%s] [%s@%u] [CRITICAL] %s\n", date.toStdString().c_str(), context.function, context.line,
                    localMsg.constData());
            stream << "[" << date.toStdString().c_str() << "] [" << context.function << "@" << context.line
                   << "] [CRITICAL] " << localMsg.constData() << endl;
            break;
        case QtFatalMsg:
            fprintf(stderr, "[%s] [%s@%u] [FATAL] %s\n", date.toStdString().c_str(), context.function, context.line,
                    localMsg.constData());
            stream << "[" << date.toStdString().c_str() << "] [" << context.function << "@" << context.line
                   << "] [FATAL] " << localMsg.constData() << endl;
            abort();
    }
    logfile.close();
}

int main(int argc, char *argv[]) {
    /*Database db;
    TgBot::Bot bot("5988228878:AAGhVYImhRw76gtRsi7vwIMVJBSqXBvratc");

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
     */
    QCoreApplication app(argc, argv);
    qInstallMessageHandler(customMessageOutput);
    FishachBot bot("5988228878:AAGhVYImhRw76gtRsi7vwIMVJBSqXBvratc");
    bot.readAdmins();
    bot.startHandler();
    return 0;
}
