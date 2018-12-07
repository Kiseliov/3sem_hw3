#include <stdlib.h>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <algorithm>
#include <ctime>
#include <iostream>
#include <string>
#include <dirent.h>
#include <signal.h>
#include <cstddef>
#include <cstring>
#include <utime.h>
#include <vector>
#include <pthread.h>
#include <iostream>
#include <sys/types.h> 

using namespace std;

struct thread_args{
	int width = 5;
	int height = 2;
	int min_time[2] = {1,2};
	int max_time[2] = {2,3};
	pthread_mutex_t mutex[3];
	int son;
	string *wall;
};

void show_wall(string* wall, int height) {
	cout << endl;
	for(int i = 0; i <= height; i++){
		cout << wall[height-i] << endl;
	}
	cout << endl;
}

void build_brick(int x, int y,struct thread_args *args){
	while(args->wall[y - 1][2*(x+args->son)+(1-args->son)] == ' '){
		pthread_mutex_lock(args->mutex+1-args->son);
		pthread_mutex_unlock(args->mutex+1-args->son);
	}
	pthread_mutex_lock(args->mutex+args->son);
	usleep((args->min_time[args->son] + rand()%(args->max_time[args->son]-args->min_time[args->son]))*10000);
	args->wall[y][2*x + args->son - 1] = '[';
	args->wall[y][2*x + args->son] = ']';
	pthread_mutex_lock(args->mutex+2);
	show_wall(args -> wall, args -> height);
	pthread_mutex_unlock(args->mutex+2);
	pthread_mutex_unlock(args->mutex+args->son);
}

void *work_func(void *arg) {
	struct thread_args args = *((struct thread_args*)arg);
	for(int y = 1 + args.son; y <= args.height; y += 2){
		if(args.son){ // build first half of brick; only for son as he always build odd lines
			while(args.wall[y - 1][1] == ' '){
				pthread_mutex_lock(args.mutex+1-args.son);
				pthread_mutex_unlock(args.mutex+1-args.son);
			}
			pthread_mutex_lock(args.mutex+args.son);
			usleep((args.min_time[args.son] + rand()%(args.max_time[args.son]-args.min_time[args.son]))*10000);
			args.wall[y][1] = 'h';
			pthread_mutex_lock(args.mutex+2);
			show_wall(args.wall, args.height);
			pthread_mutex_unlock(args.mutex+2);
			pthread_mutex_unlock(args.mutex+args.son);
		}
		for(int x = 1; x <= args.width - args.son; x++){
			build_brick(x, y, &args);
		}

		if(args.son){ // build last half of brick; only for son as he always build odd lines
			while(args.wall[y - 1][args.width*2] == ' '){
				pthread_mutex_lock(args.mutex+1-args.son);
				pthread_mutex_unlock(args.mutex+1-args.son);
			}
			pthread_mutex_lock(args.mutex+args.son);
			usleep((args.min_time[args.son] + rand()%(args.max_time[args.son]-args.min_time[args.son]))*10000);
			args.wall[y][args.width*2] = 'h';
			pthread_mutex_lock(args.mutex+2);
			show_wall(args.wall, args.height);
			pthread_mutex_unlock(args.mutex+2);
			pthread_mutex_unlock(args.mutex+args.son);
		}
	}
	printf("%d is done\n", args.son);
}

int main(){
	struct thread_args arguments;
	scanf("%d %d \n %d %d \n %d %d", &arguments.width, &arguments.height, arguments.max_time, arguments.min_time, &arguments.max_time[1], &arguments.min_time[1]);
	arguments.wall = (string*) malloc((arguments.height+1) * sizeof(string));
	for(int i = 0; i < arguments.height + 1; i++){
		for(int j = 0; j < 2*arguments.width + 1; j++){
			if(j == 0 || i == 0) arguments.wall[i].push_back('#');
			else arguments.wall[i].push_back(' ');
		}
	}
	arguments.son = 0;
	pthread_t builders[2];
	show_wall(arguments.wall, arguments.height);
	for(int i = 0; i<3; i++){
		pthread_mutex_init(arguments.mutex+i, NULL);
	}
	pthread_create(builders, NULL, work_func, (void*)(&arguments));
	usleep(100000);
	arguments.son = 1;
	pthread_create(builders+1, NULL, work_func, (void*)(&arguments));
	pthread_join(builders[1], NULL);
	pthread_join(builders[0], NULL);
}