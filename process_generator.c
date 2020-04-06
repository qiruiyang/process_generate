/*
 * CSE 330 Spring 19'
 * User space program for project 2-1
 * This program creates 8 processes with different scheduling policies
 * There are 1 default policy (TS), 1 FIFO policy (FF), 2 Round Robin policy (RR)
 * and 4 Batch policy (B).
 */
#define _GNU_SOURCE

#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>

#define CURR_THREAD 0

int main(int argc, char **argv)
{
	int ret, ret_child_1, ret_child_2, ret_child_3, ret_child_4, scheduler;
	struct sched_param param;
	struct timeval start, now;
	long long unsigned i = 0;
	
	if (argc != 2 || (strncmp(argv[1], "normal", 6) && strncmp(argv[1], "rr", 2) && strncmp(argv[1], "fifo", 4))) {
		fprintf(stderr, "error selecting scheduler, make sure to choose normal or rr or fifo\n");
		return -1;
	}

	if (!strncmp(argv[1], "rr", 2))
		scheduler = SCHED_RR;
	else if (!strncmp(argv[1], "fifo", 4))
		scheduler = SCHED_FIFO;
	else
		scheduler = SCHED_OTHER;

	/*
	 * Retrieves the scheduling parameters from the current process.
	 */
	gettimeofday(&start, NULL);
	ret = sched_getparam(CURR_THREAD, &param);
	if (scheduler == SCHED_RR || scheduler == SCHED_FIFO)
		param.sched_priority = 94;
	if (sched_setscheduler(CURR_THREAD, scheduler, &param) == -1) {
		fprintf(stderr, "error setting scheduler, make sure you are root\n");
		return -1;
	}

	while (1) {
		i++;
		gettimeofday(&now, NULL);
		if ((now.tv_sec * 1000000 + now.tv_usec -
				start.tv_sec * 1000000 - start.tv_usec) >= 3200000)
			break;
	}

	/*
	 * Call fork() to create a child process. You now have two processes.
	 */
	ret_child_1 = fork();

	/*
	 * The fork() system call returns 0 to the child process and the pid to the
	 * parent process.
	 */
	if (ret_child_1 == 0){
		printf("Child 1-%d. My pid is %d and my parent's pid is %d\n", getpid(),
			getpid(), getppid());

		if (scheduler == SCHED_RR || scheduler == SCHED_FIFO)
	 		param.sched_priority = 94;

		/*
		 * Change the child 1 process to FIFO scheduling policy.
		 * The sched_setscheduler() returns 0 on success and -1 on failure.
		 */
		if (sched_setscheduler(CURR_THREAD, scheduler, &param) == -1) {
			fprintf(stderr, "error setting scheduler, make sure you are root\n");
		}
	}

	/*
	 * Call fork() to create a child process. You now have four processes.
	 */
	ret_child_2 = fork();

	if (ret_child_2 == 0){
		printf("Child 2-%d. My pid is %d and my parent's pid is %d\n", getpid(),
			getpid(), getppid());
		
		if (scheduler == SCHED_RR || scheduler == SCHED_FIFO)
			param.sched_priority = 95;

		/*
		 * Change the child 2 processes to Round Robin scheduling policy.
		 */
		if (sched_setscheduler(CURR_THREAD, scheduler, &param) == -1) {
			fprintf(stderr, "error setting scheduler, maker sure you are root\n");
		}

	}

	/*
	 * Finally, we call fork() again. You now have eight processes.
	 */
	ret_child_3 = fork();
	if (ret_child_3 == 0){
		printf("Child 3-%d. My pid is %d and my parent's pid is %d\n", getpid(),
			getpid(), getppid());

		if (scheduler == SCHED_RR || scheduler == SCHED_FIFO)
			param.sched_priority = 96;

		/*
		 * Change the child 3 processes to batch.
		 */
		if (sched_setscheduler(CURR_THREAD, scheduler, &param) == -1) {
			fprintf(stderr, "error setting scheduler, maker sure you are root\n");
		}
	}

    /*
	 * Finally, we call fork() again. You now have eight processes.
	 */
	ret_child_4 = fork();
	if (ret_child_4 == 0){
		printf("Child 4-%d. My pid is %d and my parent's pid is %d\n", getpid(),
			getpid(), getppid());

		if (scheduler == SCHED_RR || scheduler == SCHED_FIFO)
			param.sched_priority = 97;

		/*
		 * Change the child 3 processes to batch.
		 */
		if (sched_setscheduler(CURR_THREAD, scheduler, &param) == -1) {
			fprintf(stderr, "error setting scheduler, maker sure you are root\n");
		}
	}

	/*
	 * Sleep for 30 seconds so that you can check the result with another
	 * terminal.
	 */
	sleep(30);

	return 0;
}
