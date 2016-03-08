/************************************************************/

/************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define N 26 // Total number of threads (in addition to main thread).
#define M 10 // Number of loop iterations per thread.
int sum = 0; // Data shared by all the threads.
int flag[N] = {-1};
int turn[N-1] = {-1};
// The function executed by each thread.
void *runner(void *param) {
	int i = *(int *)param;
	
	for(int m = 0; m < M; m++) {
		for(int j =0;j<N-1;j++){
			flag[i] = j;
			turn[j] = i; // 阻塞区
			int k =(i+1)%N;
			while(k!=i){
				if(flag[k]>=j){
					break;
				}
				k = (k+1)%N;
			}
			while(turn[j]==i&&k!=i&&flag[k]>=j);
		}
		
		int s = sum;
		// Even threads increase s, odd threads decrease s.
		if(i % 2 == 0) {
			s++;
		} else {
			s—;
		}
		// Sleep a small random amount of time.
		struct timespec delay;
		delay.tv_sec = 0;
		delay.tv_nsec = 100000000ULL * rand() / RAND_MAX;
		nanosleep(&delay, NULL);
		sum = s;
		printf("%c", 'A'+i);
		fflush(stdout);
	}
	flag[i] = -1;
	pthread_exit(0); // Thread dies.
	
}

int main(void) {
	pthread_t tid[N]; // Thread IDs.
	int param[N]; // One parameter for each thread.
	
	// Create N threads, which each call the runner function with
	// i as argument.
	
	for(int i = 0; i < N; i++) {
		param[i] = i;
		pthread_create(&tid[i], NULL, runner, &param[i]);
	}
	// Wait for N threads to finish.
	for(i = 0; i < N; i++) {
		pthread_join(tid[i], NULL);
	}
	printf("\nResult is %d\n", sum);
	return 0;
}
/************************************************************/