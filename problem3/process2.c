#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>
#include <unistd.h>

#define MSG_KEY 1234
#define MAX_TEXT 100

typedef struct {
    long msg_type;
    char text[MAX_TEXT];
} message_t;

int msgid;

void *receive_msg(void *arg) {
    message_t msg;
    
    while (1) {
        msgrcv(msgid, &msg, sizeof(msg.text), 1, 0);
        printf("Process 2 received: %s", msg.text);
        
        if (strncmp(msg.text, "exit", 4) == 0) {
            break;
        }
    }
    pthread_exit(NULL);
}

void *send_msg(void *arg) {
    message_t msg;
    msg.msg_type = 2;

    while (1) {
        printf("Process 2 - Enter response: ");
        fgets(msg.text, MAX_TEXT, stdin);
        msgsnd(msgid, &msg, sizeof(msg.text), 0);
        
        if (strncmp(msg.text, "exit", 4) == 0) {
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t send_thread, receive_thread;

    msgid = msgget(MSG_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(1);
    }

    pthread_create(&receive_thread, NULL, receive_msg, NULL);
    pthread_create(&send_thread, NULL, send_msg, NULL);

    pthread_join(receive_thread, NULL);
    pthread_join(send_thread, NULL);

    return 0;
}
