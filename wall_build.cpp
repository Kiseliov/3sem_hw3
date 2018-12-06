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

int width = 5;
int height = 2;
int min_time[2] = {1,2};
int max_time[2] = {2,3};
pthread_mutex_t mutex;
string *wall;

void show_wall() {
	cout << endl;
	for(int i = 0; i <= height; i++){
		cout << wall[height-i] << endl;
	}
	cout << endl;
}

void build_brick(int x, int y, int son){
	while(wall[y-1][2*x+2] == ' ');
	usleep((min_time[son] + rand()%(max_time[son]-min_time[son]))*10000);
	wall[y][2*x - 1 + son] = '[';
	wall[y][2*x + son] = ']';
	pthread_mutex_lock(&mutex);
	show_wall();
	pthread_mutex_unlock(&mutex);
}

void *work_func(void *arg) {

	int son = *((int*)arg);
	cout << son <<" thread" << endl;
	for(int y = 1 + son; y <= height; y += 2){
		if(son){ // build first half of brick; only for son as he always build odd lines
			while(wall[y-1][2] == ' ');
			usleep((min_time[son] + rand()%(max_time[son]-min_time[son]))*10000);
			wall[y][1] = 'h';
			pthread_mutex_lock(&mutex);
			show_wall();
			pthread_mutex_unlock(&mutex);
		}
		
		for(int x = 1; x <= width - son; x++){
			build_brick(x, y, son);
		}

		if(son){ // build last half of brick; only for son as he always build odd lines
			while(wall[y-1][2*width] == ' ');
			usleep((min_time[son] + rand()%(max_time[son]-min_time[son]))*10000);
			wall[y][2*width] = 'h';
			pthread_mutex_lock(&mutex);
			show_wall();
			pthread_mutex_unlock(&mutex);
		}
	}
	printf("%d is done\n", son);
}

int main(){
	scanf("%d %d \n %d %d \n %d %d", &width, &height, max_time, min_time, max_time+1, min_time+1);
	wall = (string*) malloc((height+1) * sizeof(string));
	for(int i = 0; i < height + 1; i++){
		for(int j = 0; j < 2*width + 1; j++){
			if(j == 0 || i == 0) wall[i].push_back('#');
			else wall[i].push_back(' ');
		}
	}
	pthread_t builders[2];
	show_wall();
	pthread_mutex_init(&mutex, NULL);
	int arg[2] = {0,1};
	cout <<arg[0] << arg[1] <<endl;
	pthread_create(builders, NULL, work_func, (void*)(arg));
	pthread_create(builders+1, NULL, work_func, (void*)(arg+1));
	pthread_join(builders[1], NULL);
	pthread_join(builders[0], NULL);
}