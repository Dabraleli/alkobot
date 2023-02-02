#ifndef ALKOBOT_UTILS_H
#define ALKOBOT_UTILS_H


#include <QString>
#include "tgbot/types/User.h"
#include "tgbot/types/ChatMember.h"

class Utils {
public:
    static QString formatDisplayName(TgBot::ChatMember::Ptr user);

    static QString formatDays(int count);

    static QString formatFishingTotals(int count);

    static QString formatSuccessfulFishings(int count);
};


#endif //ALKOBOT_UTILS_H
