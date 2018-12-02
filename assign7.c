#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "assign7.h"

int main () {
    // Open for read and write and create file if it does not exist
    int fd = open(INVENTORY, O_RDWR | O_CREAT);
    
    if (fd < 0) {
        printf("Could not open file %s", INVENTORY);
        return 1;
    }

    input_loop(fd);
    close(fd);

    return 0;
}

void input_loop(int fd) {
    char opt;
    int keep_going = 1;
    int count = 0;

    while (keep_going) {
        printf("%s", OPTIONS);
        scanf(" %c", &opt);

        count++;
        if (count == 3) return;

        switch (tolower(opt)) {
            case 'c':
                create_record(fd);
                break;
            case 'r':
                read_record(fd);
                break;
            case 'u':
                update_record(fd);
                break;
            case 'd':
                delete_record(fd);
                break;
            default:
                printf("Invalid option %c please try again.\n", opt);
                break;
        }
    }
}

void create_record (int fd) {
    int item_number = get_item_number();
    struct Item item = get_item(fd, item_number);

    if (item.max_quantity != 0) {
        printf("ERROR: item %d already exists\n", item_number);
        return;
    }

    item = get_item_data();
    write_struct(fd, item_number, item);
}

void read_record (int fd) {
    struct Item item;
    int item_number = get_item_number();
    
    item = get_item(fd, item_number);
    print_item(item, item_number);
}

void update_record (int fd) {
    int item_number = get_item_number();
    struct Item item = get_item(fd, item_number);

    if (item.max_quantity == 0) {
        printf("ERROR: item not found\n");
        return;
    }

    struct Item new_item = get_item_data();

    // Check for empty values in the struct
    replace_empty_values(&new_item, &item);
    write_struct(fd, item_number, new_item);
}

void delete_record (int fd) {
    int item_number = get_item_number();
    struct Item item = get_item(fd, item_number);

    if (item.max_quantity == 0) {
        printf("ERROR: item not found\n");
        return;
    }

    struct Item empty_item;
    write_struct(fd, item_number, empty_item);
    printf("%s was successfully deleted.\n", item.simple_name);
}


// Helper functions
struct Item get_item (int fd, int item_number) {
    // Initialize item
    struct Item item;
    item.max_quantity = 0;

    long seek_offset = item_number * sizeof(item);
    int seek_result = lseek(fd, seek_offset, SEEK_SET);

    // Error could not find item number in the offset. Returning not found empty item struct with max qty = 0
    if (seek_result < 0) {
        return item;
    }

    int read_result = read(fd, &item, sizeof(item));

    // Error reading returning empty item qty = 0
    if (read_result < 0) {
        return item;
    }

    return item;
}

void print_item (struct Item item, int item_number) {
    if (item.max_quantity == 0) {
        printf("\nERROR: item not found\n");
        return;
    }

    printf("Item Name: %s\n", item.item_name);
    printf("Simple Name: %s\n", item.simple_name);
    printf("Item Number: %d\n", item_number);
    printf("Qty: %d/%d\n", item.current_quantity, item.max_quantity);
    printf("Description: %s\n", item.body);
}

int get_item_number () {
    int item_number;
    printf("\nEnter an item number: ");
    scanf("%d", &item_number);

    return item_number;
}

struct Item get_item_data () {
    struct Item item;
    
    printf("Simple Name: ");
    scanf("%s", item.simple_name);

    printf("Item Name: ");
    scanf(" %64[^\n]s", item.item_name);

    printf("Curent Qty: ");
    scanf("%d", &item.current_quantity);

    printf("Maximum Qty: ");
    scanf("%d", &item.max_quantity);

    printf("Description: ");
    scanf(" %128[^\n]s", item.body);

    return item;
}

void write_struct (int fd, int item_number, struct Item item) {
     // Move file pointer to the right position in the file
    long seek_offset = item_number * sizeof(item);
    int seek_result = lseek(fd, seek_offset, SEEK_SET);

    // Error could not find item number in the offset. Returning not found empty item struct with max qty = 0
    if (seek_result < 0) {
        printf("There was an error for seek offset %ld", seek_offset);
        return;
    }

    // Write struct to the file
    int write_result = write(fd, &item, sizeof(item));

    if (write_result < 0) {
        printf("Could not write struct with item number %d to the file", item_number);
    }
}


void replace_empty_values(struct Item *new_item, struct Item *item) {
    // Check for empty values and replace them.
    if (new_item->simple_name[0] == '\0') {
        strcpy(new_item->simple_name, item->simple_name);
    }

    if (new_item->item_name[0] == '\0') {
        strcpy(new_item->item_name, item->item_name);
    }

    // If it is empty or 0
    if (new_item->max_quantity == 0) {
        new_item->max_quantity = item->max_quantity;
    }

    if (new_item->body[0] == '\0') {
        strcpy(new_item->body, item->body);
    }   
}