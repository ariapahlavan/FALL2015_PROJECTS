//ExpTree.h
//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#ifndef _ExpTree_h
#define _ExpTree_h 1
#include <stdlib.h>
#include <stdio.h>
#include "String.h"


class Statements;
struct ExpressionVec;

class VarTable {
private:
	struct VariableNode
	{
		String varName;
		int value;
		VariableNode* left;// left and right are pointers to our two children. 
		VariableNode* right;// either or both of left/right can be null (i.e., equal to zero)
		VariableNode* parent;// the parent pointer points to our parent. This pointer turns out
							 // to be useful when we want to start traversing a tree (without recursion)
							 // for now, I'll ignore this component
		VariableNode() { //constructor
			value = 0;
			left =  right = parent = NULL;
			varName = "";
		}
	};

	
	int len;
	String nameOfVariable;
	VariableNode* varRoot;
	/*-----------------private member functions--------------*/
	void dstryVarTbl(VariableNode* root);
	bool isMember(VariableNode * root, String & var);
	int& find(VariableNode* root, String& var);


public:
	/*-----------Constructors and Deconstructors------------*/
	//constructors
	VarTable(void) {
		varRoot = NULL;
		len = 0;
	}

	//deconstructors
	~VarTable(void) { dstryVarTbl(varRoot); }
	/*-----------Setters and Getters------------*/
	//setters
	inline void setVarName(String s) { nameOfVariable = s; }
	inline void setvar(int l) { len = l; }
	inline void setvarRoot(VariableNode* var) { varRoot = var; }

	//getters
	inline String getVarName() { return nameOfVariable; }
	inline int getvar() { return len; }
	inline VariableNode* getvarRoot() { return varRoot; }


	//useful functions
	void insert(String & var);
	int& get_var_value(String& v);
	bool isEmpty() const { return varRoot == 0; }
	/*
	//shorter version of isMember
	*/
	bool Check_var(String& v) {
		if (this == 0) { return false; }
		return isMember(varRoot, v);
	}
};



enum Command {
	NUM,
	VAR_NAME,
	SYMB,
	FUNC,
	CALL,
	NOCMD
};

enum BiOp {
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	AND,
	OR,
	COMP,
	UM,
	NEG,
	NOP
};

enum Comp {
	GT,
	LT,
	EQ,
	NE,
	LE,
	GE,
	NONE
};



class Expression {
private:
	struct ExpNode {
		ExpNode* left;
		ExpNode* right;
		ExpNode* parent;
		BiOp op;
		Command cmd;
		Comp comp;
		int value;
		String word;
		ExpressionVec* args; //a vector containing pointers to ExpTrees
		ExpNode() { //constructor
			value = 0;
			op = NOP;
			cmd = NOCMD;
			comp = NONE;
			left = NULL;
			right = NULL;
			parent = NULL;
			word = String("");
		}
	};

	//member variables
	ExpNode* expression_root;
	int length;
	//bool isComparison;		<------- useless 
	//Expression*  child_list_ptr;
	//int num_children;
	ExpNode* insert(ExpNode* root, String& w);
	BiOp operation_type(String & s);
	Comp comp_type(String& s);
	int processComp(Comp c, int x, int y);
	int execExp(ExpNode * r, Statements* st, VarTable & locVar);


public:
	//constructors
	Expression() {
		expression_root = nullptr;
		length = 0;
		//child_list_ptr = NULL;
		//num_children = 0;
	}
	/*Expression(ExpNode* ExpNode) {
	expression_root = ExpNode;
	length = 0;
	//child_list_ptr = NULL;
	//num_children = 0;
	}
	*/
	//deconstructors
	~Expression() {
		//destroy(); 
	}
	int shortExecExp(Statements * st, VarTable& locVar);

	//member functions
	/*void destroy() {
	delete this->expression_root; //destroy the expression
	}*/

	void shortInsert(String& b);


	//void dstryExpTree(Expression * root); no need, who cares about the memory leak
	//ExpNode * insert_string(ExpNode* root, String w);
	//Expression * insert_command(ExpressionVec * e, Expression * c);
	//Expression* Expression::creat_cmd(String w);
	//Expression * insert_number(Expression* root, int v);


};

struct ExpressionVec {
	Expression* data; // an array of statements (allocated on the heap)
	int cap; // size of the array on the heap
	int len; // number of valid statements actually in the array


	ExpressionVec(void) { // done, please do not edit
		this->cap = 0;
		this->data = nullptr;
		this->len = 0;
	}

	void copy(const ExpressionVec& v) {
		this->len = v.len;
		this->cap = v.len;
		if (v.len == 0) { this->data = nullptr; }
		else {
			this->data = new Expression[this->cap];
		}
		for (uint32_t k = 0; k < v.len; k += 1) {
			this->data[k] = v.data[k];
		}
	}

	ExpressionVec(const ExpressionVec& that) {
		copy(that);
	}

	void destroy(void) {
		delete[] this->data;
	}

	int size(void) {
		return this->len;
	}

	ExpressionVec& operator=(const ExpressionVec& that) {
		if (this != &that) {
			destroy();
			copy(that);
		}
		return *this;
	}


	~ExpressionVec(void) { // done, please do not edit
		destroy();
	}

	/*
	void ExpressionVec::clear(void) { // done, please do not edit
	delete[] this->data;
	this->cap = default_cap;
	this->data = new Expression[this->cap];
	this->len = 0;
	}*/

	Expression& operator[](int k) { // done, please do not edit
		return this->data[k];
	}

	//
	//bool ExpressionVec::isMember(String name) { // not done, your effort goes here
	//	assert(name != NULL); //make sure the name is not empty
	//	for (int i = 0; i < this->len; i++)
	//	{
	//		if (name == this->data[i].getword()) { return true; } // if found the name, return true
	//	}
	//	//still no match, return false
	//	return false;
	//}
	//


};
/*-----------------function class--------------------*/


class FunctionExp {
private:
	struct FunctionNode {
		String funTxt;
		FunctionNode* left;
		FunctionNode* right;
		FunctionNode* parent;
		String param;
		int index;
		FunctionNode() { //constructor
			funTxt = String(" ");
			index = 0;
			param = String(" ");
			left = nullptr;
			right = nullptr;
			parent = nullptr;
		}
	};
	int length;

	int capacity;
	FunctionNode* root;
	/*
	//find the start of function and return the index
	*/
	int findFunc(String& name, String& param, FunctionNode* root) {
		
		if (root->funTxt == name) {
			param = root->param;
			return root->index;
		}
		if (name < root->funTxt) {
			return findFunc(name, param, root->left);
		}
		else {
			return findFunc(name, param, root->right);
		}
	}

public:
	/*----------------Constructors adn Deconstructor-------------------------*/
	FunctionExp() {
		root = nullptr;
		capacity = 0;
		length = 0;
	}

	~FunctionExp() {
		destroy(root);
	}

	/*----------------------Member Functions-----------------------------------*/
	/*
	//destroy the function node called  by the deconstructor
	*/
	void destroy(FunctionNode* root) {
		if (!root) { return; }
		destroy(root->left);
		destroy(root->right);
		delete root;
	}
	
	/*
	//insert a function into te vector
	*/
	void insert(int vectorLocation, String& funTxt, String& param) {
		FunctionNode* child = new FunctionNode;
		FunctionNode* parent = 0;
		child->funTxt = funTxt;
		child->index = vectorLocation;
		child->param = param;
		length += 1;
		if (root == 0) {
			root = child;
			return;
		}
		FunctionNode* temp = root;
		while (temp) {
			parent = temp;
			if (funTxt < temp->funTxt) {
				temp = temp->left;
			}
			else if (funTxt > temp->funTxt) {
				temp = temp->right;
			}
		}
		if (funTxt < parent->funTxt) {
			parent->left = child;
		}
		else { parent->right = child; }
	}
	
	//shorter version of findfunc();
	int shortFind(String& n, String& p) {
		return findFunc(n, p, this->root);
	}
};

////template <typename T>
//BiOp operation_type(String s, bool* is_Comparison);
//Command command_type(String s);
//bool isTextString(Expression* s);


#endif /* _Structures_h */
