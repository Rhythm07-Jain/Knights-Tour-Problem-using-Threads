/*
Group 23 
Gaurav Kumar:   2019B3A71324H
Rhythm Sethi:   2019B3A71306H
Ishan Rastogi:  2019B3A71305H
Abhinav Bansal: 2019B3A71293H
Siddhant Garg:  2019B5A70760H
Satyam Gupta:   2019B3A71277H
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
 
int N;
int *path;

int checkValidity(int path[], int x, int y, int N);
int countMoves(int path[], int x, int y,int N);
void* threadFunc(void * arg);
int nextMove(int path[],int N, int *x, int *y);
int isComplete(int arr[],int N);
int solve(int N,int i,int j);

int main(int argc,char *argv[])
{
    if (argc != 4) {
        printf("Usage: ./Knight.out grid_size StartX StartY");
        exit(-1);
    }
    N = atoi(argv[1]);
    int StartX=atoi(argv[2]);
    int StartY=atoi(argv[3]);

    /* Do your thing here */

    int k=0;
    while (!solve(N,StartX,StartY) && k<N) k++;
    printf("No Possible Tour\n");
    return 0;
}

int solve(int N,int i,int j)
{
    int size = sizeof(int);
    path=(int *)malloc(N*N*size);
    int *path_temp[2];
    path_temp[0]=(int *)malloc(N*N*size);
    path_temp[1]=(int *)malloc(N*N*size);
    for (int k = 0; k< N*N; ++k) path[k] = -1;
 
    int x = i;
    int y = j;
    path[y*N+x] = 1;

    path_temp[0][0]=x;
    path_temp[1][0]=y;
    for (int k = 0; k < N*N - 1; ++k)
        if (!nextMove(path, N,&x, &y) == 0){
            path_temp[0][k+1]=x;
            path_temp[1][k+1]=y;
        }
        else return 0;

    if(isComplete(path,N)){
        int i=0;	
        while(i<N){
            int j = 0;
            while(j<N){
                printf("%d,%d|",path_temp[0][i*N+j],path_temp[1][i*N+j]);
                j++;
            }
            i++;
        } 
        exit(0);
    }
    return 1;
}
 
int checkValidity(int path[], int x, int y, int N){
    if(!(x >= 0 && x < N && y >= 0 && y < N && path[y*N+x] < 0)) return 0;
    return 1;
}
 
struct argStr{
    int c[2];
    int k;
    int count;
};

int adjMove[2][8]={{1,1,2,2,-1,-1,-2,-2},{2,-2,1,-1,2,-2,1,-1}};
 
void* threadFunc(void * arg){
        struct argStr* tempObj=(struct argStr*)arg;
        int nextX = (tempObj->c[0])+ adjMove[0][tempObj->k];
        int nextY = (tempObj->c[1])+ adjMove[1][tempObj->k];
        if ((checkValidity(path, nextX, nextY,N)) ){
            tempObj->count=0;
            if (checkValidity(path, nextX+1, nextY+2, N)) tempObj->count++;
            if (checkValidity(path, nextX+1, nextY-2, N)) tempObj->count++;
            if (checkValidity(path, nextX+2, nextY+1, N)) tempObj->count++;
            if (checkValidity(path, nextX+2, nextY-1, N)) tempObj->count++;
            if (checkValidity(path, nextX-1, nextY+2, N)) tempObj->count++;
            if (checkValidity(path, nextX-1, nextY-2, N)) tempObj->count++;
            if (checkValidity(path, nextX-2, nextY+1, N)) tempObj->count++;
            if (checkValidity(path, nextX-2, nextY-1, N)) tempObj->count++;
        }
        pthread_exit(NULL);
        return NULL;
}

pthread_t tID[8];
struct argStr tempObj[8];

int nextMove(int path[],int N, int *x, int *y)
{
    int minMovesIndex = -1, minMoves = _INT_MAX_, next[2];
    int random = rand()%(8);
    int i=0;
    while(i<8){
        tempObj[i].c[0]=*x;
        tempObj[i].c[1]=*y;
        tempObj[i].k=(i+random)%8;
        tempObj[i].count=_INT_MAX_;
        pthread_create(&tID[i],NULL,threadFunc,&tempObj[i]);
        i++;
    }
	i = 0;
    while(i<8){
        pthread_join(tID[i],NULL);
        if(tempObj[i].count<minMoves){
            minMovesIndex=(i+random)%8;
            minMoves=tempObj[i].count;
        }
        i++;
    }

    if (minMovesIndex == -1) return 0;

    next[0] = *x + adjMove[0][minMovesIndex];
    next[1] = *y + adjMove[1][minMovesIndex];
    path[next[1]*N + next[0]] = path[(*y)*N + (*x)]+1;
    *x = next[0];
    *y = next[1];
    
    return 1;
}
 
int isComplete(int arr[],int N){
    int i = 0;
    while(i<N)
    {
    	int j=0;
        while(j<N){
        	if(path[i*N+j]==-1) return 0;
        	j++;
        }
        i++;
    }
    return 1;
}