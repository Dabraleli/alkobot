#ifndef ALKOBOT_BAN_H
#define ALKOBOT_BAN_H

#include "tgbot/Bot.h"
#include "tgbot/types/Message.h"

namespace API {
    class Ban {
    public:
        static void ban(TgBot::Bot *, TgBot::Message::Ptr);

        static void pardon(TgBot::Bot *, TgBot::Message::Ptr);
    };
}


#endif //ALKOBOT_BAN_H
