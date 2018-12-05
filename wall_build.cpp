#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

using namespace std;

const int THE_END = -1;
long widht = 5;
long height = 2;
long min_son = 2;
long max_son = 4;
long min_father = 1;
long max_father = 3;
pthread_mutex_t mutex;
string *their_wall;

void show_wall() {
	cout << endl;
	for(int i = 0; i < height; i++){
		cout << their_wall[i] << endl;
	}
	cout << endl;
}

void* father(void* arg) {
	long x = THE_END, y;
	for (;;) {
		pthread_mutex_lock(&mutex);
		if (x >= 0) {
			their_wall[x] = their_wall[x] + 'F';
			print_their_wall();
		}
		x = THE_END;
		for (long m = 1; m =< height; m += 2) {
			for (long n = 0; ; ++n) {
				long temp;
				temp = (m)%2 + n*2
				if (temp > width*2)
					break;
				else if (their_wall[m][temp] != ' ')
					continue; 
				else if ((their_wall[m-1][temp] != ' ') && (their_wall[m-1][temp+1] != ' ')) {
					x = m;
					y = temp;
					m = height;
					break;
				}
				else {
					pthread_mutex_unlock(&mutex);
					usleep(max_son*10000);
					pthread_mutex_lock(&mutex);
					n--;
					m = m - 2;
				}
			}
		}
		pthread_mutex_unlock(&mutex);
		if (x != THE_END)
			usleep((min_father + rand() % (max_father - min_father)) * 10000);
		else return NULL;
		
	}
}


void* son(void* arg) {
	long x = THE_END, y;
	for (;;) {
		pthread_mutex_lock(&mutex);
		if (x >= 0) {
			their_wall[x] = their_wall[x] + 'S';
			print_their_wall();
		}
		x = THE_END;
		for (long m = 2; m =< height; m += 2) {
			for (long n = 0; ; ++n) {
				long temp;
				temp = m%2 + n*2
				if (temp > width*2)
					break;
				else if (their_wall[m][temp] != ' ')
					continue; 
				else if ((their_wall[m-1][temp] != ' ') && (their_wall[m-1][temp+1] != ' ')) {
					x = m;
					y = temp;
					m = height;
					break;
				}
				else {
					pthread_mutex_unlock(&mutex);
					usleep(max_father*10000);
					n--;
					m = m - 2;
					pthread_mutex_lock(&mutex);
				}
			}
		}
		pthread_mutex_unlock(&mutex);
		if (x != THE_END)
			usleep((min_son + rand() % (max_son - min_son)) * 10000);
		else return NULL;
	}
}

int main{
	sscanf("%d %d \n %d %d \n %d %d", &widht, &height, &min_father, &max_father, &min_son, &max_son);
	their_wall = (string*) malloc((Height+1) * sizeof(string));
	for (size_t i = 0; i <= Height; ++i) {
		for (size_t j = 0; j <= 2*Width+1; ++j)
			their_wall[i][j] = ' ';
		their_wall[i][(Width+1)*2+1] = 0;
	}
	for (size_t j = 0; j <= 2*Width+1; ++j)
		their_wall[0][j] = '#';
	for (size_t i = 1; i <= Height; i += 2) {
		their_wall[i][0] = '#';
		their_wall[i][2*Width+1] = '#';
	}
	show_wall();
	pthread_mutex_init(&mutex, NULL);
	pthread_t son, father;
	pthread_create(&father, NULL, father, NULL);
	pthread_create(&son, NULL, son, NULL);
	pthread_join(son, NULL);
	pthread_join(father, NULL);
}