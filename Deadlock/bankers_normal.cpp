#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>
#include <iostream>

#define RESOURCE_COUNT 5
#define PROCESS_COUNT 5

//data structures to hold information about resources allocated to a process
int available_resources [RESOURCE_COUNT];
int allocation_matrix [PROCESS_COUNT][RESOURCE_COUNT] = {{1,1,0,1,2},{1,2,0,1,0},{0,0,2,0,0},{0,1,1,2,1},{0,2,0,1,2}};
int max_demand_matrix [PROCESS_COUNT][RESOURCE_COUNT] = {{5,5,5,5,5},{3,3,6,3,3},{3,5,3,5,1},{7,1,4,2,5},{7,2,2,1,4}};
int need_matrix [PROCESS_COUNT][RESOURCE_COUNT];
int needed_time[PROCESS_COUNT];
int is_finished[PROCESS_COUNT] = {0};
int* indexes;
int completed_processes = 0;
int final_running_sequece[PROCESS_COUNT];

void simulate();
int* shuffleIndexes();
void swap(int* a, int* b);
void print_need_matrix();
void print_allocation_matrix();
void print_available();
int is_safe();
int request_resource(int process_ID,int request_vector[]);
int release_resource(int process_ID);
int is_greater_than_need(int process_ID,int request_vector[]);
int is_enough_to_allocate(int request_vector[]);

using namespace std;
int main(int argc, char *argv[]){//main function
    if (argc <= RESOURCE_COUNT){
        printf("not enough arguments\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));
    
    for (int i = 0; i < RESOURCE_COUNT; i++){
        available_resources[i] = strtol(argv[i + 1], NULL, 10);
    }

    //calculate need_matrix
	for (int i = 0; i < PROCESS_COUNT; ++i){
		for (int j = 0; j < RESOURCE_COUNT; ++j){
			need_matrix[i][j] = max_demand_matrix[i][j] - allocation_matrix[i][j];
		}
	}

    cout<<"Needed time:\n";
    //random time
    for (int i = 0; i < PROCESS_COUNT; ++i){
        needed_time[i] = rand()%5 + 1;
        printf("%d ", needed_time[i]);
    }
    puts("");


    indexes = shuffleIndexes();
    cout<<"Shuffled indexes\n";
    for(int i=0; i<RESOURCE_COUNT; i++)
        cout<<indexes[i]<<" ";
    cout<<endl;


    cout<<"Allocation matrix:\n";
    print_allocation_matrix();

    cout<<"Available matrix:\n";
    print_available();

    cout<<"Need matrix:\n";
    print_need_matrix();

    cout<<"Start Simulation\n\n";
    simulate();

    cout<<"--------------------------------\nFinal sequence:\n";
    for(int i=0; i<PROCESS_COUNT;i++)
        cout<<final_running_sequece[i]<<" ";
    cout<<endl;

    return 0;
}
void swap(int* a, int* b){

    int temp = *a;
    *a = *b;
    *b = temp;
}
int* shuffleIndexes(){
    static int indexes[PROCESS_COUNT];
    for(int i=0; i < PROCESS_COUNT; i++)
        indexes[i] = i;

    for(int i=0; i< PROCESS_COUNT; i++)
        swap(&indexes[i], &indexes[rand()%PROCESS_COUNT]);

    return indexes;
}
void simulate(){
    int safe;
 
    int processID;
    for(int i=0; completed_processes < PROCESS_COUNT ; i=(i+1)%PROCESS_COUNT){
        processID = indexes[i];
        
        if(!is_finished[processID]){
            int req_vector[RESOURCE_COUNT];
            for(int j=0; j<RESOURCE_COUNT; j++)
                req_vector[j] = need_matrix[processID][j];

            if(request_resource(processID, req_vector) == 0){
                printf("\nRunning for %d seconds...\n", needed_time[processID]);
                sleep(needed_time[processID]);
                release_resource(processID);

                final_running_sequece[completed_processes] = processID;
                completed_processes++;
                is_finished[processID] = 1;
                cout<<"################################END##################################"<<endl;
            }
        }
 
 
    }
 

}

int request_resource(int process_ID,int request_vector[]){

    printf("***************************Checking Process%d ...\n", process_ID);
    
	if (is_greater_than_need(process_ID,request_vector) == -1){
		printf("[#] Request bigger than needed.\n");
		return -1;
	}

	//whether request number of resources is greater than needed
	if(is_enough_to_allocate(request_vector) == -1){
		printf("Not enough resources\n");
		return -1;
	}

	//pretend allocated
	for (int i = 0; i < RESOURCE_COUNT; ++i){
		need_matrix[process_ID][i] -= request_vector[i];
		allocation_matrix[process_ID][i] += request_vector[i];
		available_resources[i] -= request_vector[i];
	}
	
	//check if still in safe status
	if (is_safe() == 0){

        printf("***************************RUNNING Process%d**********************************\n", process_ID);
		printf("Safe. Allocattion Done. Available Resources:\n");
		print_available();
		
        printf("Allocated matrix:\n");
		print_allocation_matrix();
		printf("Need matrix:\n");
		print_need_matrix();
		
        return 0;
	}
	else{
		printf("[NOT SAFE] return resources\n");
		for (int i = 0; i < RESOURCE_COUNT; ++i){
			need_matrix[process_ID][i] += request_vector[i];
			allocation_matrix[process_ID][i] -= request_vector[i];
			available_resources[i] += request_vector[i];
		}
		return -1;
	}
}

int release_resource(int process_ID){

	for(int i = 0; i < RESOURCE_COUNT; i++){
		available_resources[i] += allocation_matrix[process_ID][i];
		allocation_matrix[process_ID][i] = 0;
	}
	printf("Release successfully.\nAvailable Resources:\n");
	print_available();
	printf("Allocated matrix:\n");
	print_allocation_matrix();
	printf("Need matrix:\n");
	print_need_matrix();
	return 0;
}


int is_greater_than_need(int process_ID,int request_vector[]){//check if the requested resorces
	for (int i = 0; i < RESOURCE_COUNT; ++i){				 //are more than needed
		if (request_vector[i] <= need_matrix[process_ID][i]){
			continue;
		}
		else{
			return -1;
		}
	}
	return 0;
}

int is_enough_to_allocate(int request_vector[]){//check if there are enough resources to allocate
	for (int i = 0; i < RESOURCE_COUNT; ++i){
		if (request_vector[i] <= available_resources[i]){
			continue;
		}
		else{
			return -1;
		}
	}
	return 0;
}


void print_need_matrix(){
	for(int i = 0; i < PROCESS_COUNT; ++i){
		printf("{ ");
		for (int j = 0; j < RESOURCE_COUNT; ++j){
			printf("%d", need_matrix[i][j]);
            if(j != RESOURCE_COUNT - 1)
                printf(", ");
		}
		printf("}\n");
	}
}

void print_allocation_matrix(){
	for(int i = 0; i < PROCESS_COUNT; ++i){
		printf("{ ");
		for (int j = 0; j < RESOURCE_COUNT; ++j){
			printf("%d", allocation_matrix[i][j]);
            if(j != RESOURCE_COUNT - 1)
                printf(", ");
		}
		printf("}\n");
	}
}

void print_available(){
	for(int i = 0; i < RESOURCE_COUNT; ++i)
		printf("%d, ",available_resources[i]);
	printf("\n");
}
int is_safe(){
	int if_finished[PROCESS_COUNT] = {0};
	int work[RESOURCE_COUNT];//temporary available resources vector
	for(int i = 0; i < RESOURCE_COUNT; i++){
		work[i] = available_resources[i];
	}
	int k;
	for(int i = 0; i < PROCESS_COUNT; i++){
		if (if_finished[i] == 0){
			for(int j = 0; j < RESOURCE_COUNT; j++){
				if(need_matrix[i][j] <= work[j]){
					if(j == RESOURCE_COUNT - 1){
						if_finished[i] = 1;
						for (k = 0; k < RESOURCE_COUNT; ++k){
							work[k] += allocation_matrix[i][k];
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
	for(int i = 0; i < PROCESS_COUNT; i++){
		if (if_finished[i] == 0){
			return -1;
		}
		else{
			continue;
		}
	}
	return 0;
}