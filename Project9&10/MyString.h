//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#ifndef _MYSTRING_h
#define _MYSTRING_h
#include <stdlib.h>
#include <stdint.h>
#include "String.h"



struct StringVec {
	uint32_t length;
	uint32_t capacity;
	String* data;

	StringVec(void) {
		length = 0;
		capacity = 0;
		data = nullptr;
	}

	void copy(const StringVec& x) {
		this->length = x.length;
		this->capacity = x.length;
		if (x.length == 0) { this->data = nullptr; }
		else {
			this->data = new String[this->capacity];
		}
		for (uint32_t k = 0; k < x.length; k += 1) {
			this->data[k] = x.data[k];
		}
	}


	void destroy(void) {
		delete[] this->data;
	}

	StringVec(const StringVec& that) {
		copy(that);
	}

	~StringVec(void) {
		destroy();
	}

	StringVec& operator=(const StringVec& that) {
		if (this != &that) {
			destroy();
			copy(that);
		}
		return *this;
	}

	void insert(String* s) {
		//if capacity fulll double it
		if (capacity == length) {
			capacity *= 2;
			if (capacity == 0) { capacity = 1; }
			String* temp = new String[capacity * 2];
			for (uint32_t k = 0; k < length; k += 1) {
				temp[k] = data[k];
			}
			delete[] this->data;
			data = temp;
		}
		this->data[length] = *s;
		length += 1;
	}

};


#endif /* _MYSTRING_h */
