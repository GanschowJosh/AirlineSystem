#include "Utils.h"
#include <cstdio>

int convert_to_minutes(int hhmm) {
    int hours = hhmm / 100;
    int minutes = hhmm % 100;
    return hours * 60 + minutes;
}

string format_time(int minutes) {
    int h = minutes / 60;
    int m = minutes % 60;
    char buffer[6];
    sprintf(buffer, "%02d:%02d", h, m);
    return string(buffer);
}