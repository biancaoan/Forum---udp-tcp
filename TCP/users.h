#ifndef USER_H
#define USER_H

#define USERNAME_MAX 32

typedef struct {
    char username[USERNAME_MAX];
} User;

// Initialize user (by username)
void userInit(User *user, const char *name);

// Formats a forum line: "username: message"
void userMessage(const User *user, const char *message, char *output, int size);

#endif
