#include <stdio.h> 
#include <pthread.h> 
#include <stdlib.h>
#include <unistd.h>

pthread_mutex_t read_mutex;
pthread_mutex_t write_mutex;

void * writeTest(void *temp) {
  char *ret;
  FILE *file1;
  char *str;
  pthread_mutex_lock(&write_mutex);
  sleep(5);
  pthread_mutex_lock(&read_mutex);
  printf("\nFile locked, please enter the message \n");
  str=(char *)malloc(10*sizeof(char));
  file1=fopen("temp","w");
  scanf("%s",str);
  fprintf(file1,"%s",str);
  fclose(file1);
  pthread_mutex_unlock(&read_mutex);
  pthread_mutex_unlock(&write_mutex);
  printf("\nUnlocked the file you can read it now \n");
  return ret;
}


void * readTest(void *temp) {
  char *ret;
  FILE *file1;
  char *str;
  pthread_mutex_lock(&read_mutex);
  sleep(5);
  pthread_mutex_lock(&write_mutex);
  printf("\n Opening file \n");
  file1=fopen("temp","r");
  str=(char *)malloc(10*sizeof(char));
  fscanf(file1,"%s",str);
  printf("\n Message from file is %s \n",str);

  fclose(file1);

  pthread_mutex_unlock(&write_mutex);
  pthread_mutex_unlock(&read_mutex);
  return ret;
}




int main() {
  pthread_t thread_id,thread_id1;
  pthread_attr_t attr;
  int ret;
  void *res;
  ret=pthread_create(&thread_id,NULL,&writeTest,NULL);
  ret=pthread_create(&thread_id1,NULL,&readTest,NULL);
  printf("\n Created thread");
  pthread_join(thread_id,&res);
  pthread_join(thread_id1,&res);
}
