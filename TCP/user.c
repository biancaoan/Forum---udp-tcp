#include "users.h"
#include <string.h>

void userInit(User *user, const char *name)
{
    int i = 0;
    while (i < USERNAME_MAX - 1 && name[i] != '\0' && name[i] != '\n') {
        user -> username[i] = name[i];
        i++;
    }
    user -> username[i] = '\0';
}

void userMessage(const User *user, const char *message, char *output, int size)
{
    int i = 0;
    int j = 0;
    while (user -> username[i] != '\0' && j < size - 1) {
        output[j++] = user -> username[i++];
    }
    if (j < size - 2) {
        output[j++] = ':';
        output[j++] = ' ';
    }
    i = 0;
    while (message[i] != '\0' && message[i] != '\n' && j < size - 2) {
        output[j++] = message[i++];
    }
    output[j++] = '\n';
    output[j] = '\0';
}
