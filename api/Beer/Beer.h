#ifndef ALKOBOT_BEER_H
#define ALKOBOT_BEER_H

#include "tgbot/tgbot.h"

namespace API {
    class Beer {
    public:
        static void beer(TgBot::Bot*, TgBot::Message::Ptr);
        static void beer_top(TgBot::Bot*, TgBot::Message::Ptr);
        static void set(TgBot::Bot*, TgBot::Message::Ptr);
    };
}


#endif //ALKOBOT_BEER_H
