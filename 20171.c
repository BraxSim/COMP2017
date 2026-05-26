#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 64
#define MAX_STATUS_LEN 32
#define LINE_LEN 256
#define BONUS_BOOKS 2

typedef struct reservation {
    int reservation_id;
    char student[MAX_NAME_LEN];
    int books;
    char status[MAX_STATUS_LEN];
    struct reservation *next;
} Reservation;

Reservation *create_reservation(
    int reservation_id,
    const char *student,
    int books,
    const char *status
) {
    if(student == NULL || status == NULL){
        return NULL;
    }
    Reservation *reservation = malloc(sizeof(Reservation));
    if (reservation == NULL){
        return NULL;
    } 

    reservation->reservation_id = reservation_id;
    reservation->books = books;
    reservation->next = NULL;

    strncpy(reservation->student, student, MAX_NAME_LEN - 1);
    reservation->student[MAX_NAME_LEN - 1] = '\0';
    strncpy(reservation->status, status, MAX_STATUS_LEN -1);
    reservation->status[MAX_STATUS_LEN - 1] = '\0';
    return reservation;
}
    

int append_reservation(
    Reservation **head,
    Reservation *new_reservation
) {
    if(head == NULL || new_reservation == NULL){
        return 0;
    }

    if(head == NULL){
        *head = new_reservation;
        return 1;
    }

    Reservation *curr = *head;

    while(curr->next != NULL){
        curr = curr->next;
    }
    curr -> next = new_reservation;
    return 1;
}

int remove_reservation(
    Reservation **head,
    int reservation_id
) {
    if(head == NULL || *head == NULL){
        return 0;
    }
    Reservation *curr = *head;
    Reservation *prev = NULL;

    while(curr != NULL){
        if(curr->reservation_id == reservation_id){
            if(prev == NULL){
                *head = curr -> next;
            }else{
                prev -> next = curr ->next;
            }
        free(curr);
        return 1;
        }

        prev = curr;
        curr = curr -> next;
    }
    return 0;
}

void adjust_books(
    Reservation *head,
    int (*adjust_fn)(int)
) {
    if(adjust_fn == NULL){
        return;
    }

    Reservation *curr = head;
     
    while(curr != NULL){
        curr -> books = adjust_fn(curr -> books);
        curr = curr -> next;
    }
}

int load_reservations(
    const char *filename,
    Reservation **head
) {
    if(filename == NULL || head == NULL){
        return 0;
    }
    
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return 0;
    }

    char line[LINE_LEN];
    int count = 0;

    while (fgets(line, LINE_LEN, fp) != NULL) {
        int id;
        char student[MAX_NAME_LEN];
        int books;
        char status[MAX_STATUS_LEN];

        int matched = sscanf(
            line,
            "%d,%63[^,],%d,%31s",
            &id,
            student,
            &books,
            status
        );

        if (matched != 4) {
            continue;
        }

        Reservation *new_reservation = create_reservation(
            id,
            student,
            books,
            status
        );

        if (new_reservation == NULL) {
            fclose(fp);
            return count;
        }

        if (append_reservation(head, new_reservation) == 0) {
            free(new_reservation);
            fclose(fp);
            return count;
        }

        count++;
    }

    fclose(fp);
    return count;
}



void free_reservations(Reservation *head) {
    Reservation *curr = head;

    while (curr != NULL){
        Reservation *next = curr -> next;
        free(curr);
        curr = next;
    }
}