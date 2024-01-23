/* CS4-53203: Systems Programming \
/* Name: Garrett Stallsmith \
/* Date: 01/22/2024 \
/* AssignmentWarmup.txt\
*/

#include <stdio.h>
#include <stdlib.h>

struct numlist {
    float *list;    /* points to list of numbers */
    int len;        /* number of items in list */

    float min,      /* the minimal value in list */
    max,            /* the maximal value in list */
    avg;            /* the mean of the numbers */
};

void compute_stats(struct numlist *listptr) {
    // Set min and max to first item in list as we are searching linearly
    // Will use A/B comparisons to find largest / smallest
    listptr->min = listptr->list[0];
    listptr->max = listptr->list[0];

    float accumulator = 0;

    for(int i = 0; i < listptr->len; i++) {
        accumulator += listptr->list[i];            // Accumulator to compute the average later

        if(listptr->list[i] < listptr->min) {       // If we find a smaller number than the previous smallest we replace the min
            listptr->min = listptr->list[i];
        }

        if(listptr->list[i] > listptr->max) {       // If we find a larger number than the previous largest we replace the max
            listptr->max = listptr->list[i];
        }
    }

    listptr->avg = accumulator / listptr->len;      // Average = Total of items / Number of items

}

void print_stats(struct numlist *listptr) {
    printf("Min: %f\n", listptr->min);
    printf("Max: %f\n", listptr->max);
    printf("Average: %f\n", listptr->avg);
}

int main() {
    // Need to allocate memory for the structure    rewind(filePointer);

    struct numlist* nl = (struct numlist*)malloc(sizeof(struct numlist));;

    // Arbitrary float list
    float floatList[] = {3.14, 5.01, 2.33, 10.2, 1.2, 6.12, -9.99, 0.00};

    nl->len = sizeof(floatList) / 4;    // Four bytes per float. Lets us calculate the amount of floats in the list
    nl->list = floatList;

    compute_stats(nl);
    print_stats(nl);


    
    return 0;
}