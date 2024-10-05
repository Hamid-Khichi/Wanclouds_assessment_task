#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data Structure
typedef struct {
    char *name;
    int id;
} Data;

// Node Structure
typedef struct Node {
    Data data;
    struct Node *prev;
    struct Node *next;
} Node;

// Function prototypes
void insert_end(Node **head_ref, char *name, int id);
void delete_node(Node **head_ref, int id);
void print_list(Node *node);
void free_list(Node **head_ref);
void merge_sort(Node **head_ref);
Node* sorted_merge(Node *a, Node *b);
void split_list(Node *source, Node **front_ref, Node **back_ref);

int main() {
    Node *head = NULL;
    char name[100];
    int id;
    int choice;

    while (1)
    {
        printf("\nMenu:\n");
        printf("1. Insert node\n");
        printf("2. Delete node\n");
        printf("3. Print list\n");
        printf("4. Sort list\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
            case 1:
                printf("Enter name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                printf("Enter ID: ");
                scanf("%d", &id);
                getchar();
                insert_end(&head, name, id);
                break;
            case 2:
                printf("Enter ID to delete: ");
                scanf("%d", &id);
                getchar();
                delete_node(&head, id);
                break;
            case 3:
                print_list(head);
                break;
            case 4:
                merge_sort(&head);
                printf("List Sorted\n");
                break;
            case 5:
                free_list(&head);
                exit(0);
                break;
            default:
                printf("Invalid choice\n");
                break;
        } 
    }
    return 0;
}

// Functions definitions

void insert_end(Node **head_ref, char *name, int id)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data.name = strdup(name);
    new_node->data.id = id;
    new_node->next = NULL;

    if (*head_ref == NULL)
    {
        new_node->prev = NULL;
        *head_ref = new_node;
        return;
    }

    Node *last = *head_ref;
    while (last->next != NULL)
        last = last->next;
    
    last->next = new_node;
    new_node->prev = last;
}

void delete_node(Node **head_ref, int id)
{
    Node *current = *head_ref;

    while (current != NULL && current->data.id != id)
        current = current->next;
    
    if (current == NULL)
    {
        printf("Node with ID %d not found\n", id);
        return;
    }

    if (*head_ref == current)
        *head_ref = current->next;
    
    if (current->next != NULL)
        current->next->prev = current->prev;

    if (current->prev != NULL)
        current->prev->next = current->next;
    

    free(current->data.name);
    free(current);
}

void print_list(Node *node)
{
    while (node != NULL)
    {
        printf("ID: %d, Name: %s\n", node->data.id, node->data.name);
        node = node->next;
    }
}

void free_list(Node **head_ref)
{
    Node *current = *head_ref;
    Node *next;

    while (current != NULL)
    {
        next = current->next;
        free(current->data.name);
        free(current);
        current = next;
    }

    *head_ref = NULL;
}

void merge_sort(Node **head_ref)
{
    Node *head = *head_ref;
    if ((head == NULL) || (head->next == NULL))
        return;
    
    Node *a;
    Node *b;

    split_list(head, &a, &b);

    merge_sort(&a);
    merge_sort(&b);

    *head_ref = sorted_merge(a, b);
}

Node* sorted_merge(Node *a, Node *b)
{
    Node *result = NULL;

    if (a == NULL)
        return b;
    else if (b == NULL)
        return a;

    if (a->data.id <= b->data.id)
    {
        result = a;
        result->next = sorted_merge(a->next, b);
        if (result->next != NULL)
            result->next->prev = result;
        result->prev = NULL;
    }
    else
    {
        result = b;
        result->next = sorted_merge(a, b->next);
        if (result->next != NULL)
            result->next->prev = result;
        result->prev = NULL;
    }

    return result;
}

void split_list(Node *source, Node **front_ref, Node **back_ref)
{
    Node *fast;
    Node *slow;
    slow = source;
    fast = source->next;

    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }

    *front_ref = source;
    *back_ref = slow->next;
    slow->next = NULL;
    if (*back_ref != NULL)
        (*back_ref)->prev = NULL;
}