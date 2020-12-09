#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

#define resourceTypeQuan 3
#define processQuan 5

//global resources
int i = 0;
int j = 0;
pthread_mutex_t mutex;//mutex lock for access to global variable

//data structures to hold information about resources allocated to a process
int availResourceVector [resourceTypeQuan];
int allocMatrix [processQuan][resourceTypeQuan] = {{1,1,0},{1,3,0},{0,0,2},{0,1,1},{0,2,0}};
int maxMatrix [processQuan][resourceTypeQuan] = {{5,5,5},{3,3,6},{3,5,3},{7,1,4},{7,2,2}};
int needMatrix [processQuan][resourceTypeQuan];

//function declaration
int requestResource(int processID,int requestVector[]);
int releaseResource(int processID,int releaseVector[]);
int ifGreaterThanNeed(int processID,int requestVector[]);
int ifEnoughToRelease(int processId, int releaseVector[]);
int ifInSafeMode();
int ifEnoughToAlloc(int []);
void printNeedMatrix();
void printAllocMatrix();
void printAvailable();
void printReqOrRelVector(int vec[]);
void *customer(void* customerID);

int main(){//main function
	//int argv[] = {5,5,5};//available number of resources
	srand(time(NULL));
	for(i = 0; i < resourceTypeQuan; i++){
		availResourceVector[i] = rand() % 10 + 7;
	}
	//initialize needMatrix
	for (i = 0; i < processQuan; ++i){
		for (j = 0; j < resourceTypeQuan; ++j){
			needMatrix[i][j] = maxMatrix[i][j] - allocMatrix[i][j];
		}
	}
	printf("Initial Available Vector:\n");
	printAvailable();
	printf("Initial Allocation Matrix:\n");
	printAllocMatrix();
	printf("Initial Need Matrix:\n");
	printNeedMatrix();

	//Multi Threading Setup
	pthread_mutex_init(&mutex,NULL);
	pthread_attr_t attrDefault;
	pthread_attr_init(&attrDefault);
	pthread_t *tid = (pthread_t*)malloc(sizeof(pthread_t) * processQuan);
	int *pid = (int*)malloc(sizeof(int)*processQuan);//customer's ID
	//initialize pid and create threads
	for(i = 0; i < processQuan; i++){
		*(pid + i) = i;
		pthread_create((tid+i), &attrDefault, customer, (pid+i));
	}
	//join threads
	for(i = 0; i < processQuan; i++){
		pthread_join(*(tid+i),NULL);
	}
	return 0;
}

void *customer(void* customerID){//function that simualtes request and release of resorces
	int processID = *(int*)customerID;//get process id
	int counter = 2;
	while(counter--){
		sleep(1);
		int requestVector[resourceTypeQuan];//request random number of resources
		pthread_mutex_lock(&mutex);//lock mutex for accessing global variable and printf
		//initialize requestVector using rand()
		for(i = 0; i < resourceTypeQuan; i++){
			if(needMatrix[processID][i] != 0){
				requestVector[i] = rand() % needMatrix[processID][i];
			}
			else{
				requestVector[i] = 0;
			}
		}
		printf("\nCustomer %d Requesting Resources:\n",processID);
		printReqOrRelVector(requestVector);
		requestResource(processID,requestVector);
		pthread_mutex_unlock(&mutex);//unlock mutex
		
		//release random number of resources		
		sleep(rand()%5 + 1);
		int releaseVector[resourceTypeQuan];
		//lock mutex for accessing global variable and printf
		pthread_mutex_lock(&mutex);
		//initialize releaseVector
		for(i = 0; i < resourceTypeQuan; i++){
			if(allocMatrix[processID][i] != 0){
				releaseVector[i] = rand() % allocMatrix[processID][i];
			}
			else{
				releaseVector[i] = 0;
			}
		}
		printf("\nCustomer %d Releasing Resources:\n",processID);
		printReqOrRelVector(requestVector);
		releaseResource(processID,requestVector);
		
		pthread_mutex_unlock(&mutex);//unlock mutex
	}
}

int requestResource(int processID,int requestVector[]){//allocate resources to a process
	//whether request number of resources is greater than needed
	if (ifGreaterThanNeed(processID,requestVector) == -1){
		printf("requested resources bigger than needed.\n");
		return -1;
	}
	printf("Simulating Allocation..\n");

	//whether request number of resources is greater than needed
	if(ifEnoughToAlloc(requestVector) == -1){
		printf("Not enough resources\n");
		return -1;
	}

	//pretend allocated
	for (i = 0; i < resourceTypeQuan; ++i){
		needMatrix[processID][i] -= requestVector[i];
		allocMatrix[processID][i] += requestVector[i];
		availResourceVector[i] -= requestVector[i];
	}
	
	//check if still in safe status
	if (ifInSafeMode() == 0){
		printf("Safe. Allocated successfully.\nAvailable Resources:\n");
		printAvailable();
		printf("Allocated matrix:\n");
		printAllocMatrix();
		printf("Need matrix:\n");
		printNeedMatrix();
		return 0;
	}
	else{
		printf("Rolling back\n");
		for (i = 0; i < resourceTypeQuan; ++i){
			needMatrix[processID][i] += requestVector[i];
			allocMatrix[processID][i] -= requestVector[i];
			availResourceVector[i] += requestVector[i];
		}
		return -1;
	}
}

int releaseResource(int processID,int releaseVector[]){//release the resources allocated
	if(ifEnoughToRelease(processID,releaseVector) == -1){//to a process
		printf("Process do not own enough resources\n");
		return -1;
	}

	//enough to release
	for(i = 0; i < resourceTypeQuan; i++){
		allocMatrix[processID][i] -= releaseVector[i];
		needMatrix[processID][i] += releaseVector[i];
		availResourceVector[i] += releaseVector[i];
	}
	printf("Release successfully.\nAvailable Resources:\n");
	printAvailable();
	printf("Allocated matrix:\n");
	printAllocMatrix();
	printf("Need matrix:\n");
	printNeedMatrix();
	return 0;
}

int ifEnoughToRelease(int processID,int releaseVector[]){//check if released resources
	for (i = 0; i < resourceTypeQuan; ++i){				 //are more than allocated 
		if (releaseVector[i] <= allocMatrix[processID][i]){
			continue;
		}
		else{
			return -1;
		}
	}
	return 0;
}

int ifGreaterThanNeed(int processID,int requestVector[]){//check if the requested resorces
	for (i = 0; i < resourceTypeQuan; ++i){				 //are more than needed
		if (requestVector[i] <= needMatrix[processID][i]){
			continue;
		}
		else{
			return -1;
		}
	}
	return 0;
}

int ifEnoughToAlloc(int requestVector[]){//check if there are enough resources to allocate
	for (i = 0; i < resourceTypeQuan; ++i){
		if (requestVector[i] <= availResourceVector[i]){
			continue;
		}
		else{
			return -1;
		}
	}
	return 0;
}

void printNeedMatrix(){//print need matrix
	for (i = 0; i < processQuan; ++i){
		printf("{ ");
		for (j = 0; j < resourceTypeQuan; ++j){
			printf("%d, ", needMatrix[i][j]);
		}
		printf("}\n");
	}
	return;
}

void printAllocMatrix(){//print Allocation Matrix
	for (i = 0; i < processQuan; ++i){
		printf("{ ");
		for (j = 0; j < resourceTypeQuan; ++j){
			printf("%d, ", allocMatrix[i][j]);
		}
		printf("}\n");
	}
	return;
}

void printAvailable(){//print number of available resources
	for (i = 0; i < resourceTypeQuan; ++i){
		printf("%d, ",availResourceVector[i]);
	}
	printf("\n");
	return;
}

void printReqOrRelVector(int vec[]){//print Request/Release Vector
	for (i = 0; i < resourceTypeQuan; ++i){
		printf("%d, ",vec[i]);
	}
	printf("\n");
	return;
}

int ifInSafeMode(){//checks if a safe sequence exists if resource allocation occurs
	int ifFinish[processQuan] = {0};
	int work[resourceTypeQuan];//temporary available resources vector
	for(i = 0; i < resourceTypeQuan; i++){
		work[i] = availResourceVector[i];
	}
	int k;
	for(i = 0; i < processQuan; i++){
		if (ifFinish[i] == 0){
			for(j = 0; j < resourceTypeQuan; j++){
				if(needMatrix[i][j] <= work[j]){
					if(j == resourceTypeQuan - 1){
						ifFinish[i] = 1;
						for (k = 0; k < resourceTypeQuan; ++k){
							work[k] += allocMatrix[i][k];
							//execute and release resources
						}
						i = -1;
						break;
					}
					else{
						continue;
					}
				}
				else{
					break;
				}
			}
		}
		else{
			continue;
		}
	}
	for(i = 0; i < processQuan; i++){
		if (ifFinish[i] == 0){
			return -1;
		}
		else{
			continue;
		}
	}
	return 0;
}