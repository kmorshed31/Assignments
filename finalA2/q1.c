#include "q1.h"

/*
 * A basic menu based structure has been provided
*/

int main(){

    //Our three Linked Lists
    struct car *available_head = NULL;
	struct car *rented_head = NULL;
	struct car *repair_head = NULL;

    int choice;

    do
    {
        prompt();

        printf("Enter a Choice: ");
        scanf("%d", &choice);
        
        /*
         * You will need to add all the cases
         * Call the appropriate functions
         * You would also need to save data to disc upon exit
         * File Format: plate,mileage,return_date
         * Error Handling needs to happen too
        */

        switch (choice) {
            case 1: {
                char plate[9];
                int mileage;

                printf("Enter plate number (2-8 characters): ");
                scanf("%s", plate);

                printf("Enter mileage: ");
                scanf("%d", &mileage);

                if (!is_plate_in_list(available_head, plate) && !is_plate_in_list(rented_head, plate) && !is_plate_in_list(repair_head, plate)) {
                    // Insert the new car into the available list
                    insert_to_list(&available_head, plate, mileage, -1);
                    sort_list(&available_head, true, false); // Sort by mileage
                    printf("Added car with plate %s to the available-for-rent list.\n", plate);
                } else {
                    printf("Error: A car with this plate number already exists.\n");
                }
                break;
            }
            case 2: {
                char plate[9];
                int mileage;
                printf("Enter plate number: ");
                scanf("%s", plate);
                printf("Enter current mileage: ");
                scanf("%d", &mileage);

                struct car *returned_car = remove_car_from_list(&rented_head, plate);
                if (returned_car) {
                    if (mileage >= returned_car->mileage) {
                        double charge = profit_calculator(returned_car->mileage, mileage);
                        printf("Car with plate %s returned. Total charge: $%.2f\n", plate, charge);
                        returned_car->mileage = mileage;
                        insert_to_list(&available_head, returned_car->plate, returned_car->mileage, -1);
                        sort_list(&available_head, true, false); // Sort by mileage
                    } else {
                        printf("Error: Mileage is invalid (less than recorded).\n");
                        insert_to_list(&rented_head, returned_car->plate, returned_car->mileage, returned_car->return_date);
                    }
                } else {
                    printf("Error: Car not found in rented list.\n");
                }
                break;
            }
            case 3: {
                char plate[9];
                int mileage;
                printf("Enter plate number: ");
                scanf("%s", plate);
                printf("Enter current mileage: ");
                scanf("%d", &mileage);

                struct car *returned_car = remove_car_from_list(&rented_head, plate);
                if (returned_car) {
                    if (mileage >= returned_car->mileage) {
                        double charge = profit_calculator(returned_car->mileage, mileage);
                        printf("Car with plate %s returned and sent to repair. Total charge: $%.2f\n", plate, charge);
                        returned_car->mileage = mileage;
                        insert_to_list(&repair_head, returned_car->plate, returned_car->mileage, -1);
                    } else {
                        printf("Error: Mileage is invalid (less than recorded).\n");
                        insert_to_list(&rented_head, returned_car->plate, returned_car->mileage, returned_car->return_date);
                    }
                } else {
                    printf("Error: Car not found in rented list.\n");
                }
                break;
            }
            case 4: {
               char plate[9];
                printf("Enter plate number: ");
                scanf("%s", plate);

                // Remove from repair list and add to available list
                struct car *car = remove_car_from_list(&repair_head, plate);
                if (car!=NULL) {
                    insert_to_list(&available_head, car->plate, car->mileage, -1);
                    sort_list(&available_head, true, false); // Sort by mileage
                    printf("Car with plate %s transferred from repair to available-for-rent list.\n", plate);
                } else {
                    printf("Error: Car not found in repair list.\n");
                }
                break;
            }
            case 5: {
                // Rent the first available car
                int return_date;
                printf("Enter expected return date (YYMMDD): ");
                scanf("%d", &return_date);

                if (return_date > 0) {
                    // Remove the first car from the available list and add to rented
                    struct car *car = remove_first_from_list(&available_head);
                    if (car) {
                        insert_to_list(&rented_head, car->plate, car->mileage, return_date);
                        sort_list(&rented_head, false, true); // Sort by return date
                        printf("Car with plate %s rented, expected return date: %d.\n", car->plate, return_date);
                    } else {
                        printf("No available cars to rent.\n");
                    }
                } else {
                    printf("Error: Invalid return date.\n");
                }
                break;
            }
            case 6:
                // Print all lists
                printf("\nAvailable cars:\n");
                print_list(available_head);

                printf("\nRented cars:\n");
                print_list(rented_head);

                printf("\nCars in repair:\n");
                print_list(repair_head);
                break;

            case 7:
                // Quit the program
                printf("Saving data and quitting...\n");
                write_list_to_file("available.txt", available_head);
                write_list_to_file("rented.txt", rented_head);
                write_list_to_file("repair.txt", repair_head);

                free_list(&available_head);
                free_list(&rented_head);
                free_list(&repair_head);
                return 0;

            default:
                printf("Invalid choice, please try again.\n");
                break;
        }

    } while (true);

    return 0;
}
    

