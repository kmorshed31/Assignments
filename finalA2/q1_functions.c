#include "q1.h"


/* function will display the primary user interface
    This is already done for you*/
void prompt(){

  printf("1. Add a new car to the available-for-rent list,\n");
  printf("2. Add a returned car to the available-for-rent list,\n");
  printf("3. Add a returned car to the repair list,\n");
  printf("4. Transfer a car from the repair list to the available-for-rent list,\n");
  printf("5. Rent the first available car,\n");
  printf("6. Print all the lists,\n");
  printf("7. Quit.\n");
  printf("Enter your choice: ");
}

/**
 * Inserts a new car into the linked list.
 * @param head Pointer to the head of the list.
 * @param plate Character array containing the car's license plate.
 * @param mileage Integer representing the car's mileage.
 * @param return_date Integer representing the car's return date, you can have -1 represent no return date.
 * @return Pointer to the newly added car in the list.
 */
struct car * insert_to_list(struct car ** head, char plate[], int mileage, int return_date){
    struct car *ptr = malloc(sizeof(struct car));
    if (ptr == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }

    strcpy(ptr->plate, plate);
    ptr->mileage = mileage;
    ptr->return_date = return_date;
    ptr->next = *head;
    *head = ptr;
    return NULL;
}

/**
 * Prints all the cars in the list.
 * @param head Pointer to the head of the list.
 * This function prints out the car details, it should not print return date if it does not exist.
 */
void print_list(struct car *head){

        while (head != NULL) {
            printf("Mileage: %d\n ",head->mileage);
            printf("Return Date: %d\n ", head->return_date);
            printf("Car plate: %s\n", head->plate);

            head = head->next;
        }


    return;
}

/**
 * Checks if a car's plate is already in the list.
 * @param head Pointer to the head of the list.
 * @param plate Character array containing the car's license plate.
 * @return Boolean value indicating if the plate is found.
 */
bool is_plate_in_list(struct car * head, char plate[]){
    struct car* temporary = head;

    while (temporary != NULL) {
        if (strcmp(temporary->plate, plate) == 0) {
            return true;
        }
        temporary = temporary->next;  //We traverse through the array by using temporary->next until we reach NULL
    }

    return false;

}

/**
 * Swaps two cars in the list.
 * @param a Pointer to the first car.
 * @param b Pointer to the second car.
 * Swaps the plate, mileage, and return date of two cars.
 */
void swap(struct car *a, struct car *b){
    struct car *temp= (struct car*)malloc(sizeof(struct car));

    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }


    *temp= *a;  // Dereference and run the usual swapping algorithm
    *a = *b;
    *b = *temp;

    free(temp);
    return;
}

/**
 * Sorts the list of cars based on mileage or return date.
 * @param head Pointer to the head of the list.
 * @param sort_by_mileage Boolean to sort by mileage.
 * @param sort_by_return_date Boolean to sort by return date.
 */
void sort_list(struct car ** head, bool sort_by_mileage, bool sort_by_return_date){
    if (*head == NULL) {
        return;
    }

    bool swapped;
    struct car *ptr;
    struct car *lptr = NULL;  // Last pointer, I made this because we have to work with last list element

    // Bubble sort approach
    do {
        swapped = false;
        ptr = *head;

        while (ptr->next != lptr) {
            bool should_swap = false;

            // I determine whether to swap based on the sorting criteria
            if (sort_by_mileage && ptr->mileage > ptr->next->mileage) {
                should_swap= true;
            } else if (sort_by_return_date && ptr->return_date > ptr->next->return_date) {
                should_swap = true;
            }

            if (should_swap) {
                swap(ptr, ptr->next);
                swapped= true;
            }

            ptr = ptr->next;
        }
        lptr= ptr;
    } while (swapped);
    return;
}

/**
 * Removes a car from the list by its plate number.
 * @param head Pointer to the head of the list.
 * @param plate Character array containing the car's license plate.
 * @return Pointer to the removed car.
 */
struct car * remove_car_from_list(struct car **head, char plate[]){
    struct car *temp = *head;
    struct car *prev= NULL;

    // I'll check if the head node has the car I want to delete
    if (temp != NULL && strcmp(temp->plate, plate) == 0) {
        *head= temp->next;  // Move the head to the next node
        free(temp);          // Free the removed node
        return NULL;         // Return NULL after successful deletion
    }

    // Traverse through the list to find the car Iwant to delete
    while (temp != NULL && strcmp(temp->plate, plate) != 0) {
        prev= temp;
        temp = temp->next;
    }

    // If the car was not found, return NULL
    if (temp== NULL) {
        return NULL;
    }

    // Unlink the car from the linked list
    prev->next = temp->next;

    // Free the memory of the removed node
    free(temp);

    // Return NULL to indicate the function has finished successfully
    return NULL;
}

/**
 * Removes the first car from the list.
 * @param head Pointer to the head of the list.
 * @return Pointer to the removed car.
 */
struct car * remove_first_from_list(struct car **head){

        struct car *temp= *head;

        if (temp== NULL) {
            return NULL;
        }

        // shift the head to next node
        *head = temp->next;

        free(temp);

        return NULL;

}

/**
 * Calculates the profit based on the mileage difference.
 * @param initial_mileage Integer representing the car's starting mileage.
 * @param final_mileage Integer representing the car's final mileage.
 * @return Double value representing the calculated profit.
 */
double profit_calculator(int initial_mileage, int final_mileage){
    int total_miles =final_mileage - initial_mileage;
    double profit= 80.00;  // specific rate  for up to 200 km

    if (total_miles > 200) {
        // Add extra charge for kilometers after 200 km
        profit +=(total_miles - 200) * 0.15;
    }

    return profit;

}

/**
 * Writes the list of cars to a file.
 * The file would be in the format: plate,mileage,return_date
 * @param filename Character array with the file name.
 * @param head Pointer to the head of the list.
 * Writes the details of each car in the list to a file.
 */
void write_list_to_file(char *filename, struct car *head){
    FILE *file = fopen(filename, "w");  //open the file in writing mode
    if (file == NULL) {
        printf("Error opening file %s for writing.\n", filename);
        return;
    }

    struct car *temp = head;

    //write each car's details to the file
    while (temp != NULL) {
        fprintf(file, "Plate: %s, Mileage: %d, Return Date: %d\n",
                temp->plate, temp->mileage, temp->return_date);
        temp = temp->next;
    }

    fclose(file);
    return;

}

/**
 * Reads car details from a file and adds them to the list.
 * The file would be in the format: plate,mileage,return_date
 * @param filename Character array with the file name.
 * @param head Pointer to the head of the list.
 * Reads data from the file and inserts each car into the list.
 */
void read_file_into_list(char *filename, struct car **head){
    FILE *file = fopen(filename, "r");  //in read mode
    if (file == NULL) {
        printf("Error opening file %s for reading.\n", filename);
        return;
    }

    struct car *temp = NULL;
    struct car *new_car = NULL;
    char plate[10];
    int mileage, return_date;


    while (fscanf(file, "%s %d %d\n", plate, &mileage, &return_date) == 3) {

        new_car = malloc(sizeof(struct car));
        if (new_car == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file);
            return;
        }


        strcpy(new_car->plate, plate);
        new_car->mileage = mileage;
        new_car->return_date = return_date;
        new_car->next = NULL;


        if (*head == NULL) {
            *head = new_car;  // If the list is empty, this car becomes the head
        } else {
            temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = new_car;
        }
    }

    fclose(file);
    return;
}

/**
 * Prints the date in the format of year/month/day.
 * @param date Integer representing the date in YYMMDD format.
 */
void date(int date) {
    int year = date / 10000;       // Extract the first 2 digits
    int month= (date / 100) % 100;  // Extract the middle 2 digits
    int day = date % 100;          // Extract the last 2 digits (DD)

    printf("%02d/%02d/%02d\n", year, month, day);
}


/**
 * Frees all the memory allocated for the cars in the list.
 * @param head Pointer to the head of the list.
 * Frees each car node in the list.
 */
void free_list(struct car ** head){
    struct car *current= *head;
    struct car *next_node;


    while (current != NULL) {
        next_node= current->next;
        free(current);
        current =next_node;
    }

    *head =NULL;
    return;

}