#define NULL_TEMP_VALUE -999

struct RemoteTemp {
    int id = -1;
    long lastMessageTime = 0;
    double value = NULL_TEMP_VALUE;
};
