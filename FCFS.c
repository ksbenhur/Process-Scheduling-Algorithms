#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

// Function to create a process stat object
process_stat * create_process_stat(process* current_proc) {
	process_stat * p_stat = (process_stat *) malloc(sizeof(process_stat));
	p_stat->proc = current_proc;
	p_stat->wait_time = 0;
	p_stat->turnaround_time = 0;
	p_stat->response_time = 0;
	p_stat->run_time = 0;
	p_stat->start_time = -1;
	p_stat->end_time = -1;
	return p_stat;
}

// Function to implement First Come First Serve scheduling
average_stats first_come_first_serve(linked_list * proc_list) {
	int time_counter = 0;

	//Create Process Queue
	queue *proc_queue = (queue *)create_queue();
	node * proc_node = proc_list->head;
	if(proc_list->head == NULL) {
		fprintf(stderr, "No processes available for scheduling\n");
	}
	
	// Scheduled process to keep track of currently running process
	process_stat * active_process = NULL;

	linked_list *completed_list = create_linked_list();
	printf("\nExecuting First Come First Serve Scheduling:\n");

	// Run until either time exceeds 100 or there's an active process
	while(time_counter < 100 || active_process != NULL) {
		// Check if new processes have arrived, if so enqueue them
		if(proc_node != NULL) {
			process * incoming_process = (process *)(proc_node->data);
			if(incoming_process->arrival_time <= time_counter) {
				enqueue(proc_queue, create_process_stat(incoming_process));
				proc_node = proc_node->next;
			}
		}

		// If there's no active process, schedule the next one from the queue
		if(active_process == NULL && proc_queue->size > 0) {
			active_process = (process_stat *) dequeue(proc_queue);
		}

		// If a process is currently running, update its statistics
		if(active_process != NULL) {
			process * running_proc = active_process->proc;

			// Log the currently running process in the timeline
			printf("%c", running_proc->pid);

			// If the process is starting for the first time, update its start time
			if(active_process->start_time == -1) {
				active_process->start_time = time_counter;
			}
			// Increment the runtime of the active process
			active_process->run_time++;

			// Check if the process has finished executing
			if(active_process->run_time >= running_proc->run_time) {
				active_process->end_time = time_counter;
				add_node(completed_list, active_process);  // Move the process to the completed list
				active_process = NULL;  // Mark no active process
			}
		} else {
			// No process is running, print idle
			printf("_");
		}
		// Increment the time
		time_counter++;
	}
	printf("\n");

	// Print the statistics of the scheduling policy
	return print_policy_stat(completed_list);
}