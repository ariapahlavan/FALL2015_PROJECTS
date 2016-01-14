//Project 7
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#include <stdio.h>
#include "String.h"
#include "Customer.h"
#include "CustomerDB.h"
#include <cassert>


void readString(String&);
void readNum(int&);

CustomerDB database;



int num_bottles = 0;
int num_rattles = 0;
int num_diapers = 0;

/* clear the inventory and reset the customer database to empty */
void reset(void) {
	for (int i = 0; i < database.length; i++)
	{
		database.data[i].bottles = 0;
		database.data[i].diapers = 0;
		database.data[i].rattles = 0;
	}
	database.clear();
	num_bottles = 0;
	num_rattles = 0;
	num_diapers = 0;
}

/*
 * selectInventItem is a convenience function that allows you
 * to obtain a pointer to the inventory record using the item type name
 * word must be "Bottles", "Diapers" or "Rattles"
 * for example the expression *selectInventItem("Bottles") returns the 
 * current global variable for the number of bottls in the inventory
 */
int* selectInventItem(String word) {
	if (word == "Bottles") {
		return &num_bottles;
	} else if (word == "Diapers") {
		return &num_diapers;
	} else if (word == "Rattles") {
		return &num_rattles;
	}
	
	/* NOT REACHED */
	return 0;
}

/*
 * this overload of selectInventItem is similar to selectInventItem above, however
 * this overload takes a Customer as a second argument and selects the data member
 * of that Customer that matches "word". The function returns a pointer to one of the three data
 * members from the specified customer.
 */
int* selectInventItem(String word, Customer& cust) {
	if (word == "Bottles") {
		return &cust.bottles;
	} else if (word == "Diapers") {
		return &cust.diapers;
	} else if (word == "Rattles") {
		return &cust.rattles;
	}
	
	/* NOT REACHED */
	return 0;
}



//update customer purchase history
bool custUpdate(String word, Customer& cust, int q) {
	if (word == "Bottles") {
		 cust.bottles += q;
		 return true;
	}
	else if (word == "Diapers") {
		cust.diapers += q;
		return true;
	}
	else if (word == "Rattles") {
		cust.rattles += q;
		return true;
	}
	/* NOT REACHED */
	return false;
}


void updateInventoryPurchase(String item, int num) {
	if (item == "Bottles") {
		num_bottles -= num;
	}
	else if (item == "Diapers") {
		num_diapers -= num;
	}
	else if (item == "Rattles") {
		num_rattles -= num;
	}
}


/*
 * findMax searches through the CustomerDB "database" and returns the Customer
 * who has purchased the most items of the specified type.
 * type must be one of "Bottles", "Rattles" or "Diapers".
 * 
 * Note: if two or more Customers are tied for having purchased the most of that item type
 * then findMax returns the first Customer in the CustomerDB who has purchased that maximal
 * quantity.
 *
 * Note: in the special case (invalid case) where there are zero Customers in the 
 * CustomerDB, fundMax returns a null pointer (0)
 */
Customer* findMax(String type) {
	Customer* result = 0;
	int max = 0;
	for (int k = 0; k < database.size(); k += 1) {
		Customer& cust = database[k];
		int *p = selectInventItem(type, cust); 
		if (*p > max) {
			max = *p;
			result = &cust;
		}
	}
	
	return result;
}

void processPurchase() {
	Customer customer;
	String  custName;
	String item;
	int num = 0;
	readString(custName); // get customer name
	readString(item); // get item type
	readNum(num);	// get quantity 
	//if customer wants to buy 0 quantity of the item just return... what's wrong with these people
	if (num == 0) { return; }
	//Unfortunately out of stock
	if (*selectInventItem(item) < num) {		//Unfortunately out of stock
		printf("Sorry ");		//printing out Sorry custName, we only have qInStock item
		printf("%s", custName.c_str());
		printf(", we only have %d ", *selectInventItem(item));
		printf("%s", item.c_str());
		printf("\n");
	}
	//take care of the purchase adn update the inventory
	else {
		custUpdate(item, database[custName], num);
		updateInventoryPurchase(item, num);
	}
}

void processSummarize() {
	Customer* max_bottles_cust = findMax("Bottles");
	Customer* max_diapers_cust = findMax("Diapers");
	Customer* max_rattles_cust = findMax("Rattles");

	//Start the summarize
	printf("There are %d Bottles, %d Diapers and %d Rattles in inventory\n", num_bottles, num_diapers, num_rattles);
	printf("we have had a total of %d different customers\n", database.length);
	//print out the customer who purchase max num of bottles
	if (max_bottles_cust)
	{
		printf(max_bottles_cust->name.c_str());
		printf(" has purchased the most Bottles (%d)\n", max_bottles_cust->bottles);
	}
	else
	{
		printf("no one has purchased any Bottles\n");
	}
	//print out the customer who purchase max num of diapers
	if (max_diapers_cust)
	{
		printf(max_diapers_cust->name.c_str());
		printf(" has purchased the most Diapers (%d)\n", max_diapers_cust->diapers);
	}
	else
	{
		printf("no one has purchased any Diapers\n");
	}
	//print out the customer who purchase max num of rattles
	if (max_rattles_cust)
	{
		printf(max_rattles_cust->name.c_str());
		printf(" has purchased the most Rattles (%d)\n", max_rattles_cust->rattles);
	}
	else
	{
		printf("no one has purchased any Rattles\n");
	}

}

void processInventory() {
	String invType;
	int invNum = 0;
	readString(invType); //got the inventory type
	readNum(invNum); // got the inventory quantity
	

	if (invType == "Bottles") {
		num_bottles += invNum;		//update inventory
	}
	else if (invType == "Diapers") {
		num_diapers += invNum;		//update inventory
	}
	else if (invType == "Rattles") {
		num_rattles += invNum;		//update inventory
	}
	else {
		printf("I messed up!!!");
		assert(0);		//abort program
	}
}