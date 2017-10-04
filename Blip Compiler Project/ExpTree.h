//ExpTree.h
//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#ifndef _ExpTree_h
#define _ExpTree_h

#include <cstdlib>
#include <cstdio>
#include "String.h"


class Statements;
struct ExpressionVec;

class VarTable {
private:
	struct VariableNode
	{
		String varName;
		int value;
		VariableNode* left;
		VariableNode* right;

		VariableNode() { //constructor
			value = 0;
			left =  right = nullptr;
			varName = "";
		}
	};

	
	int len;
    VariableNode* varRoot;
	/*-----------------private member functions--------------*/
	void dstryVarTbl(VariableNode* root);
	bool isMember(VariableNode * root, String & var);
	int& find(VariableNode* root, String& var);


public:
	/*-----------Constructors and Deconstructors------------*/
	//constructors
	VarTable() {
		varRoot = nullptr;
		len = 0;
	}

	//deconstructors
	~VarTable() { dstryVarTbl(varRoot); }


    //useful functions
	void insert(String & var);
	int& get_var_value(String& v);
	bool isEmpty() const { return varRoot == nullptr; }
	/*
	//shorter version of isMember
	*/
	bool Check_var(String& v) {
		return this != nullptr && isMember(varRoot, v);
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
			left = nullptr;
			right = nullptr;
            word = String("");
		}
	};

	ExpNode* expression_root;
    Expression::ExpNode *insert(String &w);
	BiOp operation_type(String & s);
	Comp comp_type(String& s);
	int processComp(Comp c, int x, int y);
	int execExp(ExpNode * r, Statements* st, VarTable & locVar);


public:
	//constructors
	Expression() {
		expression_root = nullptr;
	}

    ~Expression() = default;

    int shortExecExp(Statements * st, VarTable& locVar);


	void shortInsert(String& b);
};

struct ExpressionVec {
	Expression* data; // an array of statements (allocated on the heap)
	int cap; // size of the array on the heap
	int len; // number of valid statements actually in the array


	ExpressionVec() {
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

	void destroy() {
		delete[] this->data;
	}

    ExpressionVec& operator=(const ExpressionVec& that) {
		if (this != &that) {
			destroy();
			copy(that);
		}
		return *this;
	}

	~ExpressionVec() {
		destroy();
	}

	Expression& operator[](int k) {
		return this->data[k];
	}
};
/*-----------------function class--------------------*/


class FunctionExp {
private:
	struct FunctionNode {
		String funTxt;
		FunctionNode* left;
		FunctionNode* right;
        String param;
		int index;
		FunctionNode() { //constructor
			funTxt = String(" ");
			index = 0;
			param = String(" ");
			left = nullptr;
			right = nullptr;
        }
	};
	int length;

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
	void insert(int vectorLocation, String& funTxt, String& param);
	
	//shorter version of findfunc
	int shortFind(String& n, String& p) {
		return findFunc(n, p, this->root);
	}
};

#endif /* _Structures_h */
