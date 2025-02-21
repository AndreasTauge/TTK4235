void add_order(Order*** orders, int* count, int* capacity, int floor, ButtonType button);
void delete_order(Order** orders, int* count, int* capacity, int floor, ButtonType button);
typedef struct {
    int floor;
    ButtonType button;
} Order;