#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);
int compare_priority(void * data1, void * data2);
void printqueue(queue * q);

// highest_priority_first_np: highest priority first non-preemptive
average_stats highest_priority_first_np(linked_list * proc_list)
{
  int time_counter = 0; // Time counter (quanta)
  
  // Creation of process queues for different priority levels
  queue * high_priority_queue_1 = create_queue();
  queue * high_priority_queue_2 = create_queue();
  queue * high_priority_queue_3 = create_queue();
  queue * high_priority_queue_4 = create_queue();
  
  // Linked list to manage process execution order for each priority level
  linked_list * linked_list_1 = create_linked_list();
  linked_list * linked_list_2 = create_linked_list();
  linked_list * linked_list_3 = create_linked_list();
  linked_list * linked_list_4 = create_linked_list();
  
  node * process_node = proc_list->head;

  if(proc_list->head == NULL) {
        fprintf(stderr,"No processes available for scheduling\n");
    }
    
  printf("\nExecuting Highest Priority First Non-Preemptive Scheduling:\n");

  process_stat * active_process = NULL;

  while(time_counter < 100 || active_process != NULL )
  {
    // Check for incoming new processes and enqueue them into the respective priority queues
        if(process_node != NULL) {
            process * new_process = (process *)(process_node->data);
            while(process_node != NULL && new_process->arrival_time <= time_counter) {
        if(new_process->priority == 1) enqueue(high_priority_queue_1, create_process_stat(new_process));
        if(new_process->priority == 2) enqueue(high_priority_queue_2, create_process_stat(new_process));
        if(new_process->priority == 3) enqueue(high_priority_queue_3, create_process_stat(new_process));
        if(new_process->priority == 4) enqueue(high_priority_queue_4, create_process_stat(new_process));
                process_node = process_node->next;
                if(process_node != NULL)
        {
                    new_process = (process *)(process_node->data);
        }
            }
    }
    
    // If there's no active process, schedule the next one from the priority queues
        if(active_process == NULL) {
      if (high_priority_queue_1->size > 0) active_process = (process_stat *) dequeue(high_priority_queue_1);
      else if (high_priority_queue_2->size > 0) active_process = (process_stat *) dequeue(high_priority_queue_2);
      else if (high_priority_queue_3->size > 0) active_process = (process_stat *) dequeue(high_priority_queue_3);
      else if (high_priority_queue_4->size > 0) active_process = (process_stat *) dequeue(high_priority_queue_4);
      
      // If the process hasn't started by quanta 100, remove it and move to the next process
      if (time_counter >= 100 && active_process->start_time == -1){
        active_process = NULL;
        continue;
      }
    }

    if(active_process != NULL) {
              process * current_proc = active_process->proc;

              // Print the current running process in the timeline
              printf("%c",current_proc->pid);

              // Update the active process's stats
              if(active_process->start_time == -1) {
                  active_process->start_time = time_counter;
              }

              active_process->run_time++;

        if(active_process->run_time >= current_proc->run_time) {
          active_process->end_time = time_counter;
          if(active_process->proc->priority == 1) add_node(linked_list_1, active_process);
          else if(active_process->proc->priority == 2) add_node(linked_list_2, active_process);
          else if(active_process->proc->priority == 3) add_node(linked_list_3, active_process);
          else if(active_process->proc->priority == 4) add_node(linked_list_4, active_process);
          active_process = NULL;
        }
      } else {
                printf("_");  // If no process is active, indicate idle time
          }
          // Increment the time counter
          time_counter++;
    }

    // Print statistics for each priority queue
    average_stats avg_queue_1, avg_queue_2, avg_queue_3, avg_queue_4, avg_combined;
    printf("\nFor Priority Queue 1:");
    avg_queue_1 = print_policy_stat(linked_list_1);
    printf("\nFor Priority Queue 2:");
    avg_queue_2 = print_policy_stat(linked_list_2);
    printf("\nFor Priority Queue 3:");
    avg_queue_3 = print_policy_stat(linked_list_3);
    printf("\nFor Priority Queue 4:");
    avg_queue_4 = print_policy_stat(linked_list_4);

    // Calculate combined average statistics
    avg_combined.avg_response_time = (avg_queue_1.avg_response_time + avg_queue_2.avg_response_time + avg_queue_3.avg_response_time + avg_queue_4.avg_response_time) / 4;
    avg_combined.avg_wait_time = (avg_queue_1.avg_wait_time + avg_queue_2.avg_wait_time + avg_queue_3.avg_wait_time + avg_queue_4.avg_wait_time) / 4;
    avg_combined.avg_turnaround = (avg_queue_1.avg_turnaround + avg_queue_2.avg_turnaround + avg_queue_3.avg_turnaround + avg_queue_4.avg_turnaround) / 4;
    avg_combined.avg_throughput = avg_queue_1.avg_throughput + avg_queue_2.avg_throughput + avg_queue_3.avg_throughput + avg_queue_4.avg_throughput;

    // Print the overall statistics
    printf("\nCombined Average Statistics for Highest Priority First Non-Preemptive Scheduling across all priority queues:\n");
    printf("Average Response Time: %.1f\n", avg_combined.avg_response_time);
    printf("Average Wait Time: %.1f\n", avg_combined.avg_wait_time);
    printf("Average Turn Around Time: %.1f\n", avg_combined.avg_turnaround);

    return avg_combined;
}