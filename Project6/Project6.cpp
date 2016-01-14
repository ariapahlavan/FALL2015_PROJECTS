//Project 6
//Aria Pahlavan - ap44342
//EE 312 - Fall 2015



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "MazeParams.h"
#include "Recursion.h"



int isOK(int row, int col);

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 */
int minIt(int x[], int n) {
	int min = x[0];
	for (int i = 0; i < n; i++)
	{
		if (min > x[i])  min = x[i];
	}
	return min;
}

/* return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * solve the problem recursively and
 * use an "n-1" type of decomposition
 */
int minRec1(int x[], int n) {
	int min = 0xbeef;
	if (n == 0) {
		if (x[0] >= x[n - 1]) {
			return x[1];
		}
		if (x[0] < x[n - 1]) {
			return x[0];
		}
	}

	if (x[0] >= x[n - 1]) {
		min = minRec1(x + 1, n - 1);
		if (x[n - 1] <= min) return x[n - 1];
		else return min;
	}
	if (x[0] < x[n - 1]) {
		min = minRec1(x, n - 1);
		if (x[0] <= min) return x[0];
		else return min;
	}
	return 0xbeef;
}

/*
 * return the smallest of the elements in array x[]
 * there are n elements in x[] (x[0].. x[n-1])
 * n may be either odd or even
 * solve the problem recursively and
 * use an "n / 2" type of decomposition
 */
int minRec2(int x[], int n) {
	if (n > 1) {
		int half = (n) / 2;

		int small1 = minRec2(x, half);
		int small2 = minRec2(x + half, (n - half));
		if (small1 < small2) {
			return small1;
		}
		else {
			return small2;
		}
		//return smaller of small1 and small2 here;
	}
	else//n =1;
		return x[0];

	return 0xbeef;
}


/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtIt(double x, double low_guess, double high_guess) {
	return 0;
}

/*
 * calculate and return the square root of x.
 * The other two parameters are estimates of the square root.
 * low_guess is smaller than the actual square root, and high_guess
 * is larger than the actual square root.
 * Searching for the square root can be done much like searching
 * for a name in a phone book.
 *
 * Since you can't calculate a square root exactly, for this problem
 * you're required to calculate the answer to 15 decimal digits of
 * accuracy.
 */
double sqrtRec(double x, double low_guess, double high_guess) {
	double half = (low_guess + high_guess) / 2;
	
	if (half*half == x) {
		return half; //BAAAM! you got it
	} 
	else if (high_guess - low_guess < 0) {
		return low_guess;
	}
	if (half*half < x) {
		return sqrtRec(x, half + 0.000000000000001, high_guess);
	}
	else if (half*half > x) {
		return sqrtRec(x, low_guess, half - 0.000000000000001);
	}
	
	return 0xbeef;
}


/*
 * using only recursion, write a string comparison function
 * return -1 if str1 is less than str2
 * return 0 if the two strings are equal
 * return +1 if str1 is greater than str2
 * when comparing strings, use the ASCII value to compare each character
 * (i.e., that means 'A' is less than 'a' since it's ASCII value is smaller)
 * The string str1 is less than str2 if either
 * str1[0] < str2[0]
 * or there exists a k such that str1[k] < str2[k] and
 *   for all j < k str1[j] == str2[j]
 *   and k is less than the length of str1 and str2
 */

int strCompare(char* str1, char* str2) {

	//base case
	if (str1[0] == NULL && str2[0] != NULL) return -1;
	if (str1[0] != NULL && str2[0] == NULL) return +1;
	if (str1[0] == NULL && str2[0] == NULL) return 0;
	if (str1[0] < str2[0]) return  -1;
	if (str1[0] > str2[0]) return  +1;

	//recursion
	return strCompare(str1 + 1, str2 + 1);

}

/*
 * if c is not a letter return -1
 * if c is a letter, return the position of c in the alphabet
 * where 'A' (and 'a') are position 1 and 'Z' (and 'z') are position 26
 *
 * This code is rather ugly as I'm exploiting some detailed knowledge of the ASCII table
 */
int whatLetter(char c) {
	if (c < 'A') { return -1; }
	if (c > 'z') { return -1; }
	if (c > 'Z' && c < 'a') { return -1; }
	return c & ~32 - 64;
}

/*
 * same as before, only this time
 * ignore anything that is not a letter
 * ignore the case (upper case, lower case)
 * So, strCompare2("Hello", "hello") should return 0 (they're the same)
 * strCompare2("The plane!", "theater") should return 1 since "theplane" is larger than "theater"
 * once again, you can only use recursion, no loops
 */
int strCompare2(char* str1, char* str2) {

	//base case
	if (whatLetter(str1[0]) == -1 && str1[0] != 0) strCompare2(str1 + 1, str2);
	if (str2[0] != 0 && whatLetter(str2[0]) == -1) strCompare2(str1 , str2 + 1);

	if (str1[0] == NULL && str2[0] != NULL) return -1;
	if (str1[0] != NULL && str2[0] == NULL) return +1;
	if (str1[0] == NULL && str2[0] == NULL) return 0;
	if (whatLetter(str1[0]) < whatLetter(str2[0])) return  -1;
	if (whatLetter(str1[0]) > whatLetter(str2[0])) return  +1;

	//recursion
	if (whatLetter(str1[0]) == whatLetter(str2[0])) return strCompare2(str1 +1, str2+1);

}



/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the
 * value is 1).  Each 0 represents an open space.
 *
 * Write an recursive solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 *
 * There is a relatively easy recursive solution to this problem, the trick is
 * to think of the solution in the following terms:
 *   "Not counting the steps I've taken so far to get out of this maze,
 *    what is the minimum number of steps I can take from here, without
 *    backtracking, and get to the exit?"
 * In some cases, you may find yourself in a spot in the maze that there's
 * no way out (at least, not without backtracking). In that case, the number
 * of steps you'd have to take is infinite. You can use the value -1 to represent
 * this possibility. If you had a function that could answer that question for you
 * from each possible direction you might consider going, then you'd (obviously)
 * call that function on each possible direction and choose whichever possiblity
 * returned the shortest number of steps to the exit.
 *
 * With that hint, the base case should be pretty obvious.  In fact,
 * I'd suggest you consider the possibility that the base case is "Yup, the
 * shortest path to the exit is zero steps from here, I know 'cause I'm already at
 * the exit!"
 *
 * There is one tricky part to this decomposition.  You need to make certain
 * that the problem is getting smaller.  The "bigness" or "smallness" of this
 * problem is the number of squares that have not yet been tested.  Each time
 * you test an adjacent square (making a recursive call to decide if the
 * shortest path to the exit goes through that square), you'll be reducing
 * the number of squares that have not yet been tested.  Eventually, you must
 * have tested all the squares and so you'll have to have found a way to the
 * exit.
 *
 * The easy way to deal with the tricky part is to leave "bread crumbs" behind.
 * Before you recursively check any (or all) of your neighbors to see if you
 * can find the exit from there -- drop a bread crumb in your current square.
 * Then, don't ever check to see if you can find the exit using a square
 * with a breadcrumb in it (backtracking into that square would be silly).
 *
 * If you're trying to see if you can find an exit from some square, and all
 * the adjacent squares are either walls, or have bread crumbs in them, then
 * you already know the answer -- "The shortest path to the exit does NOT go
 * through this square".  Pick up your bread crumb and return infinity (or -1).
 *
 * You can set the value of the current square to "2" to indicate that a bread
 * crumb has been dropped.  Set the square back to 0 when you want to pick
 * the bread crumb back up.
 * be sure not to change any of the squares that are 1 (the walls).
 *
 * for partial credit, you can leave all your bread crumbs on the floor.
 * for full credit, you must pick up all the bread crumbs EXCEPT those
 * along the shortest path to the exit, and you must return the number of
 * steps along that path.
 */


int solveMazeRec(int row, int col) {
	//base case
	if (row == MATRIX_SIZE - 1 && maze[row][col] == 0) {
		maze[row][col] = 2; // put a bread
		return 1;
	}
	//curr position
	//already bread
	else if (maze[row][col] == 2) return 0;
	//wall
	else if (maze[row][col] == 1) return 0;

	//body: check if we're in a blocked path
	else if (maze[row - 1][col] > 0 && maze[row + 1][col] > 0 && maze[row][col - 1] > 0 && maze[row][col + 1] > 0)
		//no direction is ok? dang that's infinite sol
		return 0;

	maze[row][col] = 2; //put a bread

	//choose a path
	int dir1 = solveMazeRec(row, col - 1);
	int dir2 = solveMazeRec(row, col + 1);
	int dir3 = solveMazeRec(row + 1, col);
	if (dir3 == 1) return 1; //hit the end buddy
	int dir4 = solveMazeRec(row - 1, col);

	//backtrack?
	if (dir1 == 0 && dir2 == 0 && dir3 == 0 && dir4 == 0) {
		//backtrack pick up bread
		maze[row][col] = 0; //pick up bread
		return 0;
	}
	else return 1;

}
/**********************
 * adjacentCell and isOK are functions provided to help you write
 * solveMazeIt()
 */

 /*
  * OK, we're currently at maze[row][col] and we're considering moving
  * in direction dir.
  * Set trow and tcol (local variables inside the calling function)
  * to the row and column that we would move to IF we moved in
  * that direction
  *
  * For example, there are two good ways to use this function.
  * 1. to actually move one step in a direction use:
  *       adjacentCell(row, col, dir, &row, &col);
  *    That will set row and col to new values.  The new values will
  *    be one square away from the old values.
  *
  * 2. to set trow and tcol to a square that is adjacent to row and col use:
  *       adjacentCell(row, col, dir, &trow, &tcol);
  *    That will not change row and col, but will change trow and tcol.
  *    This is useful if you aren't sure if you can actually move in this
  *    direction yet (e.g., maze[trow][tcol] may be a wall!).  So, you set
  *    trow and tcol, and then check to see if it's OK to move there
  */
void adjacentCell(int row, int col, int dir, int* trow, int* tcol) {
	*trow = row;
	*tcol = col;
	switch (dir) {
	case 0: // UP
		*trow = *trow - 1;
		break;
	case 1: // RIGHT
		*tcol = *tcol + 1;
		break;
	case 2: // DOWN
		*trow = *trow + 1;
		break;
	case 3: // LEFT
		*tcol = *tcol - 1;
		break;
	}
}


/*
 * return false if there is a wall in the square for row and col
 * return true if it's not a wall.
 */
int isOK(int row, int col) {
	return (row > 0 && row < MATRIX_SIZE
		&& col > 0 && col < MATRIX_SIZE
		&& maze[row][col] != 1);
}

/*
 * return the value of the direction that is one turn to the right
 */
int turnRight(int dir) {
	return (dir + 1) % 4;
}

/*
 * return the value of the direction that is one turn to the left
 */
int turnLeft(int dir) {
	return (dir + 3) % 4;
}

/*
 * the two dimensional array maze[MATRIX_SIZE][MATRIX_SIZE] contains a maze
 * Each square is (initially) either a 1 or a zero.  Each 1 represents a wall
 * (you cannot walk through walls, so you can't go into any square where the value
 * is 1).  Each 0 represents an open space.
 *
 * write an iterative solution to find your way out of the maze
 * Your starting point is at row and col. (params to this function)
 * Your exit point is somewhere in the last row (which is: MATRIX_SIZE - 1)
 * The maze can be solved by following the right hand wall.  In fact, there
 * is exactly one path between any two points in the maze (well, between any two
 * points that are not walls).
 *
 * The algorithm is not too bad, although it is certainly not trivial
 * Initially, you'll be headed DOWN (direction 2)
 * Each iteration do one of the following.  Note that only one of two cases
 * can possibly apply (the properties of the maze guarantee that).
 *    case 1: you can move in the current direction
 *       In this case, you should take one step in the current direction
 *       and then turn right.
 *    case 2: you cannot move in the current direction
 *       If there's a wall directly in front of you, then to follow the right
 *       hand wall, you'd need to turn left (placing your hand on the wall that
 *       used to be directly in front of you).  So, if you discover this case
 *       then turn left.  Don't move, just turn left.
 * If you were walking down a straight corridor (with walls on both sides)
 * then you'd alternate case 1 and case 2.  On the first iteration, you'd
 * take a step, and then turn right (case 1).  This causes you to walk
 * one position down the hallway but then turn to face the wall.
 * On the next iteration, you'll be facing the wall, so you can't walk
 * forward and therefore (case 2) you'll turn left.  On the third iteration
 * you'll be able to walk down the hallway again.
 *
 * If you follow those two cases, then you'll eventually find your way out
 * of the maze.  To confirm that you're making it out, leave a "bread crumb"
 * behind each square along the path to the exit.
 * For partial credit, place a bread crumb in every square you visit.
 * For full credit, pick up all the breadcrumbs when you backtrack.
 * Backtracking is when you go back to a square you've already been before.
 * If the square you're about to step into has a breadcrumb, then pick up
 * the bread crumb in the square you're at now.
 * You indicate "bread crumbs" by setting the square equal to "2"
 */
void solveMazeIt(int row, int col) {
	int dir = 2; // 0 is up, 1 is right, 2 is down, 3 is left.
	maze[row][col] = 2; // drop a bread crumb in the starting square
	while (row < MATRIX_SIZE - 1) { // the exit is the only open square 
				// in the last row

		break;	/* the rest of this loop is yours */

	}
}

/*
/*
 * using recursion, with no loops or globals, write a function that calculates the optimal
 * (fewest total coins) change for a given amount of money. Return a Martian struct that indicates
 * this optimal collection of coins.
 */
Martian coutnMoney(Martian a, Martian b) {
	Martian money{ a.pennies + b.pennies , a.nicks + b.nicks, a.dodeks + b.dodeks };
	return money;
}

Martian change(int cents) {
	Martian money;
	money.dodeks = 0;
	money.nicks = 0;
	money.pennies = 0;
	int leastMonNum = -1;
	Martian leastMo;
	//base case
	if (cents == 0) return money;
	if (cents == 5) { money.nicks = 1; return money; }
	if (cents == 1) { money.pennies = 1; return money; }
	if (cents == 12) { money.dodeks = 1; return money; }

	//body
	//option 1 
	if (cents > 5) {
		
		Martian n = { 0,1,0 };
		n = coutnMoney(n, change(cents - 5));
		int totN = n.dodeks + n.nicks + n.pennies;
		leastMo = n;
		leastMonNum = totN;
	}
	//option 2
	if (cents > 12) {
		Martian d = { 0,0,1 };
		d = coutnMoney(d, change(cents - 12));
		int totD = d.dodeks + d.nicks + d.pennies;
		if (leastMonNum != -1) {
			if (totD < leastMonNum) {
				leastMo = d;
				leastMonNum = totD;
			}
		}
		else {
			leastMo = d;
			leastMonNum = totD;
		}
	}
		//option 3
		
			Martian p = { 1,0,0 };
			p = coutnMoney(p, change(cents - 1));
			int totP = p.dodeks + p.nicks + p.pennies;
			if (leastMonNum != -1) {
				if (totP < leastMonNum) {
					leastMo = p;
					leastMonNum = totP;
				}
			}
			else {
				leastMo = p;
				leastMonNum = totP;
			}
		
			return leastMo;
}
		

/*
 * Same as before, except the more general version where the coins have values
 * a cents and b cents, with a and b being algebraic. For the original problem,
 * a is the constant 5 (nick_value = 5 cents) and b is the constant 12
 * (dodek_value = 12 cents).
 * If you've really mastered thinking recursively, then this version of the
 * martian change problem is just as easy as the concrete version
 */
Martian change(int cents, int nick_val, int dodek_val) {
	Martian money;
	money.dodeks = 0;
	money.nicks = 0;
	money.pennies = 0;
	int leastMonNum = -1;
	Martian leastMo;
	//base case
	if (cents == 0) return money;
	if (cents == nick_val) { money.nicks = 1; return money; }
	if (cents == 1) { money.pennies = 1; return money; }
	if (cents == dodek_val) { money.dodeks = 1; return money; }

	//body
	//option 1 
	if (cents > nick_val) {

		Martian n = { 0,1,0 };
		n = coutnMoney(n, change(cents - nick_val));
		int totN = n.dodeks + n.nicks + n.pennies;
		leastMo = n;
		leastMonNum = totN;
	}
	//option 2
	if (cents > dodek_val) {
		Martian d = { 0,0,1 };
		d = coutnMoney(d, change(cents - dodek_val));
		int totD = d.dodeks + d.nicks + d.pennies;
		if (leastMonNum != -1) {
			if (totD < leastMonNum) {
				leastMo = d;
				leastMonNum = totD;
			}
		}
		else {
			leastMo = d;
			leastMonNum = totD;
		}
	}
	//option 3

	Martian p = { 1,0,0 };
	p = coutnMoney(p, change(cents - 1));
	int totP = p.dodeks + p.nicks + p.pennies;
	if (leastMonNum != -1) {
		if (totP < leastMonNum) {
			leastMo = p;
			leastMonNum = totP;
		}
	}
	else {
		leastMo = p;
		leastMonNum = totP;
	}

	return leastMo;
}

/*
 * without using recursion, solve the Martian change problem described above.
 * it's not too bad for the specific case of nick_value = 5 and dodek_value = 12
 */
Martian changeIt(int cents) {
	cents++;
	return Martian{}; // delete this line, it's broken. Then write the function properly!
}

/*
 * However, I don't have a solution for the general case. In fact, I consider
 * the general problem to "challenging" (i.e, an exercise in futility).
 * But, if you like banging your head against the wall for zero credit
 * this is the problem for you!
 */
Martian changeIt(int cents, int nick_value, int dodek_value) {
	return Martian{}; // delete this line, it's broken. Then write the function properly!
}

