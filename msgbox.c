#include <stdio.h>
#include <string.h>
#include <time.h>
#include "tree.h"


#define MAX_USERS 10
#define MAX_MESSAGES 100
#define MAX_MESSAGE_LENGTH 256

typedef struct 
{
    char username[30];
} User;

typedef struct 
{
    char content[MAX_MESSAGE_LENGTH];
    User sender;
    User recipient;
    char timestamp[20];  // Store date and time as a string
} Message;

typedef struct 
{
    User users[MAX_USERS];
    Message messages[MAX_MESSAGES];
    int userCount;
    int messageCount;
} ChatPlatform;

void addUser(ChatPlatform *platform, const char *username) {
    if (platform->userCount < MAX_USERS) {
        strcpy(platform->users[platform->userCount].username, username);
        platform->userCount++;
        printf("User '%s' added successfully.\n", username);
    } else {
        printf("Error: Maximum number of users reached.\n");
    }
}

void listUsers(ChatPlatform *platform) {
    printf("User List:\n");
    for (int i = 0; i < platform->userCount; ++i) {
        printf("%d. %s\n", i + 1, platform->users[i].username);
    }
}

void sendMessage(ChatPlatform *platform, const char *senderUsername, const char *content) {
    if (platform->messageCount < MAX_MESSAGES) {
        if (platform->userCount > 1) {
            int senderIndex = -1;
            for (int i = 0; i < platform->userCount; ++i) {
                if (strcmp(platform->users[i].username, senderUsername) == 0) {
                    senderIndex = i;
                    break;
                }
            }

            if (senderIndex != -1) {
                listUsers(platform);

                int recipientIndex;
                do {
                    printf("Choose recipient (enter the number): ");
                    char recipientChoice[10];
                    fgets(recipientChoice, sizeof(recipientChoice), stdin);
                    recipientIndex = atoi(recipientChoice) - 1;
                } while (recipientIndex < 0 || recipientIndex >= platform->userCount || recipientIndex == senderIndex);

                Message newMessage;
                strcpy(newMessage.content, content);
                newMessage.sender = platform->users[senderIndex];
                newMessage.recipient = platform->users[recipientIndex];

                // Get current date and time
                time_t t = time(NULL);
                struct tm *tm_info = localtime(&t);
                strftime(newMessage.timestamp, sizeof(newMessage.timestamp), "%Y-%m-%d %H:%M:%S", tm_info);

                // Save the message to CSV
                FILE *csvFile = fopen("messages.csv", "a");
                if (csvFile != NULL) {
                    fprintf(csvFile, "%s,%s,%s,%s,%s\n",
                            newMessage.timestamp,
                            newMessage.sender.username,
                            newMessage.recipient.username,
                            newMessage.content,
                            newMessage.timestamp);

                    fclose(csvFile);
                } else {
                    printf("Error: Unable to open the CSV file.\n");
                }

                platform->messages[platform->messageCount] = newMessage;
                platform->messageCount++;

                printf("Message sent from %s to %s.\n", senderUsername, platform->users[recipientIndex].username);
            } else {
                printf("Error: Sender not found.\n");
            }
        } else {
            printf("Error: Need at least two users to send a message.\n");
        }
    } else {
        printf("Error: Maximum number of messages reached.\n");
    }
}

void listMessages(const ChatPlatform *platform) {
    printf("Message List:\n");
    for (int i = 0; i < platform->messageCount; ++i) {
        printf("%d. Time: %s From: %s To: %s - %s\n", i + 1,
               platform->messages[i].timestamp,
               platform->messages[i].sender.username,
               platform->messages[i].recipient.username,
               platform->messages[i].content);
    }
}

void checkReceivedMessages(const ChatPlatform *platform, const char *recipientUsername) {
    printf("Received Messages for %s:\n", recipientUsername);
    for (int i = 0; i < platform->messageCount; ++i) {
        if (strcmp(platform->messages[i].recipient.username, recipientUsername) == 0) {
            printf("Time: %s From: %s To: %s - %s\n",
                   platform->messages[i].timestamp,
                   platform->messages[i].sender.username,
                   platform->messages[i].recipient.username,
                   platform->messages[i].content);
        }
    }
}

void filterMessagesByTime(const ChatPlatform *platform, const char *startTime, const char *endTime) {
    printf("Messages sent between %s and %s:\n", startTime, endTime);
    for (int i = 0; i < platform->messageCount; ++i) {
        if (strcmp(platform->messages[i].timestamp, startTime) >= 0 &&
            strcmp(platform->messages[i].timestamp, endTime) <= 0) {
            printf("Time: %s From: %s To: %s - %s\n",
                   platform->messages[i].timestamp,
                   platform->messages[i].sender.username,
                   platform->messages[i].recipient.username,
                   platform->messages[i].content);
        }
    }
}

void filterMessagesByUsers(const ChatPlatform *platform, const char *senderUsername, const char *recipientUsername) {
    printf("Messages between %s and %s:\n", senderUsername, recipientUsername);
    for (int i = 0; i < platform->messageCount; ++i) {
        if (strcmp(platform->messages[i].sender.username, senderUsername) == 0 &&
            strcmp(platform->messages[i].recipient.username, recipientUsername) == 0) {
            printf("Time: %s From: %s To: %s - %s\n",
                   platform->messages[i].timestamp,
                   platform->messages[i].sender.username,
                   platform->messages[i].recipient.username,
                   platform->messages[i].content);
        }
    }
}

void searchMessages(const ChatPlatform *platform, const char *searchWord) {
    printf("Messages containing '%s':\n", searchWord);
    for (int i = 0; i < platform->messageCount; ++i) {
        if (strstr(platform->messages[i].content, searchWord) != NULL) {
            printf("Time: %s From: %s To: %s - %s\n",
                   platform->messages[i].timestamp,
                   platform->messages[i].sender.username,
                   platform->messages[i].recipient.username,
                   platform->messages[i].content);
        }
    }
}

int main() {
    ChatPlatform platform;
    platform.userCount = 0;
    platform.messageCount = 0;

    char input[256];
    char username[30];
    char messageContent[MAX_MESSAGE_LENGTH];
    char startTime[20];
    char endTime[20];
    char searchWord[30];

    while (1) {
        printf("-------------------------\n1. Add User\n2. List Users\n3. Send Message\n4. List Messages\n"
               "5. Check Received Messages\n6. Filter Messages by Time\n"
               "7. Filter Messages by Users\n8. Search Messages\n9. Exit\n----------------------\n");
        printf("Choose an option: ");
        fgets(input, sizeof(input), stdin);

        int option = atoi(input);

        switch (option) {
            case 1:
                printf("Enter username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';  // Remove newline character
                addUser(&platform, username);
                break;

            case 2:
                listUsers(&platform);
                break;

            case 3:
                printf("Enter your username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';  // Remove newline character
                printf("Enter message content: ");
                fgets(messageContent, sizeof(messageContent), stdin);
                messageContent[strcspn(messageContent, "\n")] = '\0';  // Remove newline character
                sendMessage(&platform, username, messageContent);
                break;

            case 4:
                listMessages(&platform);
                break;

            case 5:
                printf("Enter your username to check received messages: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';  // Remove newline character
                checkReceivedMessages(&platform, username);
                break;

            case 6:
                printf("Enter start time (YYYY-MM-DD HH:MM:SS): ");
                fgets(startTime, sizeof(startTime), stdin);
                startTime[strcspn(startTime, "\n")] = '\0';  // Remove newline character
                printf("Enter end time (YYYY-MM-DD HH:MM:SS): ");
                fgets(endTime, sizeof(endTime), stdin);
                endTime[strcspn(endTime, "\n")] = '\0';  // Remove newline character
                filterMessagesByTime(&platform, startTime, endTime);
                break;

            case 7:
                printf("Enter sender's username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = '\0';  // Remove newline character
                printf("Enter recipient's username: ");
                fgets(messageContent, sizeof(messageContent), stdin);
                messageContent[strcspn(messageContent, "\n")] = '\0';  // Remove newline character
                filterMessagesByUsers(&platform, username, messageContent);
                break;

            case 8:
                printf("Enter search word: ");
                fgets(searchWord, sizeof(searchWord), stdin);
                searchWord[strcspn(searchWord, "\n")] = '\0';  // Remove newline character
                searchMessages(&platform, searchWord);
                break;

            case 9:
                printf("Exiting...\n");
                exit(0);

            default:
                printf("Invalid option. Please try again.\n");
        }
    }

}
