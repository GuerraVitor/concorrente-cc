#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* routine() {
	printf("test from thread\n");
}

int main(int ac, char **av){
	pthread_t t1;
	pthread_create(&t1, NULL, &routine, NULL);
	pthread_join(
	return 0;
}
