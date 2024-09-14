#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

class Product {
private:
    string category;
    string itemID;
    string itemName;
    int itemPrice;
    int itemStock;
    int productID;

public:
    // Parameterized constructor
    Product(string showCategory = "", string showItemID = "", string showItemName = "", int showItemPrice = 0, int showItemStock = 0) {
        category = showCategory;
        itemID = showItemID;
        itemName = showItemName;
        itemPrice = showItemPrice;
        itemStock = showItemStock;
    }

    // Getters for Product attributes
    string getItemID() const { return itemID; }
    string getItemName() const { return itemName; }
    int getItemPrice() const { return itemPrice; }
    int getItemStock() const { return itemStock; }

    // Function to display items within the same category
    void displayItems(Product productList[], int size) {
        string currentCategory = "";

        for (int i = 0; i < size; ++i) {
            Product product = productList[i];

            if (currentCategory != product.category) {
                // Print category header when the category changes
                if (currentCategory != "") {
                    cout << endl;  // Extra line between categories
                }
                currentCategory = product.category;
                cout << "\tCategory: " << currentCategory << endl;
                cout << "\t\tProduct ID\tName\t\t\t\tPrice\t\tStock" << endl;
            }

            // Display each product in the category with adjusted column widths
            cout << "\t\t" << setw(8) << left << product.itemID << "\t"
                 << setw(25) << left << product.itemName << "\t"
                 << "Php " << setw(8) << left << product.itemPrice << "\t"
                 << product.itemStock << endl;
        }
    }
    
    int getProductID() const {
        return productID;
    }

    // Define operator== for comparing two Product objects
    bool operator==(const Product& other) const {
        return productID == other.productID;
    }
};

class Order {
private:
    static const int MAX_ITEMS = 100; // Define max items if needed
    static int nextOrderID; // Static variable to keep track of the next order ID
    Product orderedItems[MAX_ITEMS]; // Array to hold items
    int quantities[MAX_ITEMS]; // Quantities of the items
    int itemCount; // Number of items in the order
    int orderID; // ID of the order
    float totalAmount; // Total amount for items
    float shippingFee; // Shipping fee
    float totalPayment; // Total payment (items + shipping)
    string paymentOption; // Payment option
    int orderSize; // Number of items added to the order

public:
    // Constructor
    Order() : itemCount(0), totalAmount(0), shippingFee(0), totalPayment(0), orderSize(0) {
        orderID = nextOrderID++; // Assign the current nextOrderID and increment it
    }

    // Static function to initialize nextOrderID
    static void initializeOrderID(int startID) {
        nextOrderID = startID;
    }

    void setOrderID(int id) { orderID = id; }
    int getOrderID() const { return orderID; }

    // Setter and Getter for totalAmount
    void setTotalAmount(float amount) {
        totalAmount = amount;
    }

    float getTotalAmount() const {
        return totalAmount;
    }

    // Setter and Getter for shippingFee
    void setShippingFee(float fee) {
        shippingFee = fee;
    }

    float getShippingFee() const {
        return shippingFee;
    }

    // Function to add items to the order
    void addItemToOrder(Product product, int quantity) {
        if (orderSize < MAX_ITEMS) {
            orderedItems[orderSize] = product;
            quantities[orderSize] = quantity;
            totalAmount += product.getItemPrice() * quantity;
            orderSize++;
        }
    }

    // Function to calculate total payment (merchandise subtotal + shipping)
    void calculateTotalPayment() {
        totalPayment = totalAmount + shippingFee;
    }

    // Function to set the payment option
    void setPaymentOption(const string& option) {
        paymentOption = option;
    }

    // Display order summary
    void displayOrderSummary() const {
        cout << "\tOrder Summary" << endl;
        cout << "\t\tProduct ID\tName\t\t\t\tPrice\t\tQuantity" << endl;

        for (int i = 0; i < orderSize; ++i) {
            cout << "\t\t" << setw(8) << left << orderedItems[i].getItemID() << "\t"
                 << setw(25) << left << orderedItems[i].getItemName() << "\t"
                 << "Php " << setw(8) << left << orderedItems[i].getItemPrice() << "\t"
                 << quantities[i] << endl;
        }

        cout << "\n\tOrder Total: Php " << totalAmount << endl;
        cout << "\tShipping Fee: Php " << shippingFee << endl;
        cout << "\tTotal Payment: Php " << totalPayment << endl;
        cout << "\tPayment Option: " << paymentOption << endl;
    }

    // Transfer items from the cart to the order
    void transferItemsFromCart(Product cartItems[], int quantities[], int cartSize) {
        for (int i = 0; i < cartSize; ++i) {
            addItemToOrder(cartItems[i], quantities[i]);
        }
    }
    
    void displayOrderDetails() const {
        cout << "\nOrder ID: " << orderID << endl;
        cout << "Total Amount: Php " << totalAmount << endl;
        cout << "Order Details:" << endl;
        cout << "\tProduct ID\tName\t\t\tPrice\t\tQuantity" << endl;

        for (int i = 0; i < orderSize; ++i) {
            cout << "\t" << setw(8) << left << orderedItems[i].getItemID() << "\t"
                 << setw(25) << left << orderedItems[i].getItemName() << "\t"
                 << "Php " << setw(8) << left << orderedItems[i].getItemPrice() << "\t"
                 << quantities[i] << endl;
        }

        cout << "\n\tOrder Total: Php " << totalAmount << endl;
        cout << "\tShipping Fee: Php " << shippingFee << endl;
        cout << "\tTotal Payment: Php " << totalPayment << endl;
    }	    
};

// Initialize the static variable
int Order::nextOrderID = 1; // Start with 1 for the first order

class ShoppingCart {
private:
    static const int MAX_CART_ITEMS = 100; // Define max cart items
    Product cartItems[MAX_CART_ITEMS]; // Array to hold items in the cart
    int quantities[MAX_CART_ITEMS]; // Quantities of the items
    int cartSize; // Number of items in the cart
    int totalQuantity;
    float totalAmount;
    float merchandiseSubtotal;
    float shippingSubtotal;
    float totalPayment;
    bool orderPlaced;
    Order lastOrder;  // Store the last order

    string toUpperCase(const string& str) const {
        string upperStr = str;
        for (char& c : upperStr) {
            c = toupper(c);
        }
        return upperStr;
    }

    bool isValidNumber(const string& str) const {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    }

    int stringToInt(const string& str) const {
        return stoi(str);
    }

    void calculateTotalPayment() {
        totalPayment = merchandiseSubtotal + shippingSubtotal;
    }

    void setOrderPlaced(bool status) {
        orderPlaced = status;
    }

public:
    ShoppingCart() : cartSize(0), totalQuantity(0), totalAmount(0), merchandiseSubtotal(0), shippingSubtotal(0), totalPayment(0), orderPlaced(false) {}

    void addItem(Product productList[], int size) {
        string inputID;
        string inputQuantity;
        int quantity;
        bool itemFound = false;
        string addMore = "N";

        do {
            while (true) {
                cout << "\n\tEnter the Product ID you want to add to your Shopping Cart: ";
                cin >> inputID;
                inputID = toUpperCase(inputID);

                itemFound = false;
                for (int i = 0; i < size; ++i) {
                    string productID = toUpperCase(productList[i].getItemID());

                    if (productID == inputID) {
                        itemFound = true;
                        while (true) {
                            cout << "\tEnter the Quantity: ";
                            cin >> inputQuantity;

                            if (isValidNumber(inputQuantity)) {
                                quantity = stringToInt(inputQuantity);

                                if (quantity > productList[i].getItemStock()) {
                                    cout << "\t\tSorry, we only have " << productList[i].getItemStock() << " in stock.\n" << endl;
                                } else {
                                    bool alreadyInCart = false;

                                    for (int j = 0; j < cartSize; ++j) {
                                        if (toUpperCase(cartItems[j].getItemID()) == inputID) {
                                            quantities[j] += quantity;
                                            totalQuantity += quantity;
                                            totalAmount += quantity * productList[i].getItemPrice();
                                            merchandiseSubtotal += quantity * productList[i].getItemPrice();
                                            cout << "\tUpdated quantity for " << productList[i].getItemName() << " to " << quantities[j] << endl;
                                            alreadyInCart = true;
                                            break;
                                        }
                                    }

                                    if (!alreadyInCart) {
                                        cartItems[cartSize] = productList[i];
                                        quantities[cartSize] = quantity;
                                        totalQuantity += quantity;
                                        totalAmount += quantity * productList[i].getItemPrice();
                                        merchandiseSubtotal += quantity * productList[i].getItemPrice();
                                        cartSize++;
                                        cout << "\t\tProduct added successfully!\n" << endl;
                                    }
                                    break;
                                }
                            } else {
                                cout << "\t\tInvalid input. Please enter a valid number.\n" << endl;
                            }
                        }
                        break;
                    }
                }

                if (itemFound) {
                    break;
                } else {
                    cout << "\t\tProduct ID not found. Please try again." << endl;
                }
            }

            while (true) {
                cout << "\tDo you want to add more products? (Y/N): ";
                cin >> addMore;
                addMore = toUpperCase(addMore);

                if (addMore == "Y" || addMore == "N") {
                    break;
                } else {
                    cout << "\t\tInvalid input. Please enter 'Y' or 'N'.\n" << endl;
                }
            }

        } while (addMore == "Y");
    }

    void displayCart() {
        if (cartSize == 0) {
            cout << "\n\tYour shopping cart is empty." << endl;
            return;
        }

        cout << "\tYour Cart" << endl;
        cout << "\t\tProduct ID\tName\t\t\t\tPrice\t\tQuantity" << endl;

        for (int i = 0; i < cartSize; ++i) {
            cout << "\t\t" << setw(8) << left << cartItems[i].getItemID() << "\t"
                 << setw(25) << left << cartItems[i].getItemName() << "\t"
                 << "Php " << setw(8) << left << cartItems[i].getItemPrice() << "\t"
                 << quantities[i] << endl;
        }

        cout << "\n\tTotal Quantity: " << totalQuantity << endl;
        cout << "\tTotal Amount: Php " << totalAmount << endl;
    }

    void displayOrderSummary() {
        cout << "\n\tOrder Summary" << endl;
        cout << "\t\tProduct ID\tName\t\t\t\tPrice\t\tQuantity" << endl;

        for (int i = 0; i < cartSize; ++i) {
            cout << "\t\t" << setw(8) << left << cartItems[i].getItemID() << "\t"
                 << setw(25) << left << cartItems[i].getItemName() << "\t"
                 << "Php " << setw(8) << left << cartItems[i].getItemPrice() << "\t"
                 << quantities[i] << endl;
        }

        cout << "\n\tOrder Total (Items): " << totalQuantity << endl;
        cout << "\tMerchandise Subtotal: Php " << merchandiseSubtotal << endl;
        cout << "\tShipping Subtotal: Php " << shippingSubtotal << endl;
        cout << "\tTotal Payment: Php " << totalPayment << endl;
    }

    Order checkoutItems() {
	    Order order;
	
	    for (int i = 0; i < cartSize; ++i) {
	        order.addItemToOrder(cartItems[i], quantities[i]);
	    }
	
	    char paymentOption;
	    cout << "\n\tMode of Payment\n";
	    cout << "\t\tA - Cash on Delivery\n";
	    cout << "\t\tB - Payment Center / E-Wallet\n";
	    cout << "\t\tC - Credit / Debit Card\n";
	    cout << "\t\tD - Online Banking\n";
	    cout << "\t\tPreferred Payment Option: ";
	    cin >> paymentOption;
	    paymentOption = toupper(paymentOption);
	
	    if (paymentOption == 'A' || paymentOption == 'B' || paymentOption == 'C' || paymentOption == 'D') {
	        char placeOrder;
	        while (true) {
	            cout << "\n\tPlace Order [Y/N]? ";
	            cin >> placeOrder;
	            placeOrder = toupper(placeOrder);
	
	            if (placeOrder == 'Y' || placeOrder == 'N') {
	                if (placeOrder == 'Y') {
	                    order.calculateTotalPayment();
	                    cout << "\n\tYou have successfully placed your order!" << endl;
	                    setOrderPlaced(true);
	                    lastOrder = order;
	                    cartSize = 0;
	                    totalQuantity = 0;
	                    totalAmount = 0.0;
	                    merchandiseSubtotal = 0.0;
	                    shippingSubtotal = 0;
	                    totalPayment = 0.0;
	                } else {
	                    cout << "\n\tOrder not placed." << endl;
	                }
	                break; // Exit the loop after valid input
	            } else {
	                cout << "\t\tInvalid input. Please enter 'Y' or 'N'.\n" << endl;
	            }
	        }
	    } else {
	        cout << "\n\tInvalid payment option selected." << endl;
	    }
	
	    order.setTotalAmount(totalPayment);
	    setOrderPlaced(true);
	
	    return order;
	}

    bool isOrderPlaced() const {
        return orderPlaced;
    }

    Order getLastOrder() const {
        return lastOrder;
    }
};

void createProductList(Product productList[], int& size) {
    // Define products
    productList[0] = Product("Cutlery", "CT1", "Dinner Spoon", 35, 50);
    productList[1] = Product("Cutlery", "CT2", "Dinner Fork", 35, 50);
    productList[2] = Product("Cutlery", "CT3", "Dinner Knife", 40, 50);
    productList[3] = Product("Cutlery", "CT4", "Salad Fork", 65, 50);
    productList[4] = Product("Cutlery", "CT5", "Soup Spoon", 40, 50);
    productList[5] = Product("Cutlery", "CT6", "Dessert Spoon", 35, 50);
    productList[6] = Product("Cutlery", "CT7", "Dessert Fork", 40, 50);
    productList[7] = Product("Utensils", "UT1", "Spatula", 125, 20);
    productList[8] = Product("Utensils", "UT2", "Whisk", 150, 30);
    productList[9] = Product("Utensils", "UT3", "Solid Spoon", 125, 25);
    productList[10] = Product("Utensils", "UT4", "Solid Turner", 150, 40);
    productList[11] = Product("Utensils", "UT5", "Soup Ladle", 125, 40);
    productList[12] = Product("Utensils", "UT6", "Peeler", 100, 30);
    productList[13] = Product("Utensils", "UT7", "Grater", 125, 20);
    productList[14] = Product("Utensils", "UT8", "Tong", 175, 25);
    productList[15] = Product("Utensils", "UT9", "Measuring Spoon and Cups", 200, 20);
    productList[16] = Product("Cookware", "CK1", "Frying Pan", 950, 30);
    productList[17] = Product("Cookware", "CK2", "Roasting Pan", 1250, 20);
    productList[18] = Product("Cookware", "CK3", "Sit-Fry Pan", 950, 25);
    productList[19] = Product("Cookware", "CK4", "Sauce Pan", 800, 10);
    productList[20] = Product("Cookware", "CK5", "Saute Pan", 1000, 15);

    size = 21; // Number of products
}

void displayMenu() {
    cout << "\n================================================== SHOP NAME ======================================================\n";
    cout << "\n\tWelcome to our online store!\n\n";
    cout << "===================================================================================================================\n";
    cout << "\n\t\t\t\tMENU\n";
    cout << "\t1 - View Products\n";
    cout << "\t2 - View Shopping Cart\n";
    cout << "\t3 - View Orders\n";
    cout << "\t4 - Exit\n";
    cout << "\n\tPlease select an option to get started (1-4): ";
}

int main() {  
	// Initialize order IDs
    Order::initializeOrderID(0);
	  
    Product productList[1000]; // Array to hold products
    int productSize; // To hold the number of products

    createProductList(productList, productSize);  // Fill the array and set size

    ShoppingCart cart;  // Shopping cart to hold selected products
    
    int choice;
    do {
        displayMenu();  // Display the shopping menu
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\n= VIEW PRODUCTS ===================================================================================================\n\n";
                productList[0].displayItems(productList, productSize);
                cart.addItem(productList, productSize);  // Add items to the cart
                break;

            case 2:
                cout << "\n= VIEW SHOPPING CART ==============================================================================================\n\n";
                cart.displayCart();  // Display items in the cart

                char checkOut;
                cout << "\n\tDo you want to check out all the products [Y/N]? ";
                cin >> checkOut;
                checkOut = toupper(checkOut); // Convert to uppercase

                if (checkOut == 'Y') {
                    cart.checkoutItems();  // Checkout and create the order
                } 
                break;

            case 3:
                cout << "\n= VIEW ORDERS =====================================================================================================\n\n";
                if (cart.isOrderPlaced()) {  // Check if an order has been placed
                    Order order = cart.getLastOrder();  // Retrieve the last order
                    cout << "\tOrder ID: " << order.getOrderID() << endl;  // Display Order ID
                    order.displayOrderSummary();  // Display the order summary (product id, name, price, quantity)
                } else {
                    cout << "\n\tNo order has been placed yet." << endl;  // Handle case when no order exists
                }
                break;

            case 4:
                cout << "\tThank you for shopping with us! Goodbye!\n";
                break;

            default:
                cout << "\tInvalid option. Please select a valid menu option (1-4).\n";
                break;
        }
    } while (choice != 4);

    return 0;
}