/*
 * Project4.c 
 *
 * Aria Pahlavan	(ap44342)
 * MW 13:30 - 15:00
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way. 
 * Please NOTE, you are responsible for ensuring that the functions work correctly in 
 * your project solution. Just because the function works correctly in my solution does 
 * NOT mean that the function will work correctly in yours. 
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */


/*Binary search*/
bool binarySearch(int *set, int len, int elem){
	int bottom = 0; //left side of the set
	int top = len - 1; //right side of the set

	// search until bottom and top are the same index
	while (bottom <= top) {
		int mid = (bottom + top) / 2; //midpoint

		// Check if the midpoint is equal to our element. If so, we can return the index immediately.
		if (set[mid] == elem) {
			return true;
		}

		// Check whether we should partition the array left or right.
		if (set[mid] < elem) {
			// Take the right half.
			bottom = mid + 1;
		}
		else {
			// Take the left half.
			top = mid - 1;
		}
	}

	// If we reached this point, our left and right boundaries crossed. This will only be the case if the element was not in the array.
	return false;
}
/*swap two elements of a set*/
void swap(int* set, int a, int b) {
	int temp = set[a];
	set[a] = set[b];
	set[b] = temp;
}

/*sort elemets of set*/
int* sortSet(const Set* self) {
	int* set = (int*)malloc(self->len*sizeof(int)); //set now has the same size of the set
	/*copy from set to array*/
	for (int i = 0; i < self->len; i++)
	{
		set[i] = self->elements[i];
	}
	/*bubble sort the array*/
	for (int i = (self->len)-1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (set[j] > set[j + 1]) {
				swap(set, j, j + 1);
			}
		}
	}
	return set; //return the array
}

/* done for you already */
void destroySet(Set* self) {
	free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
	self->len = 0;
	self->elements = 0;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
	self->elements = (int*) malloc(sizeof(int));
	self->elements[0] = x;
	self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
	self->elements = (int*) malloc(other->len * sizeof(int)); //allocate mem for all elements of new set
	for (int k = 0; k < other->len; k += 1) { //start copying elements
		self->elements[k] = other->elements[k]; //setArr is sorted version of other->elemet
	}
	self->len = other->len;	
}

/* done for you already */
void assignSet(Set* self, const Set* other) {
	if (self == other) { return; }
	
	destroySet(self);
	createCopySet(self, other);
}

/* return true if x is an element of self */
bool isMemberSet(const Set* self, int x) {
	int setLen = self->len; //get the length of self
	int* a = self->elements; //get the ptr to set
	/*check if set is empty*/
	if (isEmptySet(self)) {
		return false; //return false if the set is empty since x is not in the set
	}

	int left = 0;
	int right = setLen - 1;

	// Keep going until our left and right cross.
	while (left <= right) {
		// Calculate the midpoint.
		int mid = (left + right) / 2;

		// Check if the midpoint is equal to our element. If so, we can return the index immediately.
		if (a[mid] == x) {
			return true;
		}

		// Check whether we should partition the array left or right.
		if (a[mid] < x) {
			// Take the right half.
			left = mid + 1;
		}
		else {
			// Take the left half.
			right = mid - 1;
		}
	}

	// If we reached this point, our left and right boundaries crossed. This will only be the case if the element was not in the array.
	return false;
}

/*
 * add x as a new member to this set. 
 * If x is already a member, then self should not be changed
 * Be sure to restore the design invariant property that elemnts[] remains sorted
 * (yes, you can assume it is sorted when the function is called, that's what an invariant is all about)
 */
void insertSet(Set* self, int x) {
	int* setArr = (int*)malloc((self->len+1) * sizeof(int)); //allocate space for the new set
	int i = 0;
	bool isInserted = false; //just checking if x has already been inserted into the set
	//if self is empty dont check just insert
	if (isEmptySet(self)) {
		self->len += 1; //increment length
		setArr[0] = x; //insert
	}
	else {
		while (i < (self->len)) {
			if (self->elements[i] == x) { //if any element of the set is eql to x return
				free(setArr);
				return;
			}
			else {
				//if the ith element is less than x then 
				if (self->elements[i] < x) {
					setArr[i] = self->elements[i];
					i++;
				}
				else { //if ith element is greater than x then 
					setArr[i] = x; //ith becomes x and the rest of the elements will be coppied
					isInserted = true;
					for (int j = i; j < self->len; j++)
					{
						setArr[j + 1] = self->elements[j]; //j+1 is because x has been inserted
					}
					self->len += 1; //update the set length
					i = self->len; //to break the while loop
					free(self->elements); //after finihsing setting up the temp set free up the old set
				}
			}
		}
		//if x hasn't already been inserted, then do it
		if (isInserted == false) {
			setArr[self->len] = x; //insert
			self->len++; //increment length
		}
	}	
	self->elements = setArr; //copy the new array to the set
	

}


/*
 * don't forget: it is OK to try to remove an element
 * that is NOT in the set.  
 * If 'x' is not in the set 'self', then
 * removeSet should do nothing (it's not an error)
 * Otherwise, ('x' IS in the set), remove x. Be sure to update self->length
 * It is not necessary (nor recommended) to call malloc -- if removing an element means the 
 * array on the heap is "too big", that's almost certainly OK, and reallocating a smaller array 
 * is almost definitely NOT worth the trouble
 */
void removeSet(Set* self, int x) {
	int i = 0;
	while (i < self->len) {
		if (self->elements[i] != x) {
			i++; //check next element if curr element is not eql to x
		}
		else { //if the ith element is eql to x
			for (int j = i; j < (self->len -1); j++){ //up to len-1 coz one element is being removed
				self->elements[j] = self->elements[j + 1]; //shift elements back one by one
			}
			i = self->len; //break the while loop
			self->len -= 1; //update the length of set
		}
	}
}

/* done for you already */
void displaySet(const Set* self) {
	int k;
	
	printf("{");

	if (self->len == 0) { 
		printf("}"); 
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/* return true if self and other have exactly the same elements */
bool isEqualToSet(const Set* self, const Set* other) {
	if (isEmptySet(self) && isEmptySet(other)) {
		return true; //if both an empty set, return true
	}
	if (self->len != other->len) {
		return false; //return false if the length of the sets are not eql
	}
	for (int i = 0; i < self->len; i++) {
		if (self->elements[i] != other->elements[i]) {
			return false; //if any element is not eql the false
		}
	}
	return true; //if no problem going through all elements then we have two eql sets
}

/* return true if every element of self is also an element of other */
bool isSubsetOf(const Set* self, const Set* other) {
	if (isEmptySet(self)) {
		return true; // if empty is subset to any set
	}
	if (isEqualToSet(self, other)) {
		return true; //if both eql then they're subset of one another
	}
	if (isEmptySet(other)) {
		return false; // if other is empty then self cannot be its subset (we know that self is noth empty)
	}
	if (self->len > other->len) {
		return false; //self cannot be a subset if it has more elements than other
	}
	if (self->len == other->len) { //if the lengths are eql but they are not eql sets, return false
		return false;
	}
	
	int i = 0, j = 0;
	while (i != self->len && j != other->len) {
		//if the elements of curr indeces are eql the increment both indeces
		if (self->elements[i] == other->elements[j]) {
			i++;
			j++;
		}
		else {
			//if element of self is bigger than element of other then increment only the other index
			if (self->elements[i] > other->elements[j]) {
				j++;
			}
			//if element of other is bigger than elemnt of self then self is not a subset of other
			else {
				return false;
			}
		}
	}
	//if  the index of self is not eql to its size then its noself-<t a subset
	if (i != self->len) {
		return false;
	}
	//finished the loop? hurray it's a subset
	return true;
}

/* done for you */
bool isEmptySet(const Set* self) {
	return self->len == 0;
}

/* remove all elements from self that are not also elements of other */
void intersectFromSet(Set* self, const Set* other) {
	int* temp = (int*)malloc(sizeof(int) * self->len); //create a temp arr
	int* tempAddress = temp; //copy of the temp for later reference
	//if both sets are eql just return 
	if (isEqualToSet(self, other)) {
		free(temp); //free up temp
		return;
	}
	//if the second set is empty just return an empty set
	if (isEmptySet(other)) {
		free(self->elements); //freeup old set
		self->elements = temp; //temp is an empty set
		self->len = 0; //empty set
		return;
	}
	//if the first set is empty then return itself
	if (isEmptySet(self)) {
		free(temp); //freeup temp
		return;
	}
	int i = 0, j = 0, newLen = 0; //two indeces for both sets
	//search throguh the sets until we reached the end of either of the sets
	while (i != self->len && j != other->len){
		if (self->elements[i] < other->elements[j]) {
			++i; //if ith el of self is less then move to the next el
		}
		else {
			if (other->elements[j] < self->elements[i]) {
				++j; //if jth el of other is less then move to the next el
			}
			else {
				*temp = self->elements[i]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
				++temp; //increment temp
				++newLen; //increment the length of the intersection set
				++i;  //increment conters
				++j;
			}
		}
	}
	free(self->elements); //free up old set
	self->elements = tempAddress; //update the address of  intersection set
	tempAddress = temp = 0; //destroy the pointer
	self->len = newLen; //update the length of intersection set
}


/* remove all elements from self that are also elements of other */
void subtractFromSet(Set* self, const Set* other) {
	int* temp = (int*)malloc(sizeof(int) * self->len); //create a temp arr
	int* tempAddress = temp; //copy of the temp for later reference
	int i = 0, j = 0, newLen = 0; //two indeces for both sets
								  //search throguh the sets until we reached the end of either of the sets
	//if the first set is empty then retrun itself
	if (isEmptySet(self)) {
		free(temp); //free up temp
		return;
	}
	//if the second set is empty just return the first set
	if (isEmptySet(other)) {
		free(temp); //free up temp
		return;
	}
	while (i != self->len && j != other->len) {
		if (self->elements[i] < other->elements[j]) {
			*temp = self->elements[i]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
			++temp; //increment temp
			++newLen; //increment the length of the intersection set
			++i; //if ith el of self is less then move to the next el
		}
		else {
			if (other->elements[j] < self->elements[i]) {
				++j; //if jth el of other is less then move to the next el
			}
			else {
				++i;  //increment conters
				++j;
			}
		}
	}
	//if we're done with subtracting
	if (i != self->len) {
		while (i != self->len) {
			*temp = self->elements[i]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
			++temp; //increment temp
			++newLen; //increment the length of the intersection set
			++i; //if ith el of self is less then move to the next el
		}
	}
	free(self->elements); //free up old address
	self->elements = tempAddress; //update the address of  intersection set
	tempAddress = temp = 0; //destroy the pointer
	self->len = newLen; //update the length of intersection set
}

/* add all elements of other to self (obviously, without creating duplicate elements) */
void unionInSet(Set* self, const Set* other) {
	int* temp =(int*)malloc(sizeof(int) * (self->len + other->len)); //create a temp arr
	int* tempAddress = temp; //copy of the temp for later reference
	int i = 0, j = 0, newLen = 0; //two indeces for both sets
								  //search throguh the sets until we reached the end of either of the sets
	
	//if the 1st set is empty then return the second set
	if (isEmptySet(self)) {
		//if both are empty just return the first set
		if (isEmptySet(self) && isEmptySet(other)) {
			free(temp); //freeup temporary
			return;
		}
		self->len = other->len;
		//copy elements of other to temp one by one
		for (int i = 0; i < other->len; i++){
			temp[i] = other->elements[i];
		}
		free(self->elements);
		self->elements = temp; //update address of self
		temp = tempAddress = 0;
		return;
	}
	//if the second set is zero just return
	if (isEmptySet(other)) {
		free(temp); //free up temporary coz unused
		return;
	}
	while (i != self->len && j != other->len) {
		if (self->elements[i] < other->elements[j]) {
			*temp = self->elements[i]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
			++temp; //increment temp
			++newLen; //increment the length of the intersection set
			++i; //if ith el of self is less then move to the next el
		}
		else {
			if (other->elements[j] < self->elements[i]) {
				*temp = other->elements[j]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
				++temp; //increment temp
				++newLen; //increment the length of the intersection set
				++j; //if jth el of other is less then move to the next el
			}
			else {
				*temp = self->elements[i]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
				++temp; //increment temp
				++newLen; //increment the length of the intersection set
				++i;  //increment conters
				++j;
			}
		}
	}
	//if either hasn't finished
	if (i != self->len) {
		while (i != self->len) {
			*temp = self->elements[i]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
			++temp; //increment temp
			++newLen; //increment the length of the intersection set
			++i; //if ith el of self is less then move to the next el
		}
	}
	if (j != other->len) {
		while(j != other->len) {
			*temp = other->elements[j]; // if both elements are eql then it gonna be added to the intersection set (i.e. temp)
			++temp; //increment temp
			++newLen; //increment the length of the intersection set
			++j; //if jth el of other is less then move to the next el
		}
	}
	free(self->elements); //free up the old set before setting it eql to the new set
	self->elements = tempAddress; //update the address of  intersection set
	tempAddress = temp = 0; //destroy the pointer
	self->len = newLen; //update the length of intersection set
}