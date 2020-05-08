/* Header files */
#include "CItem.h"
#include "CUser.h"
/* Prototypes */
// -- Menu functions
int ShowMenu(char usertype);
// -- Callback functions
void DoInitializePriceList(void);
void DoDisplayFullPriceList(void);
void DoAddItemToList(void);
void DoSetItemPrice(void);
void DoSetItemDiscountRate(void);
void DoDisplayItem(void);
void DoOrderCost(void);
void DoTotalInvoice(void);
void DoRemoveItemFromList(void);
void DoAddUser(void);
void DoEditUser(void);// for changing passwords
// -- Helper function
CUser Login(void);
void LoadUsers(void);
int GetItemPosition(int itemCode);
int GetUserPosition(string userName);
void LoadList(void);
bool ItemExists(int itemCode);
bool UserExists(string userName);
void SaveList(void);
void SaveUsers(void);
void intInputHandler(int &value);
void doubleInputHandler(double &value);
/* Global data */
// -- Arrays (item list, user list)
/*
    I use pointers as my array method, this is so that I can resize the array during runtime.
    seen throughout my code as
    delete[] itemList;
    itemList = { new CItem[itemCount] };
    or
    delete[] userList;
    userList = { new CUser[userCount] };
*/
CItem* itemList;
CUser* userList;
// -- Global user
CUser user;
// -- Global variables
int itemCount, // keey track of the item array size
    itemsAdded, // keey track of each individual items
    userCount, // keey track of the user array size
    userAdded; // used to increade the user count if and only if a user has been added, decrease back to 0 once the user has successfully been added
/* Main program */
int main() 
{
    // -- Loading in the users data
    LoadUsers();
    // -- Log in section
    user = Login();
    if (user.GetName() != "")
    {
        // -- Application
        int option;
        DoInitializePriceList();// -- Inital list
        do {
            option = ShowMenu(user.GetType());
            switch (option)
            {
            case 0:
                break;
            case 1:
                DoInitializePriceList();// -- Use this to make a new list
                break;
            case 2:
                DoDisplayFullPriceList();
                break;
            case 3:
                DoAddItemToList();
                break;
            case 4:
                DoSetItemPrice();
                break;
            case 5:
                DoSetItemDiscountRate();
                break;
            case 6:
                DoDisplayItem();
                break;
            case 7:
                DoOrderCost();
                break;
            case 8:
                DoTotalInvoice();
                break;
            case 9:
                DoRemoveItemFromList();
                break;
            case 10:
                DoAddUser();
                break;
            case 11:
                DoEditUser();
                break;
            default:
                cout << "\nInvalid option.\n";
            };
        } while (option != 0);// once the user enters 0 the while loop ends
        // -- Save out, give the option for the user to save their list
        cout << "Would you like to save the item list?\n\t0 - No\n\t1 - Yes\n\t-> ";
        while (!(cin >> option))
        {
            cin.clear();
            cin.ignore(123, '\n');
            cout << "\n\tEnter a valid option-> ";
        }
        if (option == 1)
            SaveList();
    }
    // -- Login failed message
    else
        cout << "Failed login, access denied.\n";
    // -- Application termination
    return 0;
}
/* Definitions */
// -- Menu functions
int ShowMenu(char usertype)
{
    /*
    process
        0 - display assistant menu, is the parameter taken is 'M' then append the manager menu to the user menu
        1 - error handling, if a non manager inputs a manager option (which is hidden) the program will set the option to an invalid option to prevent non managers from accessing manager features
        2 - clear the console screen
    */
    int option;
    cout << "Item Menu: "
        << "\n\t1 - Initialize Price List"
        << "\n\t2 - Display Full Price List"
        << "\n\t3 - Add Item to List"
        << "\n\t4 - Set Item Price"
        << "\n\t5 - Set Item Discount Rate"
        << "\n\t6 - Display Item"
        << "\n\t7 - Order Cost";
    if (usertype == 'M')
        cout << "\n\t8 - Total Invoice"
        << "\n\t9 - Remove Item from List"
        << "\n\t10 - Add User"
        << "\n\t11 - Edit User";
    cout << "\n\t0 - QUIT\n\t-> ";
    intInputHandler(option);
    if (usertype != 'M' && option > 7)
           option = 12;// invalid option - this is to prevent non managers from accessing manager features
    system("CLS");
    return option;
}
// -- Callback functions
void DoInitializePriceList(void)
{
    /*
    process
        0 - check if a list already exists, if so give the option to start over or to return to the menu keeping the current list
        1 - asks for input for the size of the array (up tp 25), destroy the previous array and create a new array with the new size
        2 - ask for initalization method, keyboard input or load from file, if keyboard input is chosen then ask for item details until the list is either full or the user wants to return to the menu
        3 - create list and return to main menu
    */
    int option = 1;
    bool addItems;
    if (itemsAdded > 0)
    {
        cout << "List alread exists\n\t0 - Go back\n\t1 - Start over\n\t";
        intInputHandler(option);
    }
    if (option == 1)
    {
        cout << "Chose method of initialization:\n\t0 - Keyboard input\n\t1 - Load previous list\n\t-> ";
        intInputHandler(option);
        itemsAdded = 0;
        if (option == 1)
            LoadList();
        else
        {
            cout << "Enter list size (max 25): ";
            intInputHandler(itemCount);
            if (itemCount <= 25)
            {
                delete[] itemList;
                itemList = { new CItem[itemCount] };
                for (int i = 0; i < itemCount; i++)
                    itemList[i] = CItem();
                cout << "Would you like to begin adding items to your list now?\n\t0 - Return to the main menu\n\t1 - Add more items\n\t-> ";
                intInputHandler(option);
                if (option == 1)
                {
                    addItems = true;
                    while (addItems)
                    {
                        cout << "\n\n";
                        DoAddItemToList();
                        cout << "Stop adding items to list?\n\t0 - No\n\t1 - Yes\n\t-> ";
                        intInputHandler(option);
                        if (option == 1)
                            addItems = false;
                    }
                }
                cout << "List initalized, chose option 3 to add items to the list.\n\n";
            }
            else
                cout << "List size too large.\n";
        }
    }
}
void DoDisplayFullPriceList(void)
{
    /*
    process
        0 - displau header
        1 - loop and display each item
    */
    cout << setw(30) << setfill('-') << "Code" << setw(30) << "Description" << setw(30) << "Price" << setw(30) << "Discount\n";
    for (int i = 0; i < itemCount; i++)
        itemList[i].Display();
}
void DoAddItemToList(void)
{
    /*
    process
        0 - take item code and description
        1 - ask if the user wants to input the price and discount rate, if so then take them as input
        2 - check if the item exists, if the item code exists do no allow the item to be added to the array 
        3 - if the item code does not exist already then construct the item and add the item to the array
    */
    int code, choice;
    double price, discountRate;
    string description;
    cout << "Enter item code: ";
    intInputHandler(code);
    cout << "Enter item description: ";
    cin >> description;
    cout << "Would you like to apply a price and discount\n\t0 - No\n\t1 - Yes\n\t->  ";
    intInputHandler(choice);
    if (choice == 1)
    {
        cout << "Enter item price: ";
        doubleInputHandler(price);
        cout << "Enter item discount rate: ";
        doubleInputHandler(discountRate);
    }
    if (ItemExists(code))
        cout << "Item already exists.\n";
    else
    {
        if (itemsAdded < itemCount)
        {
            CItem item(code, description);
            if (choice == 1)
            {
                item.SetPrice(price);
                item.SetDiscountRate(discountRate);
            }
            itemList[itemsAdded] = item;
            itemsAdded++;
        }
        else
            cout << "List is full.  To start a new list enter option 1 and chose Keyboard input (0).\n";
    }
}
void DoSetItemPrice(void)
{
    /*
    process
        0 - take item code and item price
        1 - check if the item code exists
        2 - if the item exists then update the price
        3 - if the item does not exist then output a message giving the user that feedback
    */
    int code;
    double price;
    cout << "Enter Item Code: ";
    intInputHandler(code);
    cout << "Enter Item Price: ";
    doubleInputHandler(price);
    if (ItemExists(code))
        itemList[GetItemPosition(code)].SetPrice(price);
    else
        cout << "Item does not exist.\n";
}
void DoSetItemDiscountRate(void)
{
    /*
    process
        0 - take item code and discount rate
        1 - check if the item code exists
        2 - if the item exists then update the discount rate
        3 - if the item does not exist then output a message giving the user that feedback
    */
    int code;
    double discountRate;
    cout << "Enter Item Code: ";
    intInputHandler(code);
    cout << "Enter Item Discount Rate: ";
    doubleInputHandler(discountRate);
    if (ItemExists(code))
        itemList[GetItemPosition(code)].SetDiscountRate(discountRate);
    else
        cout << "Item does not exist.\n";
}
void DoDisplayItem(void)
{
    /*
    process
        0 - take item code
        1 - check if the item code exists
        2 - if the item exists then output a header along with the item details
        3 - if the item does not exist then output a message giving the user that feedback
    */
    int code;
    cout << "Enter Item Code: ";
    intInputHandler(code);
    if (ItemExists(code))
    {
        cout << setw(30) << setfill('-') << "Code" << setw(30) << "Description" << setw(30) << "Price" << setw(30) << "Discount\n";
        itemList[GetItemPosition(code)].Display();
    }
    else
        cout << "Item does not exist.\n";
}
void DoOrderCost(void)
{
    /*
    process
        0 - take item code and quantity
        1 - check if the item code exists
        2 - if the item exists then check if the item has a discount, if the item has a discount then ask the user if the want to apply it and only apply if the user wants to, output order cost
        3 - if the item does not exist then output a message giving the user that feedback
    */
    int code, quantity, option;
    cout << "Enter Item Code: ";
    intInputHandler(code);
    cout << "Enter Quantity: ";
    intInputHandler(quantity);
    if (ItemExists(code))
    {
        if (itemList[GetItemPosition(code)].HasDiscount())
        {
            cout << "There is a discount available, would you like to use it?\n\t0 - No\n\t1 - Yes\n\t->";
            intInputHandler(option);
            cout << setw(30) << "You ordered item code " << code << " with a quantity of " << quantity << "\n";
            if (option == 1)
                cout << setw(30) << "Total order cost: " << itemList[GetItemPosition(code)].GetDiscount() * quantity << "\n";
            else
                cout << setw(30) << "Total order cost: " << itemList[GetItemPosition(code)].GetPrice() * quantity << "\n";
        }
        else
        {
            cout << setw(30) << "You ordered item code " << code << " with a quantity of " << quantity << "\n";
            cout << setw(30) << "Total order cost: " << itemList[GetItemPosition(code)].GetPrice() * quantity << " EURO\n";
        }
    }
    else
        cout << "\n\tItem does not exist.\n";
}
void DoTotalInvoice(void)
{
    /*
    process
        0 - display header
        1 - loop through each item in the array, display the item, update the total cost (apply a discount if it exists)
        2 - output the total cost
    */
    double totalCost = 0;
    cout << setw(30) << setfill('-') << "Code" << setw(30) << "Description" << setw(30) << "Price" << setw(30) << "Discount\n";
    for (int i = 0; i < itemsAdded; i++)
    {
        cout << setw(30) << setfill('-') << itemList[i].GetCode() << setw(30) << itemList[i].GetDescription() << setw(30) << itemList[i].GetPrice() << setw(30) << itemList[i].GetDiscountRate() << "\n";
        if (itemList[i].HasDiscount())
            totalCost += itemList[i].GetDiscount();
        else
            totalCost += itemList[i].GetPrice();
    }
    cout << setw(30) << "Total cost " << totalCost << " EURO\n";
}
void DoRemoveItemFromList(void)
{
    /*
    process
        0 - take item code
        1 - check if the item exists, if the item exists then delete the item
        2 - if the item does not exist then output a message giving the user that feedback
    */
    int code;
    cout << "Enter Item Code: ";
    intInputHandler(code);
    if (ItemExists(code))
        itemList[GetItemPosition(code)].Delete();
    else
        cout << "Item does not exist.\n";
}
void DoAddUser(void)
{
    /*
    process
        0 - take username
        1 - check if the user exists
        2 - if the user does not exist then take user details and add the user to the array
        3 - if the user does not exist then output feedback to the user letting them know
    */
    string username, password;
    int choice;
    char usertype;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    cout << "Enter usertype\n\t0 - Assistant\n\t1 - Manager\n\t-> ";
    intInputHandler(choice);
    if (choice == 1)
        usertype = 'M';
    else
        usertype = 'A';
    if (UserExists(username))
        cout << "User already exists\n";
    else
    {
        // -- increment the userCount by 1
        userAdded += 1;
        // -- save out the users with the new user count
        SaveUsers();
        // -- Append the user data file with the new user
        ofstream outfile("./Users.dat", ios::app);
        if (outfile.is_open())
        {

            outfile << username << " " << password << " " << usertype << "\n";
        }
        outfile.close();
    }
    // -- reload the array with the new size
    LoadUsers();
}
void DoEditUser(void)
{
    /*
    process
        0 - take username and password
        1 - check if the user exists
        2 - if the user exists then update the user password
        3 - if the user does not exist then output feedback to the user letting them know
    */
    string username, password;
    // -- User input
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
    // -- check if the user exists
    if (UserExists(username))
    {
        userList[GetUserPosition(username)].SetPassword(password);// -- Change the password
        SaveUsers();// -- Save out
        LoadUsers();// -- Reload users with new data
    }
    else
        cout << "User does not exist.\n";
}
// -- Helper function
CUser Login(void)
{
    /*
    process
        0 - take username and password
        1 - check if the user exists
        2 - if the user exists and the input credentials are validated then login as that user
        3 - 3 login attempts are given, if the user has login attempts left then give the option to reattempt, after the attempts are all gone then deny access
    */
    string username, password;
    for (int i = 1; i < 4; i++)
    {
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;
        if (userList[GetUserPosition(username)].passwordCheck(password))
        {
            CUser loginUser(username, password, userList[GetUserPosition(username)].GetType());
            return loginUser;
        }
        cout << "Attempt " << i << " of 3\n\n";
    }
    return CUser();// if a failed login attempt has occured past the allowed limit, return a null CUser which will give no further access to the application
}
void LoadUsers(void)
{
    /*
    process
        0 - open the users file in read mode
        1 - delete the previous users array
        2 - apply the user count (from line 1) to the userCount variable
        3 - point the userList to a new array with the userCount as the array size
        4 - contruct a user object with the details from a give line, add the user object to the array
        5 - if the file failed to open then give the user feedback
    */
    string username, password;
    char usertype;
    ifstream infile("./Users.dat");
    delete[] userList;
    infile >> userCount;
    if (infile.is_open())
    {
    userList = { new CUser[userCount] };
    for (int i = 0; i < userCount; i++)
    {
        infile >> username >> password >> usertype;
        userList[i] = CUser(username, password, usertype);
    }
    }
    else
        cout << "Could not access file\n";
    infile.close();
}
int GetItemPosition(int itemCode)
{
    /*
    process
        0 - loop through the array
        1 - if any given object within the array shares the given parameter item code then return the variable i (which represents the array position)
        2 - if no object shares the code then return an invalid array position
    */
    for (int i = 0; i < itemCount; i++)
        if (itemList[i].HasCode(itemCode))
            return i;
    return -1;// -- if the item doesnt exist return a number that is always outside the array
}
int GetUserPosition(string userName)
{
    /*
    process
        0 - loop through the array
        1 - if any given object within the array shares the given username parameter then return the variable i (which represents the array position)
        2 - if no object shares the user name then return an invalid array position
    */
    for (int i = 0; i < userCount; i++)
        if (userList[i].HasUser(userName))
            return i;
    return -1;// -- if the user doesnt exist return a number that is always outside the array
}
void LoadList(void)
{
    /*
    process
        0 - open the list file in read mode
        1 - delete the previous list array
        2 - apply the item count (from line 1) to the itemCount variable
        3 - point the itemList to a new array with the itemCount as the array size
        4 - contruct an item object with the details from a give line, add the item object to the array
        5 - if the file failed to open then give the user feedback
    */
    int code, oldItemCount;
    double price, discount;
    string description;
    ifstream infile("./List.dat");
    delete[] itemList;
    infile >> oldItemCount;
    cout << "Enter list size (max 25): ";
    intInputHandler(itemCount);
    if (itemCount <= 25)
    {
        itemList = { new CItem[itemCount] };
        for (int i = 0; i < itemCount; i++)
        {
            if (i < oldItemCount)
            {
                infile >> code >> description >> price >> discount;
                itemList[i] = CItem(code, description, price, discount);
                itemsAdded++;
            }
            else
                itemList[i] = CItem();
        }
    }
    else
        cout << "List size entered is too large.\n";
    infile.close();
}
bool ItemExists(int itemCode)
{
    /*
    process
        0 - loop through the array
        1 - if any given object within the array shares the given item code parameter then return true
        2 - if no object shares the given parameter then return false
    */
    for (int i = 0; i < itemCount; i++)
        if (itemList[i].HasCode(itemCode))
            return true;
    return false;
}
bool UserExists(string userName)
{
    /*
    process
        0 - loop through the array
        1 - if any given object within the array shares the given user name parameter then return true
        2 - if no object shares the given parameter then return false
    */
    for (int i = 0; i < userCount; i++)
        if (userList[i].HasUser(userName))
            return true;
    return false;
}
void SaveList(void)
{
    /*
    process
        0 - open the list file in write mode
        1 - output the itemsAdded variable to the first line
        2 - loop through the array saving each object to each line
        3 - close the file
    */
    ofstream outfile("./List.dat");
    if (outfile.is_open())
    {
        // saving list size
        outfile << itemsAdded << "\n";
        // saving list items
        for (int i = 0; i < itemsAdded; i++)
            outfile << itemList[i].GetCode() << " " << itemList[i].GetDescription() << " " << itemList[i].GetPrice() << " " << itemList[i].GetDiscountRate() << "\n";
    }
    outfile.close();
}
void SaveUsers(void)
{
    /*
    process
        0 - open the users file in write mode
        1 - output userCount + userAdded to the first line
        2 - loop through the array saving each object to each line
        3 - close the file
    */
    ofstream outfile("./Users.dat");
    // saving list size
    if (outfile.is_open())
    {
        outfile << userCount + userAdded << "\n";
        // saving list items
        for (int i = 0; i < userCount; i++)
             outfile << userList[i].SaveUser();
    }
    else
        cout << "Could not access file.\n";
    userAdded = 0;// -- RESTS THE USER ADDED VARIABLES
    outfile.close();
}
void intInputHandler(int& value)
{
    /*
    process
        0 - take a variable address as a parameter (pass by reference)
        1 - perform a while loop, continue this while loop until a valid datatype has been inputted
    */
    while (!(cin >> value))
    {
        cin.clear();
        cin.ignore(123, '\n');
        cout << "\n\tInvalid input, enter an integer\n\t-> ";
    }
}
void doubleInputHandler(double& value)
{
    /*
    process
        0 - take a variable address as a parameter (pass by reference)
        1 - perform a while loop, continue this while loop until a valid datatype has been inputted
    */
    while (!(cin >> value))
    {
        cin.clear();
        cin.ignore(123, '\n');
        cout << "\n\tInvalid input, enter a decimal\n\t-> ";
    }
}