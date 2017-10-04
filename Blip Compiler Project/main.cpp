//Project 10
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015

#include <iostream>
#include "Project10.h"

using namespace std;


//New instruction template
Statements *programLoader = nullptr;
Statements program;

bool initTest(int testNum);

int main() {
    for (int i = 1; i < 10; ++i) {
        if (initTest(i)) {
            program.build().run();
        }

        cout << "-----------------------" << endl;
    }

	return 0;
}

bool initTest(int testNum){
    delete programLoader;
    programLoader = new Statements();
    program = *programLoader;

    auto input = string("../test" + to_string(testNum) +".blip");
    return set_input(input);
}

//End of file.