#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node* next;
};
typedef struct node node_t;

int count_list(node_t* head) {
    node_t* temp = head;
    int counter = 0;
    while(temp != NULL) {
        temp = temp->next;
        counter++;
    }
    return counter;
}

void display(node_t* head) {
    while(head != NULL) {
        printf("%d ",head->data);        
        head = head->next;
    }
    printf("\n");
}

node_t* create(node_t* head, int value) {
    head = (node_t*)malloc(sizeof(node_t));
    head->data = value;
    head->next = NULL;
    return head;
}

node_t* insert_beginning(node_t* head, int value) {
    node_t* new = (node_t*)malloc(sizeof(node_t));
    new->data = value;
    new->next = head;
    head = new;
    return head;
}

node_t* insert_middle(node_t* head, int value, int position) {
    node_t* temp = head;
    for(int i=0;i<position-1;i++) {
        temp = temp->next;
    }
    node_t* new = (node_t*)malloc(sizeof(node_t));
    new->next = temp->next;
    temp->next = new;
    new->data = value;
    return head;
}

node_t* insert_end(node_t* head, int value) {
    node_t* temp = head;
    while(temp->next != NULL) {
        temp = temp->next;
    }
    node_t* new = (node_t*)malloc(sizeof(node_t));
    temp->next = new;
    new->data = value;
    new->next = NULL;
    return head;
}

node_t* delete_beginning(node_t* head) {
    node_t* temp = head;
    head = head->next;
    free(temp);
    return head;
}

node_t* delete_end(node_t* head) {
    node_t* temp = head;
    while(temp->next->next != NULL) {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
    return head;
}

node_t* delete_middle(node_t* head, int position) {
    node_t* temp = head;
    for(int i=0;i<position-1;i++) {
        temp = temp->next;
    }
    node_t* temp2 = temp->next;
    temp->next = temp2->next;
    free(temp2);
    return head;
}

int search(node_t* head, int value) {
    node_t* temp = head;
    while(temp != NULL) {
        if(temp->data == value) {
            return value;
        }
        temp = temp->next;
    }
    return -1;
}

node_t* add_sorted_2(node_t* head, int value) {
    node_t* new = (node_t*)malloc(sizeof(node_t));
    new->data = value;
    new->next = NULL;

    if(head == NULL || value > head->data) {
        new->next = head;
        head = new;
        return head;
    }

    node_t* current = head;
    while (current->next != NULL && current->next->data > value) {
        current = current->next;
    }

    new->next = current->next;
    current->next = new;
    return head;
}

node_t* add_sorted(node_t* head, int value) { // 7 -> 6 -> 5 -> 4 -> 3 ->
    node_t* temp = head;
    int counter = 0;
    int length = count_list(head);
    int flag = 0;
    
    if(value > head->data) {
        return insert_beginning(head, value);
    }

    while(temp != NULL) {
        counter++;
        if(counter != length) {
            if(temp->next->data < value) {
                flag = 1;
                break;
            }
        }
        else {
            if(temp->next == NULL) {
                flag = 1;
                break;
            }
        }
        temp = temp->next;
    }

    if(counter < length)  {
        return insert_middle(head, value, counter);
    }
    if(flag == 1 && counter == length) {
        return insert_end(head, value);
    }

    return head;
}

node_t* delete_by_id(node_t* head, int value) {
    node_t* temp = head;
    int counter = 0;
    int length = count_list(head);
    int flag = 0;

    while(temp != NULL) {
        counter++;
        if(temp->data == value) {
            flag = 1;
            break;
        }
        temp = temp->next;
    }

    if(counter == 1) {
        return delete_beginning(head);
    }
    if(counter < length)  {
        return delete_middle(head, counter-1);
    }
    if(flag == 1 && counter == length) {
        return delete_end(head);
    }
    
    return head;
}

int main() {
    node_t* head = NULL;
    head = add_sorted_2(head, 5);
    display(head);
    return 0;
}



