/*
* Project1.cpp
*
* Name: Aria Pahlavan (ap44342)
* EE312 Fall 2015
* SpellCheck
*/

#include <stdio.h> // provides declarations for printf and putchar
#include <stdint.h> // provides declarations for int32_t uint32_t and the other (new) standard C types

/* All of your code must be in this file. Please no #includes other than standard system headers (ie.., stdio.h, stdint.h)
*
* Many students find it helpful to declare global variables (often arrays). You are welcome to use
* globals if you find them helfpul. Global variables are by no means necessary for this project.
*/

/* You must write this function (spellCheck). Do not change the way the function is declared (i.e., it has
* exactly two parameters, each parameter is a standard (mundane) C string (see SpellCheck.pdf).
* You are expected to use reasonable programming style. I *insist* that you indent
* reasonably and consistently in your code. I strongly encourage you to avoid big functions
* So, plan on implementing spellCheck by writing two or three other "support functions" that
* help make the actual spell checking easier for you.
* There are no explicit restictions on using functions from the C standard library. However,
* for this project you should avoid using functionality from the C++ standard libary. You will
* almost certainly find it easiest to just write everything you need from scratch!
*/

char *buildWord(uint16_t stpt, uint16_t wrdsz, char article[]) {		//Here we have the starting point and size of a word that needs to be checked
	static char word[20];
	uint16_t x = 0;
	for (uint16_t i = stpt; i < stpt + wrdsz; i++) {		//creating the word array containing the word only and terminated by '\n'
		word[x] = article[i];
		x++;
	}
	word[x] = '\n';		//word string is terminated by '\n'
	return word;
}

void compareInDictionary(char word[], char dictionary[], uint16_t wrdsz) {		//Here we have the starting point and size of a word that needs to be checked
	uint32_t i = 0, x = 0, match = 1, size = wrdsz;
	while (dictionary[i] != 0) {		//if reached 0 then it's an invalid word
		size = wrdsz;		//make sure reinitialize the word size to the correct value before each word check
		x = 0;		// reinitialize the counter
		match = 1;
		while ((dictionary[i] != '\n' && match == 1 && size != 0)) {		// if reached '\n'  or match flag is off or word size is 0 then move to next word
			if (dictionary[i] == word[x] || (dictionary[i] + 0x20) == word[x] || dictionary[i] - 0x20 == word[x]) {		//if the letters match set the match flag on (keep checking the rest of the letters)
				match = 1;
				i++;		//next letter in dictionary
				x++;		//next letter in the word
				size--;		//decrement the word size
			}
			else {		//if don't match, turn match flag off (move on to the next dictionary word)
				match = 0;
				while (dictionary[i] != '\n') {
					i++;
				}
			}

		}
		if (dictionary[i] == '\n') {		//(1)
			if (match == 1) {		//(2)
				if (size == 0) {
					return;		//if reached the null character, the match flag is still on, and al the letters of the word is checked then it is a valid word
				}
				match = 0;
				while (dictionary[i] != '\n') {
					i++;
				}
				i++;
			}
			else {		//(2)
				i++;		//next letter in dictionary
			}
		}
		else {		//(1)	
			i++;		//next letter in dictionary
		}
	}
	if (match == 0) {
		for (int i = 0; i < wrdsz; i++) {
			printf("%c", word[i]);		//print letter by letter
		}
		printf("\n");
	}

}

void spellCheck(char article[], char dictionary[]) {
	uint16_t i = 0, wrdsz = 0, letter = 0;
	while (article[i] != 0) {			//check the letters of the article until reached the 0
		if ((article[i] >= 0x61 && article[i] <= 0x7A) || (article[i] >= 0x41 && article[i] <= 0x5A)) {  //is the char a letter?
			wrdsz++;		// if so the increment the word size var
			letter = 1;		//set the letter flag on
		}
		else {		// if not letter, set the letter flag off
			letter = 0;
		}
		if (wrdsz >= 2 && letter == 0) {		//if word size is greater than or equal to two and letter flag is off:
			compareInDictionary(buildWord((i - wrdsz), wrdsz, article), dictionary, wrdsz);		//send the starting point of the word and its size to the bldWrd function
																					//then send the word to dictComp to see if it's valid or not
			wrdsz = 0;		//after done checking the dictionary, reinitialize the word size counter
		}
		else {		//if not a complete word then next letter, but
			i++;
			if (letter == 0 && wrdsz << 2) {		//I also need to reinitialize the word size if the word was less than two letters
				wrdsz = 0;
			}
		}
	}
	if (wrdsz >= 2) {		//if word size is greater than or equal to two and letter flag is off:
		compareInDictionary(buildWord((i - wrdsz), wrdsz, article), dictionary, wrdsz);		//send the starting point of the word and its size to the bldWrd function
																				//then send the word to dictComp to see if it's valid or not
		wrdsz = 0;		//after done checking the dictionary, reinitialize the word size counter
	}
}

