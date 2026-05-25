#include <stdio.h>
#include <string.h>
#include <string.h>

#define MAX_USER_LEN 32
#define MAX_DOC_LEN 64
#define LINE_LEN 256

typedef struct PrintJob PrintJob;
typedef void (*PrintFn)(PrintJob *);

struct PrintJob {
    int job_id;

    char username[MAX_USER_lEN];
    char document[MAX_DOC_LEN];

    int pages;

    PrintFn print_fn;
    PrintJob *next;
};


void print_job(PrintJob *job)
{
    if (job == NULL) {
        return;
    }

    printf(
        "%d %s %s %d\n",
        job->job_id,
        job->username,
        job->document,
        job->pages
    );
}

PrintJob *create_print_job(
    int job_id,
    const char *username,
    const char *document,
    int pages,
    PrintFn fn
)
{
    PrintJob *job = malloc(sizeof(PrintJob));
    if(job == NULL){
        return NULL;
    }
    strncpy(job->username, username, MAX_USER_LEN - 1);
    job->username[MAX_USER_LEN - 1] = '\0';
    strncpy(job->document, document, MAX_DOC_LEN - 1);
    job->document[MAX_DOC_LEN - 1] = '\0';
    job->job_id = job_id;
    job->pages = pages;
    job->print_fn = fn;
    job->next = NULL;

    return job;
}

int append_print_job(PrintJob **head, PrintJob *new_job)
{
    if(head == NULL || new_job == NULL){
        return 0;
    }

    new_job->next = NULL;

    if(*head == NULL){
        *head = new_job;
        return 1;
    }

    PrintJob *curr = *head;
    while (curr->next != NULL){
        curr = curr->next;
    }
    curr->next = new_job;
    return 1;
}

PrintJob *find_print_job(PrintJob *head, int job_id)
{
    PrintJob *curr = head;

    while (curr != NULL){
        if(curr->job_id == job_id){
            return curr;
        }

        curr = curr -> next;
    }
    return NULL;
}

int remove_print_job(PrintJob **head, int job_id)
{
    if(head == NULL || *head == NULL){
        return 0;
    }

    PrintJob *curr = *head;
    PrintJob *prev = NULL;

    while(curr != NULL){
        if (curr->job_id == job_id){

            if(prev == NULL){
                *head = curr->next;
            }else{
                prev->next = curr->next;
            }
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;
}

void free_print_jobs(PrintJob *head)
{
    PrintJob *curr = head;
    while (curr != NULL){
        PrintJob *next = curr->next;
        free(curr);
        curr = next;
    }
}

int load_print_jobs(const char *filename, PrintJob **head)
{
    if(filename == NULL || head == NULL){
        return 0;
    }

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return 0;
    }

    char line[LINE_LEN];

    while (fgets(line, LINE_LEN, fp) != NULL) {

        int job_id;
        char username[MAX_USER_LEN];
        char document[MAX_DOC_LEN];
        int pages;
        if (sscanf(line, "%d %s %s %d", &job_id, username, document, &pages) != 4) {
            continue;
        }
        PrintJob *job = create_print_job(
            job_id,
            username,
            document,
            pages,
            print_job
        );

        if(job==NULL){
            fclose(fp);
            return 0;
        }
        if(append_print_job(head, job) == 0){
            free(job);
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}

PrintJob *largest_print_job(PrintJob *head)
{
    if (head == NULL){
        return NULL;
    }
    PrintJob *best = head;
    PrintJob *curr = head->next;

    while(curr != NULL){
        if(curr->pages > best->pages){
            best = curr;
        }
        curr = curr->next;
    }
    
    return best;
}