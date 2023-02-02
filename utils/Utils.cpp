#include "Utils.h"

QString Utils::formatDisplayName(TgBot::ChatMember::Ptr member) {
    return QString::fromStdString(
            member->user->firstName +
            (member->user->lastName.length() ? " " + member->user->lastName : ""));
}

QString Utils::formatDays(int count) {
    if ((count / 10) % 10 == 1)
        return QString("%1 дней").arg(count);
    if (count % 10 == 1)
        return QString("%1 день").arg(count);
    if (count % 10 == 0)
        return QString("%1 дней").arg(count);
    if (count % 10 < 5)
        return QString("%1 дня").arg(count);
    return QString("%1 дней").arg(count);
}

QString Utils::formatFishingTotals(int count) {
    if ((count / 10) % 10 == 1)
        return QString("%1 рыбалок").arg(count);
    if (count % 10 == 1)
        return QString("%1 рыбалка").arg(count);
    if (count % 10 == 0)
        return QString("%1 рыбалок").arg(count);
    if (count % 10 < 5)
        return QString("%1 рыбалки").arg(count);
    return QString("%1 рыбалок").arg(count);
}

QString Utils::formatSuccessfulFishings(int count) {
    if ((count / 10) % 10 == 1)
        return QString("успешных %1").arg(count);
    if (count % 10 == 1)
        return QString("успешная %1").arg(count);
    if (count % 10 == 0)
        return QString("успешных %1").arg(count);
    if (count % 10 < 5)
        return QString("успешных %1").arg(count);
    return QString("успешных %1").arg(count);
}
