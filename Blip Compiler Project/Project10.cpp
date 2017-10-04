//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#include <cstdlib>
#include <cstdio>
#include "Parse.h"
#include "Project10.h"


/*
//This function is used to parse instructions and process separately
*/
StmntNode* Statements::prepStmnts(){
	read_next_token();
	//are we done?
	if (next_token_type == END) {
		return nullptr;
	}
    auto * stChild = new StmntNode;
	if (next_token_type != SYMBOL && next_token_type != NUMBER && next_token_type != NAME) { abort(); }
	stChild->tokenType = next_token_type; 
	String str = next_token();


	if (stChild->tokenType == SYMBOL) {

		stChild->text = str;
		if (str == String("//")) { skip_line(); }

		return stChild;
	}

	stChild->cmdType = str;

	if (stChild->cmdType == String("text")) {
		read_next_token();
		str = next_token();
		stChild->text = str;
		return stChild;
	}
	else if (stChild->cmdType == String("output")) {
		read_next_token();
		str = next_token();
		//if nothing after that then it's and easy output of a var or const without any expression
		if (next_token_type == SYMBOL || next_token_type == NUMBER || next_token_type == NAME) {
			stChild->stmntType = STEXP;
            auto * newExpression = new Expression;
			newExpression->Expression::shortInsert(str);
			stChild->expressions = newExpression;
		}
	}
	else if (stChild->cmdType == String("set") || stChild->cmdType == String("var")) {
		//name of the variable:
		read_next_token();
		str = next_token();
		stChild->var = str;
		//symbol, number or another variable name
		read_next_token();
		str = next_token();
		stChild->stmntType = STEXP;
        auto * newExpression = new Expression;
		newExpression->Expression::shortInsert(str);
		stChild->expressions = newExpression;

	}
	else if (stChild->cmdType == String("do") || stChild->cmdType == String("if")) {
		read_next_token();
		str = next_token();
		stChild->stmntType = STEXP;
        auto * newExpression = new Expression;
		newExpression->Expression::shortInsert(str);
		stChild->expressions = newExpression;
	}
	//function: function name + parameters (param1 + param2 + .....)
	else if (stChild->cmdType == String("defun")) {
		stChild->stmntType = STFUNC;
		read_next_token();
		str = next_token();
		stChild->text = str;
		//parameters list
		read_next_token();
		read_next_token();
		str = next_token();
		//list of parameters
        auto * sVec = new StringVec;	//list of strings
		//add parameters until done (smarap)
		while (str != "smarap") {
			String* strPtr = &str; //points to the string not a statement list
			sVec->StringVec::insert(strPtr);
			read_next_token();
			str = next_token();
		}
		stChild->MultiPass = sVec;
	}
	//if return reached end this misery
	else if (stChild->cmdType == String("return")) {
		stChild->stmntType = STRET;
		read_next_token();
        auto * newExpression = new Expression;
		str = next_token();
		newExpression->Expression::shortInsert(str);
		stChild->expressions = newExpression;
	}

	return stChild;
}

/*-----------------------------Process tokens-----------------------------------------------*/
/*
//This function is used to process the Output token
*/
void Statements::processOut(StmntNode * s, VarTable & locVar) {
	if (s->stmntType == STEXP) { 
		printf("%d", s->expressions->Expression::shortExecExp(this, locVar)); 
	}
}

/*
//This function is used to process the Set token
*/
void Statements::processSet(StmntNode * s, VarTable & locVar) {
	int32_t i = 0;
	//send out warnings
	//set shudn exist in tree
	if (&locVar != &globStore && s->cmdType == "set") {
		if (!locVar.VarTable::Check_var(s->var) && !globStore.VarTable::Check_var(s->var))
        { //if trying to set a variable, does it already exist?
			printf("variable %s not declared\n", s->var.c_str());
			return;
		}
	}
	//var shudn't exist in tree
	if (locVar.VarTable::Check_var(s->var) && s->cmdType == "var") { 
		printf("variable %s incorrectly re-initialized\n", s->var.c_str());
		return;
	}
	//done


	if (s->cmdType == "var") {
		locVar.VarTable::insert(s->var);
	}

	if (locVar.VarTable::Check_var(s->var)) {
		i = s->expressions->Expression::shortExecExp(this, locVar);
		locVar.VarTable::get_var_value(s->var) = i;
	}
	else {
		globStore.VarTable::get_var_value(s->var) = i;
		i = s->expressions->Expression::shortExecExp(this, this->globStore);
	}
}

/*
//This function is used to process the Symbols (token)
*/
void Statements::processSymbol(String op) {
	if (op == "\n" || op == "#") { printf("\n"); }
}


/*
//This function is used to add a statement into the database
*/
int Statements::insert(StmntNode * s) {
	//if capacity is full then int it
	if (this->instList->capacity == this->instList->length) {
		instList->capacity *= 2;
		if (instList->capacity == 0) { instList->capacity = 1; }
        auto * temp = new StmntNode[instList->capacity];
		for (int k = 0; k < instList->length; k += 1) {
			temp[k] = instList->data[k];
		}
		delete[] instList->data;
		this->instList->data = temp;
	}

	//inserting the new statement node whcih has cmd and stmnt type with a variable name and expression list
	this->instList->data[this->instList->length] = *s;
	instList->length += 1;
	return this->instList->length;
}

/*
//This function is used to process the Do token
*/
int Statements::processDo(VarTable & locVar, int counter) {
	int  countDo = 1, countOd = 0, end = counter;
	StmntNode* loopCond = this->instList->data + counter;
	StmntNode* st;
	//count the statemnts
	while (countDo > countOd) {
		end += 1;
		st = instList->data + end;
		if (st->cmdType == "do") { countDo += 1; }
		if (st->cmdType == "od") { countOd += 1; }
	}
	//start executing loop expressions.... or statements
	while (loopCond->expressions->Expression::shortExecExp(this, locVar) != 0) {
        execute(this->globStore, locVar, counter + 1, end);
	}
	return end;
}

/*
//This function is used to process the If token
*/
int Statements::processIf(VarTable & locVar, int counter) {
	int end = counter;
	StmntNode* ifCond = this->instList->data + counter;
	StmntNode* st;
	int32_t ifcnt = 1, fi = 0, elsecnt = 0, esle = 0;
	//process the if and add statements
	while (fi<ifcnt) {
		end += 1;
		st = instList->data + end;
		if (st->cmdType == "if") { ifcnt += 1; }
		if (st->cmdType == "else") {
			esle += 1;
			if (fi == ifcnt - 1) { elsecnt = end; }
		}
		if (st->cmdType == "fi") {
			fi += 1;
		}
	}
	//execute if statement condition
	if (ifCond->expressions->Expression::shortExecExp(this, locVar) != 0) {
		if (elsecnt == 0) { elsecnt = end; }

		returnV = execute(this->globStore, locVar, counter + 1, elsecnt);
	}
	else {
		if (elsecnt != 0) {
			returnV = execute(this->globStore, locVar, elsecnt + 1, end);
		}
	}
	return end;
}


/*
//This function is used to process the Function token
*/
int Statements::processFunc(ExpressionVec * vec, VarTable & locVar, String & name) {
	//will contain the locals of the functions
    auto * locals = new VarTable;
	//func name
	String p;
	//just a string vector for the parameters (args)
	StringVec* str_vec;
	//index of the beginning of the function
	int  index_v = functStore.FunctionExp::shortFind(name, p);
	// if multiplass then has parameters
	str_vec = this->instList->data[index_v].MultiPass;
	//go through the function and get the locals on the varTable
	for (unsigned int k = 0; k<str_vec->length; k += 1) {
		locals->VarTable::insert(str_vec->data[k]);
		Expression* temp = vec->data + k;
		int i = temp->Expression::shortExecExp(this, locVar);
		locals->VarTable::get_var_value(str_vec->data[k]) = i;
	}
	//limit if the end of the functioon
	int lim = index_v;
	StmntNode* next_st;
	//how many calls in between the function
	contCaller += 1;
	int control = contCaller;
	//this counts the number of nested functions
	VarTable& local = *locals;
	int defunCtr = 1;
	int nufedCtr = 0;
	while (nufedCtr<defunCtr) {
		lim += 1;
		next_st = instList->data + lim;
		if (next_st->cmdType == "defun") { defunCtr += 1; }
		else if (next_st->cmdType == "nufed") { nufedCtr += 1; }
	}
	//executing functions from funcCalls
    execute(this->globStore, local, index_v + 1, lim);
	if (contCaller == control) {
		contCaller -= 1;
		returnV = 0;
	}

	return returnV;
}

/*------------------build, run and execute------------------*/
/*
//This function is used to Run, process and load the tokens with their
// parameters, then execute the sentences (by calling execute function)
*/
Statements & Statements::build() {
	StmntNode* temp;
	//create new statement
    auto * child = new Vector;
	this->instList = child;
	do {
		temp = prepStmnts();
		if (temp) {//inserting new bubble into database
			this->insert(temp);
		}
	} while (temp);

	contCaller = 0;
//    run(this->globStore, this->globStore, 0, this->instList->length);
    return *this;
}

/*
//This function is used to process and execute the saved sentences in the database
*/
int Statements::execute(VarTable &globVar, VarTable &locVar, int begBlock, int endBlock) {
	//create a temporary statement
	StmntNode* next_st;
	int k = begBlock, control = contCaller;
	//check statements in the database one by one and execute them
	//by calling the corresponding process subroutine
	while (k < endBlock && control == contCaller) {
		next_st = instList->data + k;
		if (next_st->cmdType == String("do")) {
			k = processDo(locVar, k);  
		}
		else if (next_st->cmdType == String("if")) {
			k = processIf(locVar, k); 
		}
		else if (next_st->cmdType == String("defun")) {  
			this->functStore.FunctionExp::insert(k, next_st->text, next_st->Param);
			int defunCtr = 1, nufedCtr = 0;
			while (nufedCtr<defunCtr) {
				k += 1;
				next_st = instList->data + k;
				if (next_st->cmdType == String("defun")) {
					defunCtr += 1;
				}
				else if (next_st->cmdType == String("nufed")) { nufedCtr += 1; }
			}
		}
		else if (next_st->cmdType == String("return")) {
			contCaller -= 1;
			returnV = next_st->expressions->Expression::shortExecExp(this, locVar);
			return returnV;
		}
		else if (next_st->tokenType == SYMBOL) {
			processSymbol(next_st->text);
		}
		else if (next_st->cmdType == String("text")) {
			printf("%s", next_st->text.c_str());
		}
		else if (next_st->cmdType == String("output")) {
			processOut(next_st, locVar);
		}
		else if (next_st->cmdType == String("set") || next_st->cmdType == String("var")) {
			processSet(next_st, locVar);
		}
		k += 1;
	}
	return k;
}

void Statements::run() {
        execute(this->globStore, this->globStore, 0, this->instList->length);
}

//End of file.