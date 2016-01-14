//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#include <iostream>
#include <cassert>
#include "String.h"
#include "CustomerDB.h"

using namespace std;

const int default_capacity = 5;

Customer::Customer(String name) { // done, please do not edit
	this->bottles = 0;
	this->rattles = 0;
	this->diapers = 0;
	this->name = name;
}


CustomerDB::CustomerDB(void) { // done, please do not edit
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

int CustomerDB::size(void) {
	return this->length;
}

CustomerDB::~CustomerDB(void) { // done, please do not edit
	delete[] this->data;
}

void CustomerDB::clear(void) { // done, please do not edit
	delete[] this->data;
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

Customer& CustomerDB::operator[](int k) { // done, please do not edit
	assert(k >= 0 && k < this->length);
	return this->data[k];
}

Customer& CustomerDB::operator[](String name) { // not done, your effort goes here
	
	assert(name != "");
	//if customer exists, return refrence to the customer
	for (int i = 0; i < this->length; i++)
	{
		if (name == this->data[i].name) { return this->data[i]; } // if found the name, return the refrence to the customer
	}
	//if no matching customer found, 
	//A )first check the capacity, if not full just add the customer,
	if (this->capacity > this->length) {
		this->data[this->length].name = name;
		this->length++;
	}
	//A')if the capacity is full, then do the amortize doubling then add the customer
	else {
		Customer* temp = new Customer[this->capacity * 2];
		this->capacity *= 2;
		for (int i = 0; i < this->length; i++)
		{
			temp[i] = this->data[i];
		}
		temp[this->length].name = name;
		this->length++;
		delete[] this->data;
		this->data = temp;
	}
  this->data[this->length - 1].diapers = 0;
  this->data[this->length - 1].bottles = 0;
  this->data[this->length - 1].rattles = 0;
  return this->data[this->length - 1];
}




bool CustomerDB::isMember(String name) { // not done, your effort goes here
	assert(name != NULL); //make sure the name is not empty
	for (int i = 0; i < this->length; i++)
	{
		if (name == this->data[i].name)  { return true; } // if found the name, return true
	}
	//still no match, return false
	return false;
}


