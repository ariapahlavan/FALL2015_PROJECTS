//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#include "ExpTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "String.h"
#include "Parse.h"
#include "Project10.h"



#define ERROR 0

/*
//Destroys the whole table!
//should only be used if we're exiting the program or executing a different test file
*/

 void VarTable::dstryVarTbl(VariableNode* root) {

	if (!root) { return; }

	dstryVarTbl(root->left);
	dstryVarTbl(root->right);

	delete root;

}


/*
//insert and initialize the variable doubleo database (or just initialize and warning if it exists already)
//returns a ptr to the var which was inserted and initialized (and if already exists return ptr to that var)
*/

void VarTable::insert(String & var) {
	VariableNode*  temp = varRoot;
	VariableNode* parent = 0;
	VariableNode* child = new VariableNode;
	child->varName = var;
	len += 1;
	//is it a new var table??
	if (isEmpty()) {
		//add a child to the new born Var table
		varRoot = child;
		return;
	}
	///////////////////////////BUG'S HERE///////////////////////////// 
	while (temp) {
		parent = temp;
		//if new less than parent then 
		if (var < temp->varName) {
			//if parent has no left child, then new becomes left child
			temp = temp->left;
		}
		//else if new eql to parent then no need tochange anything just update the value
		else { temp = temp->right; }
	}
	///////////////////////////////NO MORE//////////////////////////////////
	//if parent greater than new then new becomes left child
	if (var < parent->varName) {
		parent->left = child;
	}
	//if parent less than new then new becomes right 
	else { parent->right = child; }

	child->parent = parent;
}

/*
//if root is zero return error
//find the variable node and send the ptr to its location
*/

int& VarTable::find(VariableNode* root, String& var) {
	if (root == 0) {
		printf("\n Could not find variable %s.\n", var.c_str());
		return root->value;
	}
	if (root->varName == var) {
		return root->value;
	}
	if (var > root->varName) {
		return VarTable::find(root->right, var);
	}
	else {
		return VarTable::find(root->left, var);
	}
}

/*
//easy version of search
*/
int& VarTable::get_var_value(String& var) {
	return find(varRoot, var);
}

/*
//Is member?
*/

bool VarTable::isMember(VariableNode* root, String& var) {
	if (root == 0) { return false; }
	if (root->varName == var) {
		return true;
	}
	if (var > root->varName) {
		if (root->right == NULL) {
			return false;
		}
		else { return VarTable::isMember(root->right, var); }
	}
	else {
		if (root->left == NULL) {
			return false;
		}
		else { return VarTable::isMember(root->left, var); }
	}
}

/*
// to see which comparisson operator it is
*/
void insert_command(ExpressionVec* vec, Expression* exp) {
	//if capacity full, int it
	if (vec->cap == vec->len) {
		vec->cap *= 2;
		if (vec->cap == 0) { vec->cap = 1; }
		Expression* temp = new Expression[vec->cap];
		for (int k = 0; k < vec->len; k += 1) {
			temp[k] = vec->data[k];
		}
		delete[] vec->data;
		vec->data = temp;
	}
	vec->data[vec->len] = *exp;
	vec->len += 1;
}

/*
// to see which operator it is (comp/binary and which one)
*/
//template <typename T>
BiOp Expression::operation_type(String& s) {
	if (s == "+") return ADD;
	else if (s == "-") { return SUB; }
	else if (s == "*") { return MUL; }
	else if (s == "/") { return DIV; }
	else if (s == "%") { return MOD; }
	else if (s == "~") { return UM; }
	else if (s == "!") { return NEG; }
	else if (s == "&&") { return AND; }
	else if (s == "||") { return OR; }
	else if (s == "<") { return COMP; }
	else if (s == ">") { return COMP; }
	else if (s == "<=") { return COMP; }
	else if (s == ">=") { return COMP; }
	else if (s == "==") { return COMP; }
	else if (s == "!=") { return COMP; }
	//other illegal operat
	return NOP;
}


/*
// to see which comparisson operator it is
*/
Comp Expression::comp_type(String& s) {
	if (s == ">") { return GT; }
	else if (s == "<") { return LT; }
	else if (s == "<=") { return LE; }
	else if (s == ">=") { return GE; }
	else if (s == "==") { return EQ; }
	else if (s == "!=") { return NE; }
	//other illegal operat
	return NONE;
}



/*
//Process the comparisson operator
*/
int Expression::processComp(Comp c, int x, int y) {
	switch (c)
	{
	case GT:
		return x>y;
		break;
	case LT:
		return x<y;
		break;
	case EQ:
		return x == y;
		break;
	case NE:
		return x != y;
		break;
	case LE:
		return x <= y;
		break;
	case GE:
		return x >= y;
		break;
	case NONE:
		break;
	default:
		break;
	}

	//NOT REACHED
	return 0;
}



/*
//insert the next bubble child which is an operand
*/
//template <typename T>
//ExpNode* Expression::insert(ExpNode* root, String& w) {
Expression::ExpNode* Expression::insert(ExpNode* root, String& w) {

		ExpNode* temp = new ExpNode;
		//add call expression. will have two children. left child is the name of the function and the right child is a vector of the arguments


		if (next_token_type == SYMBOL) {
			temp->cmd = SYMB;
			temp->op = operation_type(w);
			if (temp->op == COMP) {
				temp->comp = comp_type(w);
			}
		}
		else if (next_token_type == NUMBER) {
			temp->cmd = NUM;
			temp->value = token_number_value;
		}
		else if (next_token_type == NAME) {
			if (w == String("call")) {
				temp->cmd = CALL;
			}

			else {
				temp->cmd = VAR_NAME;
				temp->word = w;
			}
		}

		if (temp->cmd == SYMB && temp->left == 0) {
			read_next_token();

		}


		String str = String(next_token());
		if (temp->cmd == CALL) {
			read_next_token();
			temp->word = String(next_token()); //this should be the functions name
			ExpressionVec* newV = new ExpressionVec;
			temp->args = newV;
			read_next_token(); //this should be "args"
			read_next_token(); //this should be the first argument
			str = String(next_token());

			while (str != String("sgra")) {
				Expression* newT = new Expression;
				newT->shortInsert(str);
				insert_command(newV, newT);
				read_next_token(); //this should be the next argument
				str = String(next_token());
			}
		}

		if (temp->cmd == SYMB && temp->left == 0) {
			temp->left = insert(temp->left, str);
		}
		if (temp->cmd == SYMB && temp->op != UM && temp->op != NEG && temp->left != 0) {
			read_next_token();
			String str = String(next_token());
			temp->right = insert(temp->right, str);
		}

		expression_root = temp;


		return temp;


	
}
//
//if (child->cmd == SYMB && child->op != UM && child->op != NEG && child->left != 0) {
//	read_next_token();
//	String str = next_token();
//	child->right = insert(child->right, str);
//}


/*
//short version of insert
*/
void Expression::shortInsert(String& str) {
	Expression::insert(this->expression_root, str);
}


/*
//Destroys the whole table!
//should only be used if we're exiting the program or executing a different test file
*/

//void Expression::dstryExpTree(Expression* root) {
//	if (!root) { return; }
//
//	dstryExpTree(root->getleft());
//	dstryExpTree(root->getright());
//
//	delete root;
//}


/*
//let's execute some of the statements
*/
int Expression::execExp(ExpNode* r, Statements* st, VarTable& locVar) {
	int x = 0, y = 0;

	Command f = r->cmd;
	BiOp b = r->op;
	switch (f)
	{
	case NUM:
		return r->value;
		break;
	case VAR_NAME:
		if (locVar.VarTable::Check_var(r->word)) {
			return locVar.VarTable::get_var_value(r->word);
		}
		else if (st->globStore.VarTable::Check_var(r->word)) {  //check to ensure the variable is in the symbol tree
			return st->globStore.VarTable::get_var_value(r->word);
		}
		else {
			printf("Variable %s not declared ", r->word.c_str());
		}
		break;
	case CALL:
		return st->Statements::processFunc(r->args, locVar, r->word);
		break;
	default:
		break;
	}


	if (r->left) { x = execExp(r->left, st, locVar); }

	if (b != UM && b != NEG && r->cmd != CALL) { 
		//recursively evaluate the right side of the three
		y = execExp(r->right, st, locVar); 
	}
	//when done, evaluate the the evaluated right and left side
	switch (b) {
	case ADD:
		return x + y;
		break;
	case SUB:
		return x - y;
		break;
	case MUL:
		return x*y;
		break;
	case DIV:
		return x / y;
		break;
	case MOD:
		return x % y;
		break;
	case AND:
		return x && y;
		break;
	case OR:
		return x || y;
		break;
	case COMP:
		return processComp(r->comp, x, y);
		break;
	case UM:
		return -x;
		break;
	case NEG:
		if (x != 0) { x = 0; }
		else { x = 1; }
		return x;
		break;
	default:
		printf("\nsomething went wrong! :(\n");
		break;
	}

	//NOT REACHED
	return 0;
	
}

/*
//short version of execExp
*/
int Expression::shortExecExp(Statements* st, VarTable& locVar) {
	return Expression::execExp(this->expression_root, st, locVar);
}


//End of file.


/*
//to see which command it is

Command command_type(String s){
	if (s == "output") return OUTPUT;
	else if (s == "text") return TEXT;
	else if (s == "set") return SET;
	else if (s == "var") return VAR;
	else return NOCMD;
}
*/



