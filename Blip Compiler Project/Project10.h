//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#ifndef Project10_H
#define Project10_H

#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include "String.h"
#include "ExpTree.h"
#include "Parse.h"
#include "MyString.h"

enum Stmnt {
	STEXP,
	STNUM,
	STVAR,
	//for function
	STFUNC,
	STRET,
	NOST
};

struct StmntNode {
	TokenType tokenType; //got from input.cpp
	String cmdType;		//var,set,output, text
	Stmnt stmntType;
	String text;
	String var;
	Expression* expressions;
	//for functions
	String Param;
	StringVec* MultiPass;
	StmntNode() {
		tokenType = INVALID;
		cmdType = String(" ");
		text = String(" ");
		var = String(" ");
		stmntType = NOST;
		expressions = nullptr;
	}
	~StmntNode() = default;
};

class Statements {
private:
	struct Vector {
		uint32_t length;
		uint32_t capacity;
		StmntNode* data;

		/*-------------------Member Functions-------------------------*/
		//useful member functions
		Vector& operator=(const Vector& that) {
			if (this != &that) {
				destroy();
				copy(that);
			}
			return *this;
		}
		void copy(const Vector& x) {
			this->length = x.length;
			this->capacity = x.length;
			if (x.length == 0) { this->data = nullptr; }
			else {
				this->data = new StmntNode[this->capacity];
			}
			for (uint32_t k = 0; k < x.length; k += 1) {
				this->data[k] = x.data[k];
			}
		}
		void destroy() {
			delete[] this->data;
		}
		//Constructor
		Vector() {
			length = 0;
			capacity = 0;
			data = nullptr;
		}

		Vector(const Vector& that) {
			copy(that);
		}
		//Deconstructor
		~Vector() {
			destroy();
		}

	};

	Vector* instList;
	int returnV;
	int contCaller;		//counting calls to functions in memHeap
	FunctionExp functStore;		//Updated for storing functions

							/*------------------Processing Mem Functions--------------------*/
	StmntNode* prepStmnts();
	void processOut(StmntNode* s, VarTable& locVar);
	void processSet(StmntNode* s, VarTable& locVar);
	void processSymbol(String op);
	int insert(StmntNode* s);
	int processDo(VarTable& locVar, int counter);
	int processIf(VarTable& locVar, int counter);

public:
	VarTable globStore;
	/*------------------Member functions for build, exec, and run statements--------------------*/
	int processFunc(ExpressionVec* vec, VarTable& locVar, String& name);	//adding local and global variables
																			//build, execute and run:
	//void build(void);
	Statements & build();

	void run();

	int execute(VarTable &globVar, VarTable &locVar, int begBlock, int endBlock);
};

#endif
