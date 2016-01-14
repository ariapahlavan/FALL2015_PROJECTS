/* 
 * EE312 Simple CRM Project
 *
 * Aria Pahlavan (EID: ap44342)
 * 09/24/2015
 */

#include <stdio.h>
#include <assert.h>
#include "MyString.h"
#include "Invent.h"
#include "MyString.h"
#include "MemHeap.h"


#define MAX_CUSTOMERS 1000
Customer customers[MAX_CUSTOMERS];
int num_customers = 0;


/* you'll probably need several more global variables */
/* and of course, you have a few functions to write */
//My globals and functions
void updateCust(int , int , int );
int checkInventory(char, int);
int storeReport(char , int );
int typeCheck(String*);
int isOurCust(String* customer);
int* maxPurchase();
Inventory store;
int totCust = 0;



/* clear the inventory and reset the customer database to empty */
void reset(void) {
	
	for (int i = 0; i < totCust; i++)
	{
		customers[i].bottles = 0;		//init bottles
		customers[i].diapers = 0;		//init diapers
		customers[i].rattles = 0;		//init rattles
		customers[i].name.ptr[0] = 0;		//init names
		StringDestroy(&customers[i].name);
	}
	
	store.bottles = store.diapers = store.rattles = store.bottlePurchase = store.diaperPurchase = store.rattlePurchase = 0;
	totCust = 0;		//reinit the total customers
}

void processSummarize() {
	int* ptr = maxPurchase();
	
	printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n", store.bottles, store.diapers, store.rattles);
	printf("we have had a total of %d different customers\n", totCust);
	if (store.bottlePurchase)
	{
		StringPrint(&customers[ptr[1]].name);
		assert(customers[ptr[1]].bottles == ptr[0]);
		printf(" has purchased the most Bottles (%d)\n", ptr[0]);
	}
	else
	{
		printf("no one has purchased any Bottles\n");
	}
	if (store.diaperPurchase)
	{
		StringPrint(&customers[ptr[3]].name);
		assert(customers[ptr[3]].diapers == ptr[2]);
		printf(" has purchased the most Diapers (%d)\n", ptr[2]);
	}
	else
	{
		printf("no one has purchased any Diapers\n");
	}
	if (store.rattlePurchase)
	{
		StringPrint(&customers[ptr[5]].name);
		assert(customers[ptr[5]].rattles == ptr[4]);
		printf(" has purchased the most Rattles (%d)\n", ptr[4]);
	}
	else
	{
		printf("no one has purchased any Rattles\n");
	}


	free(ptr);
}


void processPurchase() {
	String custName;		//customer name
	String item;		//item type
	int itemType;		//item type enum
	int quantity = 0;		//quantity purchased
	readString(&custName);		//get customer name from file
	readString(&item);		//get item type from file
	readNum(&quantity);		//get quantity from file
	itemType = typeCheck(&item);		//return a value indicating the type: 0 = Error, 1 = Bottles, 2 = Diapers, 3 = Rattles
	if (itemType == 0) { assert(0); }		//abort program
  if (quantity == 0) { return; }
	if (checkInventory(itemType, quantity) != -1) {		//if true take care of the purchase (if not eql to -1)
		if (isOurCust(&custName) == -1) {		//if customer is new create an account(i.e. add the new customer into the customers list)
			totCust++;		//increment the totCust by one
			customers[totCust - 1].name = StringDup(&custName);		//copy the new customer into our customers list
			updateCust(totCust, itemType, quantity);		//update purchase history of the customer
		}
		else {		//find the customer in the list and update its purchase hist
			updateCust(isOurCust(&custName)+1, itemType, quantity);		//update purchase history of the customer
		}
	}
	else {		//Unfortunately out of stock
		printf("Sorry ");		//printing out Sorry custName, we only have qInStock item
		StringPrint(&custName);
		printf(", we only have %d ", storeReport(itemType, quantity));
		StringPrint(&item);
		printf("\n");
	}
	StringDestroy(&custName);		//destroy the customer... it has already been copied into the customers list
	StringDestroy(&item);		// destroy the item type... it's just useless
}


void processInventory() {
	int num;		//holds the number of type item
	String item;		//gets the item type
	readString(&item);		//inputs the item type from the file into item string
	readNum(&num);		//inputs the quantity of item from the file into *num 
	switch (item.ptr[0])
	{
	case 'B':		//it's a bottle type
		store.bottles += num;		//update inventory
		break;
	case 'D':		//it's a diaper type
		store.diapers += num;		//update inventory
		break;
	case 'R':		//it's a rattle type
		store.rattles += num;		//update inventory
		break;
	default:
		printf("I messed up!!!");
		assert(0);		//abort program
		break;
	}
	StringDestroy(&item);
	return;

}



//This function checks if the customer name is in the customers' list to see if the customer is a new or already existed
int isOurCust(String* customer){
	for (int i = 0; i < totCust; i++)
	{
		if (StringIsEqualTo(&customers[i].name, customer)) {
			return i;		//if the customer was listed in all customers then return false/old customer
		}
	}
	//if the loop is finished the customer is new
	return -1;
}

//This function reports the quantity of a particular item if it's in stock (if does not exist, returns -1)
int storeReport(char type, int q) {
	int result = -1;
	switch (type)
	{
	case 1:
			result = store.bottles;
		break;
	case 2:
			result = store.diapers;
		break;
	case 3:
			result = store.rattles;
		break;
	default:
		assert(0);		//Error
	}
	return result;
}

//This function checks the inventory to see if we have a particular item in stock or not and return a boolean true or false respectively.
//It also updates the inventory if the purchase is valid and the item is in stock (-1 is false and any other number is quantity in stock)
int checkInventory(char type, int q) {
	int result = -1;
	switch (type)
	{
	case 1:
		if (store.bottles >= q) { //if item in stock send true else false
			store.bottles -= q;		//updating inventory
			store.bottlePurchase = true;		//we had purchase on bottles
			result = store.bottles;
		}		
		else { result = -1; }
		break;
	case 2:
		if (store.diapers >= q) { 
			store.diapers -= q;
			store.diaperPurchase = true;		//we had purchase on diapers
			result = store.diapers;
		}
		else { result = -1; }
		break;
	case 3:
		if (store.rattles >= q) {
			store.rattles -= q;
			store.rattlePurchase = true;		//we had purchase on rattles
			result = store.rattles;
		}
		else { result = -1; }
		break;
	default:
		assert(0);		//Error
	}
	return result;
}


//This fuction updates the purchase histry of a customer
void updateCust(int custIndex, int itemType, int q) {
	switch (itemType)
	{
	case 1:
		customers[custIndex-1].bottles += q;		//update the quantity of item purchased by cust
		break;
	case 2:
		customers[custIndex-1].diapers += q;
		break;
	case 3:
		customers[custIndex-1].rattles += q;
		break;
	default:
		break;
	}
	return;
}


//This function reaturns an enum for the item type: 0 = Error, 1 = Bottles, 2 = Diapers, 3 = Rattles
int typeCheck(String* item) {
	switch (item->ptr[0])
	{
	case 'B':		//it's a bottle type
		if (item->ptr[1] == 'o') {
			return 1;		//return 1
		}
		else { assert(0); }		//item not found
	case 'D':		//it's a diaper type
		if (item->ptr[1] == 'i') {
			return 2;		//return 2
		}
		else { assert(0); }		//item not found
	case 'R':		//it's a rattle type
		if (item->ptr[1] == 'a') {
			return 3;		//return 3
		}
		else { assert(0); }		//item not found
	default:
		return 0;		//Error
	}
}

//This function returns the max purchase on each item and an index to the customer who purchased the max item
int* maxPurchase() {
	int* ptr = (int*)malloc(sizeof(int) * 6);		//allocate 3 integers on heap and set the address to the first one of the int to ptr
	int curr = 0;
	ptr[0] = ptr[2] = ptr[4] = ptr[1] = ptr[3] =  ptr[5] = 0;
	// for bottles
	for (int i = 0; i < totCust; i++)
	{
		curr = customers[i].bottles;
		if (curr > ptr[0]) { ptr[0] = curr; ptr[1] = i; }		//find the person who bought the maximum of bottles the index to the customer (buyer)
	}
	//for diapers
	for (int i = 0; i < totCust; i++)
	{
		curr = customers[i].diapers;
		if (curr > ptr[2]) { ptr[2] = curr; ptr[3] = i;}		//find the person who bought the maximum of diapers the index to the customer (buyer)
	}
	//for Rattles
	for (int i = 0; i < totCust; i++)
	{
		curr = customers[i].rattles;
		if (curr > ptr[4]) { ptr[4] = curr; ptr[5] = i; }		//find the person who bought the maximum of rattles the index to the customer (buyer)
	}
	return ptr;
}
