#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);

average_stats round_robin(linked_list * process_list, int time_slice) {
    int current_time = 0;

    // Create Process Queue
    queue *process_queue = (queue *)create_queue();
    node * process_node = process_list->head;
    
    if(process_list->head == NULL) {
        fprintf(stderr, "No process available for scheduling.\n");
    }

    // While process queue is not empty or time is less than 100
    process_stat * active_process = NULL;
    linked_list *completed_processes = create_linked_list();
    
    printf("\nExecuting Round Robin Scheduling:\n");

    node * active_node = NULL;
    int time_in_slice = 0;
    
    while(current_time < 100 || process_queue->size > 0) {

        // Check for new arriving processes and add them to the queue if arrival time < 100
        if(process_node != NULL && current_time < 100) {
            process * new_process = (process *)(process_node->data);
            while(process_node != NULL && new_process->arrival_time <= current_time) {
                enqueue(process_queue, create_process_stat(new_process));
                process_node = process_node->next;
                if(process_node != NULL) {
                    new_process = (process *)(process_node->data);
                }
            }
        }

        // If there is no active process, move to the next node in the queue
        if(active_node == NULL) {
            time_in_slice = 0;
            active_node = process_queue->head;
        } else if(time_in_slice == time_slice) {
            time_in_slice = 0;
            active_node = active_node->next;
            if(active_node == NULL) {
                active_node = process_queue->head;
            }
        }

        if(active_node != NULL) {
            active_process = (process_stat *) active_node->data;
            process * current_process = active_process->proc;

            // If time > 100, don't start new processes, remove them from queue
            if(current_time >= 100) {
                if(active_process->start_time == -1) {
                    free(active_process);
                    node * next_node = active_node->next;
                    remove_node(process_queue, active_node->data);
                    active_node = next_node;
                    time_in_slice = 0;
                    continue;
                }
            }

            // Print current process execution status
           // printf("Running process: %c\n", current_process->pid);
            time_in_slice++;

            // Update stats for the current process
            if(active_process->start_time == -1) {
                active_process->start_time = current_time;
            }

            active_process->run_time++;

            // If process finishes, remove from queue
            if(active_process->run_time >= current_process->run_time) {
                active_process->end_time = current_time;
                add_node(completed_processes, active_process);
                node * next_node = active_node->next;
                remove_node(process_queue, active_node->data);
                active_node = next_node;
                time_in_slice = 0;
            }
        } else {
            printf("Idle at time %d\n", current_time);
        }

        // Increment the time
        current_time++;
    }

    // Return process statistics
    return print_policy_stat(completed_processes);
}