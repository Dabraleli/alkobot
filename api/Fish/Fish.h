#ifndef ALKOBOT_FISH_H
#define ALKOBOT_FISH_H

#include "tgbot/tgbot.h"

namespace API {
    class Fish {
    public:
        static void fish(TgBot::Bot*, TgBot::Message::Ptr);
        static void last_fishing(TgBot::Bot*, TgBot::Message::Ptr);
    };
}


#endif //ALKOBOT_FISH_H
