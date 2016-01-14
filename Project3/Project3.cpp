//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015


#include <assert.h>
#include <string.h>
#include "MemHeap.h"
#include "String.h"

/* use these two macros and the function functions if you want -- not required */
#define SIGNATURE (~0xdeadbeef)
#define STRING(s) (((String*)s)-1)

/* this simple function can be useful when you implement stage 4
 * you are not required to use the function, and you can implement stage 4 without it */
int32_t isOurs(char* s) {
    if (!isHeapVar(s)) { return 0; }
    if (STRING(s)->check == SIGNATURE) { return 1; }
    else { return 0; }
}

/* allocate a utstring on the heap, initialize the string correctly by copying
 * the characters from 'src' and return a pointer to the first character of actual string data */
char* utstrdup(const char* src) {
	char* _string;
	String* newString = (String*)malloc(sizeof(String) + strlen(src) + 1);		//allocating a String on heap plus the size of the src and 1 for null
	newString->capacity = strlen(src);		//set the ut_capacity to the length of the src
	newString->length = strlen(src);		//set the ut_length to the length of the src
	newString->check = ~0xdeadbeef;		//set the check to ~0xdeadbeef
	for (int i = 0; i <= strlen(src); i++) {		//i<= size of src so that the null char be copied as well
		newString->data[i] = src[i];		//initialize the ut_data correctly by cpying the chars from "src"
	}
	return _string = newString->data;		//return a ptr to the 1st char of ut_data
}

/* the parameter 'utstr' must be a utstring. Find the length of this string by accessing the meta-data
 * and return that length */
uint32_t utstrlen(const char* utstr) {
	uint32_t _length;		//will eql to the length of the ut_string once we'redone
	uint32_t* utptr = (uint32_t*)utstr;		//utptr is eql to utstr
	assert(*(utptr - 3) == ~0xdeadbeef);		//if ut_check is not eql to ~0xdeadbeef ten abort  the program
	String* _string = (String*)(utptr - 3);		//_string points to (utptr - 3) which is the address of ut_check 
	isOurs((char*)utstr);
	return _length = _string->length;		//_length is now eql to ut_length
}

/* s must be a utstring. suffix can be an ordinary string (or a utstring)
 * append the characters from suffix to the string s. Do not allocate any additional storage, and
 * only append as many characters as will actually fit in s. Update the length meta-data for utstring s
 * and then return s */
char* utstrcat(char* s, const char* suffix) {
	assert(*((uint32_t*)s - 3) == ~0xdeadbeef);		//check the ut_check for string s to be eql to signiture
	assert(isOurs(s));
	//uint32_t* utptr = (uint32_t*)s;		//utptr is eql to utstr
	String* _string = (String*)((uint32_t*)s - 3);		//_string points to (utptr - 3) which is the address of ut_check 
	int srclen = strlen(suffix);		//length of the src
	int dstlen = _string->length;		//length of the dst
	assert(dstlen == _string->length);		//check to make sure that the length of the dst string is correct
	
	if (srclen > (_string->capacity - dstlen)) {
		int x = 0;		//suffix counter
		for (int i = dstlen; i < _string->capacity ; i++) {
			_string->data[i] = suffix[x];	//append suffix to ut_data
			x++;		//increment the cntr for suffix
		}
		_string->length = _string->capacity;		//update the dstlen
		_string->data[_string->capacity] = 0;		//set the null
	}
	else {
		int y = 0;		//suffix counter
		for (int i = dstlen; i <= dstlen + srclen; i++) {
			_string->data[i] = suffix[y];		//append the suffix to the ut_data
			y++;		//increment the suffix counter
		}
		_string->length = dstlen + srclen;		//update dstlen
	}
	return s;
}

/* 'dst' must be a utstring. 'src' can be an ordinary string (or a utstring)
 * overwrite the characters in dst with the characters from src. Do not overflow the capacity of dst
 * For example, if src has five characters and dst has capacity for 10, then copy all five characters
 * However, if src has ten characters and dst only has capacity for 8, then copy only the first 8
 * characters. Do not allocate any additional storage, do not change capacity. Update the length
 * meta-data for dst and then return dst */
char* utstrcpy(char* dst, const char* src) {
	assert(*((uint32_t*)dst - 3) == ~0xdeadbeef);		//if ut_chech of dst is not eql to signiture then abort
	assert(isOurs(dst));
	uint32_t srclen, dstlen;
	uint32_t* utptr = (uint32_t*)dst;		//utptr is eql to utstr
	String* _string = (String*)(utptr - 3);		//_string points to (utptr - 3) which is the address of ut_check 
	srclen = strlen(src);		//get the length of src string 
	dstlen = _string->length;	//get the length of dst string
	assert(dstlen == strlen(dst));		//checking to make sure the length of dst is correct
	if (srclen > _string->capacity) {
		for (int i = 0; i < _string->capacity; i++) {
			_string->data[i] = src[i];		//copy chars from src to dst one by one
		}
		_string->data[_string->capacity] = 0;		//set null char
		_string->length = _string->capacity;		//update the dstlen
	}
	else {
		for (int i = 0; i <= srclen; i++) {
			_string->data[i] = src[i];		//copy chars from src to dst one by one
		}
		_string->length = srclen;		//set the length of the dst
	}
	return dst;
}

/* self must be a utstring. deallocate the storage for this string
 * (i.e., locate the start of the chunk and call free to dispose of the chunk, note that the start of
 * the chunk will be 12 bytes before *self) */
void utstrfree(char* self) {
	assert( *( ((uint32_t*)self) -3) == ~0xdeadbeef);		//abort if ut_check is not eql to ~deadbeef
	assert(isOurs(self));
	free( ((uint32_t*)self) - 3 );		//find the address of the first elmnt of strunk on heaf and free it up
	return;
}

/* s must be a utstring.
 * ensure that s has capacity at least as large as 'new_capacity'
 * if s already has capacity equal to or larger than new_capacity, then return s
 * if s has less capacity than new_capacity, then allocate new storage with sufficient space to store
 * new_capacity characters (plus a terminating zero), copy the current characters from s into this
 * new storage. Update the meta-data to correctly describe new new utstring you've created, deallocate s
 * and then return a pointer to the first character in the newly allocated storage */
char* utstrrealloc(char* s, uint32_t new_capacity) {
	assert(*((uint32_t*)s - 3) == ~0xdeadbeef);		//check if ut_check is eql to signature
	assert(isOurs(s));
	char* new_string;
	uint32_t* utptr = (uint32_t*)s;		//utptr is eql to utstr
	String* _string = (String*)(utptr - 3);		//_string points to (utptr - 3) which is the address of ut_check 
	if (_string->capacity >= new_capacity) {
		return s;
	}
	else {
		String* newString = (String*)malloc(sizeof(String) + new_capacity + 1);		//alloc space to store new_capacity
		newString->capacity = new_capacity;		//update the length and capacity in new ut_string
		newString->length = strlen(s);
		newString->check = ~0xdeadbeef;		//update ut_check
		for (int i = 0; i < newString->length; i++) {
			newString->data[i] = s[i];		//copying chars from s to new ut_data
		}
		newString->data[newString->length] = 0;		//add null char
		new_string = newString->data;		//return the new string
		utstrfree(s);		//free up the old ut_string
	}
	
	 
	 return new_string;
}



