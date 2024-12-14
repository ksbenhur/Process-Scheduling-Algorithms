#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);

int compareRunTime(void * data_1, void * data_2) {
	process_stat * process_stat_1 = (process_stat *) data_1;
	process_stat * process_stat_2 = (process_stat *) data_2;
	
	if(((process *)process_stat_1->proc)->run_time < ((process *)process_stat_2->proc)->run_time) {
		return -1;
	} else {
		return 1;
	}
}

average_stats shortest_job_first_np(linked_list * process_list) {
	int current_time = 0;

	// Create process queue
	queue *process_queue = (queue *)create_queue();
	node * current_process_node = process_list->head;

	if(process_list->head == NULL) {
		fprintf(stderr, "No process available for scheduling.\n");
	}

	// Check if process queue is not empty or current time is less than 100
	process_stat * active_process = NULL;
	linked_list *completed_processes = create_linked_list();

	printf("\nExecuting Shortest Job First Scheduling:\n");

	while(current_time < 100 || active_process != NULL) {
		// Check for new arriving process and add to queue
		if(current_process_node != NULL) {
			process * new_process = (process *)(current_process_node->data);
			
			while(current_process_node != NULL && new_process->arrival_time <= current_time) {
				enqueue(process_queue, create_process_stat(new_process));
				sort(process_queue, compareRunTime);
				current_process_node = current_process_node->next;
				
				if(current_process_node != NULL) {
					new_process = (process *)(current_process_node->data);
				}
			}
		}

		// If no active process, get the next one from the queue
		if(active_process == NULL && process_queue->size > 0) {
			active_process = (process_stat *) dequeue(process_queue);
		}

		if(active_process != NULL) {
			process * current_process = active_process->proc;

			// Print current running process
			//printf("Running process: %c\n", current_process->pid);

			// Update current process stats
			if(active_process->start_time == -1) {
				active_process->start_time = current_time;
			}
			active_process->run_time++;

			// If the process completes, add to completed list
			if(active_process->run_time >= current_process->run_time) {
				active_process->end_time = current_time;
				add_node(completed_processes, active_process);
				active_process = NULL;
			}
		} else {
			printf("Idle at time %d\n", current_time);
		}

		// Increment the current time
		current_time++;
	}

	printf("\n");
	return print_policy_stat(completed_processes);
}