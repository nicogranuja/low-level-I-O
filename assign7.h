// Macros
#define INVENTORY "inventory.dat"

// Vars
const char OPTIONS[] = "Enter one of the following actions or press CTRL-D to exit.\n\tC - create a new item\n\tR - read an existing item\n\tU - update an existing item\n\tD - delete an existing item\nOption: ";
typedef struct Item {
    char item_name[64];
    char simple_name[16];
    int current_quantity;
    int max_quantity;
    char body[128];
} item;

// Prototypes
void input_loop(int);
void create_record(int fd);
void read_record(int fd);
void update_record(int fd);
void delete_record(int fd);

// Helper functions prototypes
int item_exists(int fd, int item_number);
struct Item get_item(int fd, int item_number);
void print_item(struct Item item, int item_number);
int get_item_number();
struct Item get_item_data();
void write_struct(int fd, int item_number, struct Item item);
void replace_empty_values(struct Item *new_item, struct Item *item);