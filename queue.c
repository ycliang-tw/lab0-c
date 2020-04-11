#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    /* TODO */
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        fprintf(stderr, "[error] Cannot allocate space for new queue.\n");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* TODO */
    if (!q) {
        fprintf(stderr, "[warning] Queue is null.\n");
        return;
    }

    while (q->head) {
        list_ele_t *remove = q->head;
        q->head = q->head->next;
        free(remove->value);
        free(remove);
    }
    /* Free queue structure */
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO */
    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        goto err;
    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value)
        goto err;
    strncpy(newh->value, s, (strlen(s) + 1) * sizeof(char));
    if (!q)
        goto err;

    newh->next = q->head;
    q->head = newh;
    if (q->size == 0) {
        q->tail = newh;
    }
    q->size += 1;
    return true;

err:
    if (!q)
        fprintf(stderr, "[error] q is NULL.\n");
    if (!newh)
        fprintf(stderr, "[error] Cannot allocate space for new element.\n");
    else if (!newh->value)
        fprintf(stderr,
                "[error] Cannot allocate spcace for new element's value.\n");
    else
        free(newh->value);
    free(newh);
    return false;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* TODO */
    list_ele_t *newh = malloc(sizeof(list_ele_t));
    if (!newh)
        goto err;
    newh->value = malloc((strlen(s) + 1) * sizeof(char));
    if (!newh->value)
        goto err;
    strncpy(newh->value, s, (strlen(s) + 1) * sizeof(char));
    if (!q)
        goto err;

    newh->next = NULL;
    if (q->size == 0) {
        q->head = newh;
    } else {
        q->tail->next = newh;
    }
    q->tail = newh;
    q->size += 1;
    return true;

err:
    if (!q)
        fprintf(stderr, "[error] q is NULL.\n");
    if (!newh)
        fprintf(stderr, "[error] Cannot allocate space for new element.\n");
    else if (!newh->value)
        fprintf(stderr,
                "[error] Cannot allocate spcace for new element's value.\n");
    else
        free(newh->value);
    free(newh);
    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* TODO */
    if (!q || q->size == 0)
        return false;

    list_ele_t *remove = q->head;
    q->head = q->head->next;
    if (sp) {
        strncpy(sp, remove->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    free(remove->value);
    free(remove);
    q->size -= 1;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* TODO */
    if (q) {
        return q->size;
    }
    return 0;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* TODO */
    if (!q || q->size == 0 || q->size == 1)
        return;

    q->tail = q->head;

    list_ele_t *left = q->head;
    list_ele_t *head = q->head->next;
    list_ele_t *right = head->next;
    while (head) {
        head->next = left;
        left = head;
        head = right;
        if (right)
            right = right->next;
    }
    q->head = left;
    q->tail->next = NULL;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

static list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    //    list_ele_t *node = (list_ele_t*)malloc(sizeof(list_ele_t));
    list_ele_t node = {.value = NULL, .next = NULL};
    list_ele_t *p = &node;
    list_ele_t *tmp = &node;
    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            p->next = l1;
            l1 = l1->next;
        } else {
            p->next = l2;
            l2 = l2->next;
        }
        p = p->next;
    }

    if (l1)
        p->next = l1;
    if (l2)
        p->next = l2;

    list_ele_t *head = tmp->next;

    return head;
}

/*
static
list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    if(!l1) return l2;
    if(!l2) return l1;

    if(strcmp(l1->value, l2->value) < 0){
        l1->next = merge(l1->next, l2);
        return l1;
    }else{
        l2->next = merge(l1, l2->next);
        return l2;
    }
}
*/
static list_ele_t *merge_sort_list(list_ele_t *head)
{
    if (!head || !head->next)
        return head;
    list_ele_t *fast = head->next, *slow = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = merge_sort_list(head);
    list_ele_t *l2 = merge_sort_list(fast);

    return merge(l1, l2);
}

void q_sort(queue_t *q)  // merge sort(recursive)
{
    /* TODO */
    if (!q || q->size == 1)
        return;
    q->head = merge_sort_list(q->head);
}

/* iterative merge sort for linked list  (to be finished!)
void merge_sort()
{
    for(int subSize = 1; subSize < n; subSize *= 2){
        for(int start = 0; start < n-1; start += subSize*2){
            int mid = min(start + subSize - 1, n-1);
            int end = min(start + 2*subSize -1, n-1);
            merge(, start, mid, end);
        }
    }
}

void merge()
{
    list_ele_t *leftHead = q->head;
    list_ele_t *rightHead = q->head;
    int tmp = mid - start;
    while(start--){
        leftHead = leaftHead->next;
        rightHead = rightHead->next;
    }
    while(tmp--){
        rightHead = rightHead->next;
    }
}

int min(int x, int y)
{
    return (x < y)? y : x;
}
*/
