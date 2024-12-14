#include <stdio.h>
#include <stdlib.h>
#include "process.h"
#include "utility.h"
#include <time.h>

// compare function: compares two processes based on arrival time
int compare_by_arrival_time(void * process_data1, void * process_data2) {
	process *process_1 = (process *)process_data1;
	process *process_2 = (process *)process_data2;
	if(process_1->arrival_time < process_2->arrival_time) {
		return -1;
	} else if(process_1->arrival_time == process_2->arrival_time){
		return 0;
	} else {
		return 1;
	}
}

// generate_processes function: generates 'n' processes and adds them to a linked list
linked_list * generate_processes(int num_processes) {
	linked_list * process_list = create_linked_list();
	char process_id = 'A';
	unsigned char process_priority;
	float process_arrival_time, process_run_time;
	time_t current_time;

	// Initialize random number generator
	srand((unsigned) time(&current_time));

	if(process_list == NULL) {
		fprintf(stderr, "Failed to create process linked list\n");
	}

	// Generate processes and add them to the linked list
	while(num_processes--) {
		process_arrival_time = rand() % 100; // generate random arrival time
		process_run_time = (float)((rand() % 100) + 1) / 10; // generate random run time
		process_priority = (rand() % 4) + 1; // generate random priority (1 to 4)

		// Create process and add to linked list
		process *new_process = create_process(process_id, process_arrival_time, process_run_time, process_priority);
		add_node(process_list, new_process);
		process_id++;
	}

	// Sort processes by arrival time
	sort(process_list, compare_by_arrival_time);

	// Assign process IDs again
	node * current_node = process_list->head;
	process_id = 'A';
	while(current_node != NULL) {
		((process *)current_node->data)->pid = process_id;
		if(process_id == 'Z') process_id = 'a' - 1; // handle lowercase after 'Z'
		process_id++;
		current_node = current_node->next;
	}

	return process_list;
}

// create_process function: creates and initializes a new process
process * create_process(char process_id, float process_arrival_time, float process_run_time, unsigned char process_priority) {
	process * new_process = (process *) malloc(sizeof(process));
	new_process->pid = process_id;
	new_process->arrival_time = process_arrival_time;
	new_process->run_time = process_run_time;
	new_process->priority = process_priority;
	return new_process;
}