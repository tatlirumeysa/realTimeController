/*
Authors --> Rumeysa Tatlı, Şerifhan Işıklı
In this program we aimed to check the sencor values that comes from chemical plants, 
We used threads to make that program faster and reasonable, and to make our threads synchronisable we used semaphores

*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
//defining threshold values
#define MAXTEMP 10 //assuming the incoming values are celcius for temperature
#define MINTEMP 1
#define MAXHUM 90 //assuming the values are given as percentage for moisture
#define MINHUM 30
#define MAXPRES 2.0 // assuming the type of pressure value is MPa
#define MINPRES 1.5


//char shared[2];
sem_t s1, s2, s3;
void *status;
FILE *fftemp;  //file names that we're gonna receive sensor values
FILE *ffmois;
FILE *ffpres;

    int *T1(){ //implementations for plant temparature
        int counter = 0; 
        fftemp=fopen("temperature.txt", "r");
        int value[100];
        sem_wait(&s1);
        while(!feof(fftemp)) {
            sleep(2);
            fgets(value,100,fftemp);
            //scanf(“%c”,&my_data); // simulate input
            //shared[0]=1;
            //shared[1]=my_data;
            counter++;
        }
        fclose(fftemp);
        for(int i = 0; i < 100; i++){
            if(value[i] < MINTEMP && value[i] > MAXTEMP){
                printf(“\nThere is temperature issue with that plant : %d”, i);
                sem_post(&s3);
                return 0;
            }
            else {
                sem_post(&s3);
                return 1;
            }
        }
        pthread_exit(NULL);
    }
    int *T2(){ //implementations for plant moisture
        int counter = 0; 
        ffmois=fopen("moisture.txt", "r");
        int value[100];
        sem_wait(&s2);
        while(!feof(ffmois)) {
            sleep(2);
            //scanf(“%c”,&my_data); // simulate input
            fgets(value,100,ffmoisture);
            /*shared[0]=2;
            shared[1]=my_data;*/
            counter++;
        }
        fclose(ffmois);
        for(int i = 0; i < 100; i++){
            if(value[i] < MINHUM && value[i] > MAXHUM) {
                printf(“\nThere is moisture issue with that plant : %d”, i);
                sem_post(&s3);
                return 0;
            }
            else {
                sem_post(&s3);
                return 1;
            }
        }
        pthread_exit(NULL);           
    }
    int *T3(){ //implementations for plant pressure
        int counter=0;
        ffpres=fopen("pressure.txt", "r");
        int value[100];
        sem_wait(&s3);
        while(!feof(ffpres)) {
            sleep(3);
            fgets(value,100,ffpres);
            /*type= shared[0];
            if (type==1) {
                printf(“ temperature %d”, shared[1]);
                sem_post(&s1);
            } else {
                printf(“pressure %d”, shared[1]);
                }*/
                counter++;
        }
        fclose(ffpres);
        for(int i = 0; i < 100; i++){
            if(value[i] < MINPRES && value[i] > MAXPRES) {
               printf(“\nThere is pressure issue with that plant : %d”, counter); 
               sem_post(&s1);
               return 0;
            }
            else {
               sem_post(&s1);
               return 1;
            }
        }
        pthread_exit(NULL);
    }
int main(){
    sem_init(&s1, 0,1); // initialize semaphores
    sem_init(&s2, 0,1); // let sensors go
    sem_init(&s3, 0,0); // writer should wait
    pthread_t t1,t2,t3;
    pthread_create(&t1, NULL, T1, NULL);
    pthread_create(&t2, NULL, T2, NULL);
    pthread_create(&t3, NULL, T3, NULL);
    pthread_join(t1, &status); // will not come here
    pthread_join(t2, &status);
    pthread_join(t3, &status);
    printf("You BETTER check plant which you could reach by numbers !");
    return 0;
}
