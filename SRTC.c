#include <stdio.h>
#include <stdlib.h>
#include "stat.h"
#include "utility.h"

process_stat * create_process_stat(process* proc);
// compare_by_remaining_time funcntion:comparing time to completion input processes while sorting
int compare_by_remaining_time(void * data1, void * data2)
{
  process_stat * process_stat_1 = (process_stat *) data1;
  process_stat * process_stat_2 = (process_stat *) data2;
  if(((((process *)process_stat_1->proc)->run_time) - (process_stat_1->run_time)) < ((((process *)process_stat_2->proc)->run_time) - (process_stat_2->run_time))) {
    return -1;
  } else {
    return 1;
  }
}
// shortest_remaining_time_p function: calculating shortest remaining time to completion preemptive
average_stats shortest_remaining_time_p(linked_list * input_processes)
{
  int t = 0;

  // creation of a queue of input_processes
  queue * process_queue_by_remaining_time = create_queue();

  // creation of linked list for managaing process order in preemeption
  linked_list * completed_process = create_linked_list();

  node * input_processes_ptr = input_processes->head;

  //check if process queue is not empty or time quanta is less than 100
  process_stat * current_process = NULL;
  printf("\n\nShortest Remaining Time To Completion Preemptive:\n");
  while(t < 100 || current_process != NULL)
  {
    if(current_process != NULL)
    {
      enqueue(process_queue_by_remaining_time, current_process);
      current_process = NULL;
    }

    //check for incoming new process and enqueue it in the queue
        if(input_processes_ptr != NULL) {
            process * new_process = (process *)(input_processes_ptr->data);
            while(input_processes_ptr !=NULL && new_process->arrival_time <= t) {
                enqueue(process_queue_by_remaining_time, create_process_stat(new_process));
                
        input_processes_ptr = input_processes_ptr->next;

                if(input_processes_ptr!=NULL)
        {
                    new_process = (process *)(input_processes_ptr->data);
        }
            }

      //Based on the remaining running time to completion, sort all the input_processes that have arrived
      sort(process_queue_by_remaining_time,compare_by_remaining_time);
    }

    //if there is no scheduled process, then check process queue and schedule it
        if(current_process == NULL && process_queue_by_remaining_time->size > 0) {
            current_process = (process_stat *) dequeue(process_queue_by_remaining_time);

      // If the process has not started before quanta 100, discard the process from the queue and select the next process in queue for execution
      while(t>=100 && current_process->start_time == -1)
      {
        current_process = (process_stat *) dequeue(process_queue_by_remaining_time);
      }
        }

    if(current_process != NULL) {
              process * proc = current_process->proc;

              //print currently running process to the time chart
              printf("%c",proc->pid);

              //Update current input_processes stat
              if(current_process->start_time == -1) {
                  current_process->start_time = t;
              }

              current_process->run_time++;

        // If the process has finished its execution, then add it to completed_process linked list
        if(current_process->run_time >= proc->run_time) {
          current_process->end_time = t;
          add_node(completed_process,current_process);
          current_process = NULL;
        }
      }
      else
      {
                printf("_");
        }

          t++;
  }

  //Print Process Stat and return average stats
  return print_policy_stat(completed_process);
}
