#include <stdlib.h>
#include <stdio.h>
#include "pri_queue.h"
/** @file pri_queue.c */

static Node_ptr_t head = NULL;
/**
 * Insert a Node into a priority queue.
 * @param priority
 * @param data
 * @author YOUR NAME
 */
void PQ_insert(int priority, char * data) {
 //FIX THIS
    Node_ptr_t tmp = (Node_t*) malloc(sizeof(Node_t));
    tmp->data = data;
    tmp->priority = priority;
    tmp->next = NULL;
    if(head == NULL)
        head = tmp;
    else{
        if(head->priority < priority){
            Node_ptr_t tmp1 = tmp;
            tmp1->next = head;
            head = tmp1;
        }
        else{
            Node_ptr_t tmp1 = head;
            Node_ptr_t hold;
            int count = 0;
            while(tmp1->next !=NULL){
                if(tmp->priority > tmp1->next->priority){
                    count = 1;
                    break;
                }
                tmp1 = tmp1->next;
            }
            if(count == 1){
                hold = tmp1->next;
                tmp1->next = tmp;
                tmp->next = hold;
            }
            else
                tmp1->next = tmp;
        }
    }
}
/**
 * Delete and return the node with the highest priority.
 * @return The highest priority Node.
 */
Node_ptr_t PQ_delete() {
  //FIX THIS
    Node_ptr_t new_head = (Node_t*) malloc(sizeof(Node_t));
    if(head != NULL){
        new_head = head->next;
        free(head);
        head = new_head;
    }
}

/**
 * Do NOT modify this function.
 * @return A pointer to the head of the list.  (NULL if list is empty.)
 */
Node_ptr_t PQ_get_head() {
    return head;
}

/**
 * Do NOT modify this function.
 * @return the number of items in the queue
 */
int PQ_get_size() {
    int size = 0;
    Node_ptr_t tmp;
    for(tmp = head; tmp != NULL; tmp = tmp->next, size++)
        ;
    return size;
}


