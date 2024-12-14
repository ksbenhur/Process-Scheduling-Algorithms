#include <stdio.h>
#include "utility.h"
#include "process.h"
#include "stat.h"

void displayQueue(queue *q) {
	node * current_node = (node *)q->head;
				printf("<");
				while(current_node != NULL) {
					printf("%c ",((process *) current_node->data)->pid);
					current_node = current_node->next;
				}
				printf(">\n");
}

int main(int argc, char **argv) {
	int iteration_count = 0;
	//Run 5 times
	average_stats fcfs_stats[6], sjf_stats[6], srtf_stats[6], rr_stats[6], hpf_preemptive[6], hpf_non_preemptive[6], final_stats[6];

	while (iteration_count < 5) {
		//Generate Processes
		printf("\n*********************************************************************************\n");
		printf("\t\t\t\tRound %d\n", iteration_count + 1);
		printf("*********************************************************************************\n");

		linked_list * process_list = generate_processes(61);
		node * current_node = process_list->head;
		printf("-------------------------------------------------\n");
		printf("Process ID | Arrival Time | Run Time | Priority |\n");
		printf("-------------------------------------------------\n");
		while(current_node != NULL) {
			process *proc = (process *)current_node->data;
			printf("%10c | %12.1f | %8.1f | %8d |\n", proc->pid, proc->arrival_time, proc->run_time, proc->priority);
			current_node = current_node->next;
		}
		printf("--------------------------------------------------\n");
		printf("Total No. of Processes : %d\n", process_list->size);
		printf("--------------------------------------------------\n");

		//First Come First Serve
		fcfs_stats[iteration_count] = first_come_first_serve(process_list);
		//Round Robin
		rr_stats[iteration_count] = round_robin(process_list, 5);
		//Shortest Job First
		sjf_stats[iteration_count] = shortest_job_first_np(process_list);
		//Shortest Remaining Time First
		srtf_stats[iteration_count] = shortest_remaining_time_p(process_list);
		//Highest Priority First Preemptive
		hpf_preemptive[iteration_count] = highest_priority_first_p(process_list);
		//Highest Priority First Non-Preemptive
		hpf_non_preemptive[iteration_count] = highest_priority_first_np(process_list);
		iteration_count++;
	}

	for (int i = 0; i < 6; i++) {
		final_stats[i].avg_response_time = 0;
		final_stats[i].avg_wait_time = 0;
		final_stats[i].avg_turnaround = 0;
		final_stats[i].avg_throughput = 0;
	}

	// Addition of all the given stats
	for (int i = 0; i < 5; i++) {
		final_stats[0].avg_response_time += fcfs_stats[i].avg_response_time;
		final_stats[1].avg_response_time += sjf_stats[i].avg_response_time;
		final_stats[2].avg_response_time += srtf_stats[i].avg_response_time;
		final_stats[3].avg_response_time += rr_stats[i].avg_response_time;
		final_stats[4].avg_response_time += hpf_preemptive[i].avg_response_time;
		final_stats[5].avg_response_time += hpf_non_preemptive[i].avg_response_time;

		final_stats[0].avg_wait_time += fcfs_stats[i].avg_wait_time;
		final_stats[1].avg_wait_time += sjf_stats[i].avg_wait_time;
		final_stats[2].avg_wait_time += srtf_stats[i].avg_wait_time;
		final_stats[3].avg_wait_time += rr_stats[i].avg_wait_time;
		final_stats[4].avg_wait_time += hpf_preemptive[i].avg_wait_time;
		final_stats[5].avg_wait_time += hpf_non_preemptive[i].avg_wait_time;

		final_stats[0].avg_turnaround += fcfs_stats[i].avg_turnaround;
		final_stats[1].avg_turnaround += sjf_stats[i].avg_turnaround;
		final_stats[2].avg_turnaround += srtf_stats[i].avg_turnaround;
		final_stats[3].avg_turnaround += rr_stats[i].avg_turnaround;
		final_stats[4].avg_turnaround += hpf_preemptive[i].avg_turnaround;
		final_stats[5].avg_turnaround += hpf_non_preemptive[i].avg_turnaround;

		final_stats[0].avg_throughput += fcfs_stats[i].avg_throughput;
		final_stats[1].avg_throughput += sjf_stats[i].avg_throughput;
		final_stats[2].avg_throughput += srtf_stats[i].avg_throughput;
		final_stats[3].avg_throughput += rr_stats[i].avg_throughput;
		final_stats[4].avg_throughput += hpf_preemptive[i].avg_throughput;
		final_stats[5].avg_throughput += hpf_non_preemptive[i].avg_throughput;
	}

	//Average of all the algorithms
	for (int i = 0; i < 6; i++) {
		final_stats[i].avg_response_time /= 5;
		final_stats[i].avg_wait_time /= 5;
		final_stats[i].avg_turnaround /= 5;
		final_stats[i].avg_throughput /= 5;
	}
	printf("\n\n\n");

	printf("The average of the 5 runs of all algorithms is as follows:\n");
	printf("\n");
	printf("ALGORITHM: FIRST COME FIRST SERVE:\n\n");
	printf("Average Response Time (ART) : %.1f\n", final_stats[0].avg_response_time);
	printf("Average Wait Time (AWT) : %.1f\n", final_stats[0].avg_wait_time);
	printf("Average Turn Around Time (TAT) :%.1f\n", final_stats[0].avg_turnaround);
	printf("Average Throughput :%.1f\n", final_stats[0].avg_throughput);
	printf("\n");
	printf("ALGORITHM: ROUND ROBIN PREEMPTIVE:\n\n");
	printf("Average Response Time (ART) : %.1f\n", final_stats[3].avg_response_time);
	printf("Average Wait Time (AWT) : %.1f\n", final_stats[3].avg_wait_time);
	printf("Average Turn Around Time (TAT) :%.1f\n", final_stats[3].avg_turnaround);
	printf("Average Throughput :%.1f\n", final_stats[3].avg_throughput);
	printf("\n");
	printf("ALGORITHM: SHORTEST JOB FIRST NON PREEMPTIVE:\n\n");
	printf("Average Response Time (ART) : %.1f\n", final_stats[1].avg_response_time);
	printf("Average Wait Time (AWT) : %.1f\n", final_stats[1].avg_wait_time);
	printf("Average Turn Around Time (TAT) :%.1f\n", final_stats[1].avg_turnaround);
	printf("Average Throughput :%.1f\n", final_stats[1].avg_throughput);
	printf("\n");
	printf("ALGORITHM: SHORTEST REMAINING TIME FIRST PREEMPTIVE:\n\n");
	printf("Average Response Time (ART) : %.1f\n", final_stats[2].avg_response_time);
	printf("Average Wait Time (AWT) : %.1f\n", final_stats[2].avg_wait_time);
	printf("Average Turn Around Time (TAT) :%.1f\n", final_stats[2].avg_turnaround);
	printf("Average Throughput :%.1f\n", final_stats[2].avg_throughput);
	printf("\n");
	printf("ALGORITHM: HIGHEST PRIORITY FIRST PREEMPTIVE:\n\n");
	printf("Average Response Time (ART) : %.1f\n", final_stats[4].avg_response_time);
	printf("Average Wait Time (AWT) : %.1f\n", final_stats[4].avg_wait_time);
	printf("Average Turn Around Time (TAT) :%.1f\n", final_stats[4].avg_turnaround);
	printf("Average Throughput :%.1f\n", final_stats[4].avg_throughput);
	printf("\n");
	printf("ALGORITHM: HIGHEST PRIORITY FIRST NON PREEMPTIVE:\n\n");
	printf("Average Response Time (ART) : %.1f\n", final_stats[5].avg_response_time);
	printf("Average Wait Time (AWT) : %.1f\n", final_stats[5].avg_wait_time);
	printf("Average Turn Around Time (TAT) :%.1f\n", final_stats[5].avg_turnaround);
	printf("Average Throughput :%.1f\n", final_stats[5].avg_throughput);
	printf("\n");
}