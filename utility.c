#include <stdio.h>
#include <stdlib.h>
#include "utility.h"
#include "process.h"
#include "stat.h"

// Linked list implementation functions

// create_linked_list function: creates a new linked list
linked_list * create_linked_list()
{
	linked_list * new_list = (linked_list*) malloc(sizeof(linked_list));
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->size = 0;
	return new_list;
}

// create_node function: creates a new node
node* create_node(void* node_data)
{
	node* new_node = (node*) malloc(sizeof(node));
	new_node->data = node_data;
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

// add_node function: adds a node to an existing linked list
void add_node(linked_list * list, void * node_data)
{
	node * new_node = create_node(node_data);
	if(list->size == 0)
	{
		list->head = new_node;
		list->tail = new_node;
		list->size = 1;
	} else {
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
		list->size++;
	}
}

// remove_node function: removes a node from an existing linked list
void remove_node(linked_list* list, void * node_data)
{
	node* current_node = list->head;

	while(current_node != NULL && current_node->data != node_data) {
		current_node = current_node->next;
	}

	if(current_node != NULL) {
		if(current_node->prev != NULL) {
			current_node->prev->next = current_node->next;
		}
		if(current_node->next != NULL) {
			current_node->next->prev = current_node->prev;
		}
		if(list->head == current_node) {
			list->head = current_node->next;
		}
		if(list->tail == current_node) {
			list->tail = current_node->prev;
		}
		list->size--;
		free(current_node);
	}
}

// remove_head function: removes the first node from an existing linked list
void remove_head(linked_list* list)
{
	node * current_node = list->head;
	if(current_node != NULL) {
		list->head = current_node->next;
		if(list->tail == current_node) {
			list->tail = current_node->prev;
		}
		list->size--;
		free(current_node);
	}
}

// add_after function: adds a node after a particular node in an existing linked list
void add_after(linked_list* list, node *after_node, void *node_data)
{
	node* new_node = create_node(node_data);

	node* next_node = after_node->next;
	new_node->next = next_node;
	if(next_node != NULL) next_node->prev = new_node;

	new_node->prev = after_node;
	after_node->next = new_node;

	if(list->tail == after_node) {
		list->tail = new_node;
	}

	list->size++;
}

// sort function: sorts the linked list using a comparison function
void sort(linked_list *list, int (*cmp)(void *data1, void *data2)) {
	node *i = list->head;
	while(i != NULL) {
		node *j = i->next;
		while(j != NULL) {
			void * p1 = i->data;
			void * p2 = j->data;
			if((*cmp)(p1, p2) > 0) {
				swap_nodes(i, j);
			}
			j = j->next;
		}
		i = i->next;
	}
}

// swap_nodes function: swaps the data between two nodes
void swap_nodes(node *a, node *b) {
	void * temp = a->data;
	a->data = b->data;
	b->data = temp;
}

// Queue implementation

// create_queue function: creates a new queue (which is a linked list)
queue * create_queue() {
	return create_linked_list();
}

// enqueue function: adds a process at the end of the queue
void enqueue(queue* q, void * data)
{
	node* new_node = create_node(data);

	new_node->prev = q->tail;
	if(q->tail != NULL) {
		q->tail->next = new_node;
		q->tail = new_node;
	} else {
		q->tail = new_node;
		q->head = new_node;
	}
	q->size++;
}

// dequeue function: removes a process from the front of the queue
void* dequeue(queue* q)
{
	if(q->head != NULL) {
		node * current_node = q->head;
		void * node_data = current_node->data;

		node * next_node = q->head->next;
		if(next_node != NULL) next_node->prev = NULL;
		q->head = next_node;

		if(q->tail == current_node) {
			q->tail = NULL;
		}

		q->size--;
		free(current_node);
		return node_data;
	}
	return NULL;
}

// print_policy_stat function: prints process stats for different algorithms
average_stats print_policy_stat(linked_list * list)
{
	average_stats avg_stats;
	printf("\n");
	printf("---------------------------------------------------------------------------------\n");
	printf("Process ID  | Arrival | Start | End  | Run   | Response | Wait  | Turn Around | Priority |\n");
	printf("---------------------------------------------------------------------------------\n");
	node * current_node = list->head;
	float total_response_time = 0;
	float total_wait_time = 0;
	float total_turnaround_time = 0;
	int total_processes = 0;

	while(current_node != NULL) {
		process_stat *stat = (process_stat *)current_node->data;
		process *proc = (process *)stat->proc;

		float arrival_time = proc->arrival_time;
		float run_time = proc->run_time;
		float response_time = stat->start_time - arrival_time;
		float turnaround_time = stat->end_time - proc->arrival_time + 1;
		float wait_time = turnaround_time - run_time;
		unsigned char priority = proc->priority;

		total_response_time += response_time;
		total_wait_time += wait_time;
		total_turnaround_time += turnaround_time;
		total_processes++;

		printf("%10c | %7.1f | %5.1f | %4.1f | %5.1f | %8.1f | %5.1f | %11.1f | %8u |\n", 
			proc->pid, arrival_time, stat->start_time, stat->end_time, run_time, 
			response_time, wait_time, turnaround_time, priority);

		current_node = current_node->next;
	}

	avg_stats.avg_throughput = total_processes;
	if(total_processes == 0) total_processes = 1;

	avg_stats.avg_response_time = total_response_time / total_processes;
	avg_stats.avg_wait_time = total_wait_time / total_processes;
	avg_stats.avg_turnaround = total_turnaround_time / total_processes;

	printf("---------------------------------------------------------------------------------\n");
	printf("%10s | %7.1f | %5.1f | %4.1f | %5.1f | %8.1f | %5.1f | %11.1f |\n", 
		"Average", 0.0, 0.0, 0.0, 0.0, 
		avg_stats.avg_response_time, avg_stats.avg_wait_time, avg_stats.avg_turnaround);
	printf("---------------------------------------------------------------------------------\n");

	return avg_stats;
}