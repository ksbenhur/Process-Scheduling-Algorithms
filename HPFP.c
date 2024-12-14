#include "stat.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

process_stat * create_process_stat(process* proc);

// Function to compare the priority and arrival time for HPFP
int compare_priority(void * first_data, void * second_data)
{
    process_stat * first_process = (process_stat *) first_data;
    process_stat * second_process = (process_stat *) second_data;

    if((((process *)first_process->proc)->priority) < (((process *)second_process->proc)->priority)) {
        return -1;
    }
    else if (((((process *)first_process->proc)->priority)) == ((((process *)second_process->proc)->priority)) &&  (((process *)first_process->proc)->arrival_time < (((process *)second_process->proc)->arrival_time)))
  {
    return -1;
  }
  else {
		return 1;
	}
}

// Implementation of Highest Priority First Preemptive (HPFP)
average_stats highest_priority_first_p(linked_list * process_list)
{
    int time_quanta = 0;

    // Creation of linked lists to manage processes with different priorities
    linked_list * priority_1_list = create_linked_list();
    linked_list * priority_2_list = create_linked_list();
    linked_list * priority_3_list = create_linked_list();
    linked_list * priority_4_list = create_linked_list();

    // Creation of process queues for different priority levels
    queue * priority_1_queue = create_queue();
    queue * priority_2_queue = create_queue();
    queue * priority_3_queue = create_queue();
    queue * priority_4_queue = create_queue();

    node * current_node = process_list->head;
    if(process_list->head == NULL) {
        fprintf(stderr, "No Process to schedule\n");
    }

    printf("\nHighest Priority First Preemptive:\n");

    process_stat * scheduled_process = NULL;
    while(time_quanta < 100 || scheduled_process != NULL)
    {
        if(scheduled_process != NULL)
        {
            if(scheduled_process->proc->priority == 1) enqueue(priority_1_queue, scheduled_process);
            if(scheduled_process->proc->priority == 2) enqueue(priority_2_queue, scheduled_process);
            if(scheduled_process->proc->priority == 3) enqueue(priority_3_queue, scheduled_process);
            if(scheduled_process->proc->priority == 4) enqueue(priority_4_queue, scheduled_process);
        }

        // Check for new process arrivals and enqueue them
        if(current_node != NULL) {
            process * new_process = (process *)(current_node->data);
            while(current_node != NULL && new_process->arrival_time <= time_quanta) {
                if(new_process->priority == 1) enqueue(priority_1_queue, create_process_stat(new_process));
                if(new_process->priority == 2) enqueue(priority_2_queue, create_process_stat(new_process));
                if(new_process->priority == 3) enqueue(priority_3_queue, create_process_stat(new_process));
                if(new_process->priority == 4) enqueue(priority_4_queue, create_process_stat(new_process));

                sort(priority_1_queue, compare_priority);
                sort(priority_2_queue, compare_priority);
                sort(priority_3_queue, compare_priority);
                sort(priority_4_queue, compare_priority);

                current_node = current_node->next;
                if(current_node != NULL) {
                    new_process = (process *)(current_node->data);
                }
            }
        }

        // If no process is scheduled, fetch from the highest priority queue
        if(scheduled_process == NULL) {
            if (priority_1_queue->size > 0) scheduled_process = (process_stat *) dequeue(priority_1_queue);
            else if (priority_2_queue->size > 0) scheduled_process = (process_stat *) dequeue(priority_2_queue);
            else if (priority_3_queue->size > 0) scheduled_process = (process_stat *) dequeue(priority_3_queue);
            else if (priority_4_queue->size > 0) scheduled_process = (process_stat *) dequeue(priority_4_queue);

            // If a process is not started before quanta 100, discard it
            if (time_quanta >= 100 && scheduled_process->start_time == -1) {
                scheduled_process = NULL;
                continue;
            }
        }

        if(scheduled_process != NULL) {
            process * proc = scheduled_process->proc;

            // Display current process on the time chart
            printf("%c", proc->pid);

            // Update process statistics
            if(scheduled_process->start_time == -1) {
                scheduled_process->start_time = time_quanta;
            }

            scheduled_process->run_time++;

            // If process finishes, move to the appropriate linked list
            if(scheduled_process->run_time >= proc->run_time) {
                scheduled_process->end_time = time_quanta;
                if(scheduled_process->proc->priority == 1) add_node(priority_1_list, scheduled_process);
                else if(scheduled_process->proc->priority == 2) add_node(priority_2_list, scheduled_process);
                else if(scheduled_process->proc->priority == 3) add_node(priority_3_list, scheduled_process);
                else if(scheduled_process->proc->priority == 4) add_node(priority_4_list, scheduled_process);
                scheduled_process = NULL;
            }
        } else {
            printf("_");
        }

        // Increase time quanta
        time_quanta++;
    }

    // Print statistics for each priority queue
    average_stats stats_priority_1, stats_priority_2, stats_priority_3, stats_priority_4, overall_stats;
    printf("\nStatistics for Priority Queue 1:");
    stats_priority_1 = print_policy_stat(priority_1_list);
    printf("\nStatistics for Priority Queue 2:");
    stats_priority_2 = print_policy_stat(priority_2_list);
    printf("\nStatistics for Priority Queue 3:");
    stats_priority_3 = print_policy_stat(priority_3_list);
    printf("\nStatistics for Priority Queue 4:");
    stats_priority_4 = print_policy_stat(priority_4_list);

    // Calculate overall statistics
    overall_stats.avg_response_time = (stats_priority_1.avg_response_time + stats_priority_2.avg_response_time + stats_priority_3.avg_response_time + stats_priority_4.avg_response_time) / 4;
    overall_stats.avg_wait_time = (stats_priority_1.avg_wait_time + stats_priority_2.avg_wait_time + stats_priority_3.avg_wait_time + stats_priority_4.avg_wait_time) / 4;
    overall_stats.avg_turnaround = (stats_priority_1.avg_turnaround + stats_priority_2.avg_turnaround + stats_priority_3.avg_turnaround + stats_priority_4.avg_turnaround) / 4;
    overall_stats.avg_throughput = (stats_priority_1.avg_throughput + stats_priority_2.avg_throughput + stats_priority_3.avg_throughput + stats_priority_4.avg_throughput);

    printf("\nOverall Average Statistics for Highest Priority First Preemptive Scheduling:\n");
    printf("Average Response Time (RT) : %.1f\n", overall_stats.avg_response_time);
    printf("Average Wait Time (WT) : %.1f\n", overall_stats.avg_wait_time);
    printf("Average Turn Around Time (TAT) : %.1f\n", overall_stats.avg_turnaround);

    return overall_stats;
}