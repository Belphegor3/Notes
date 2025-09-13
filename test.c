#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t viande, pain;

void* employeA(void* arg) {
    pthread_mutex_lock(&viande);
    sleep(1); // Simule le temps d'action
    pthread_mutex_lock(&pain); // Attend le pain (bloqué !)
    printf("Employé A fait un burger\n");
    pthread_mutex_unlock(&pain);
    pthread_mutex_unlock(&viande);
    return NULL;
}

void* employeB(void* arg) {
    pthread_mutex_lock(&pain);
    sleep(1); // Simule le temps d'action
    pthread_mutex_lock(&viande); // Attend la viande (bloqué !)
    printf("Employé B fait un burger\n");
    pthread_mutex_unlock(&viande);
    pthread_mutex_unlock(&pain);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_mutex_init(&viande, NULL);
    pthread_mutex_init(&pain, NULL);

    pthread_create(&thread1, NULL, employeA, NULL);
    pthread_create(&thread2, NULL, employeB, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Service terminé !\n");
    return 0;
}