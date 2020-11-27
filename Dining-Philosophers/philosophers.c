#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define PHILOSOPHER_COUNT 5
#define TAKEN 0
#define FREE 1

void *handle_philoshpher(void *id);
void think(int phil_id);
void finish_eating(int phil_id);
void eat(int phil_id);
void put_down_chop(int phil_id);
void take_chop(int phil_id);


sem_t take_lock;
sem_t put_down_lock;
sem_t chopstick[PHILOSOPHER_COUNT];
int chop_states[PHILOSOPHER_COUNT];

int main(){
    pthread_t philosopher[PHILOSOPHER_COUNT];
    for(int i=0; i< PHILOSOPHER_COUNT; i++)
        chop_states[i] = FREE;

    sem_init(&take_lock, 0 , 1);
    sem_init(&put_down_lock, 0 , 1);
    for(int i=0; i<PHILOSOPHER_COUNT; i++)
        sem_init(&chopstick[i], 0, 1);

    for(int i=0; i<PHILOSOPHER_COUNT; i++){
        pthread_create(&philosopher[i], NULL, handle_philoshpher, (void *)i);
    }

    for (int i = 0; i < PHILOSOPHER_COUNT; i++)
        pthread_join(philosopher[i], NULL);
}
void *handle_philoshpher(void *id){
    int phil_id = (int)(long)id;
    int *temp;
    for(;1;){
        take_chop(phil_id);
        sleep(2);
        put_down_chop(phil_id);
    }

}
void take_chop(int phil_id){
    sem_wait(&take_lock);
    think(phil_id);

    sem_wait(&chopstick[phil_id]);
    sem_wait(&chopstick[(phil_id+1)%PHILOSOPHER_COUNT]);        
    chop_states[phil_id] = TAKEN;
    chop_states[(phil_id+1)%PHILOSOPHER_COUNT] = TAKEN;
    sem_post(&take_lock);

    eat(phil_id);  
}
void put_down_chop(int phil_id){
    sem_wait(&put_down_lock);
    sem_post(&chopstick[phil_id]);
    sem_post(&chopstick[(phil_id+1)%PHILOSOPHER_COUNT]);
    chop_states[phil_id] = FREE;
    chop_states[(phil_id+1)%PHILOSOPHER_COUNT] = FREE;
    finish_eating(phil_id);
    sem_post(&put_down_lock);
}
void think(int phil_id){
    printf("Philosopher[%d] is thinking\n", phil_id);
}
void eat(int phil_id){
    printf("Philosopher[%d] is eating using chopstick[%d], chopstick[%d]\n", phil_id, phil_id, (phil_id+1)%PHILOSOPHER_COUNT );
}
void finish_eating(int phil_id){
    printf("Philosopher[%d] is finished eating\n", phil_id);
}