//============================================================================
//Key delimeters in this code are
//    (x) = end of coefficient
//    (^) = signifies existence of exponent
//        ***(^-) = signifies negative exponent
//    (+, -) = signifies new term
//============================================================================

#include <iostream>
#include <string>

#define MAX_POLYNOMIAL 11
#define GENERAL_SIZE 20

using namespace std;

struct ATerm
{
    int coef;
    int exp;
    ATerm* next;
};
typedef ATerm* polynomial;

polynomial addPolynomials(const polynomial, const polynomial);// args: poly1, poly2
polynomial multiplyPolynomials(const polynomial, const polynomial);// args: poly1, poly2
polynomial storePoly(string);//args: strPoly
string stripChar(string, char);// args: poly, removeChar
string lowerX(string);// args: polyStr
string getPoly(int);// arg: iteration
signed int stringToInt(string, char='+');// args: integer, sign
int pow(int, int);//args: base, power
int getTotalPolyn();
char returnSign(const string);// arg: term
bool checkNumericValue(string);// arg: temp
bool checkExponents(string);// arg: poly
bool checkSigns(string);// arg: poly
bool checkBadInput(string, char);// arg: polu, criteria
bool checkInt(string); //arg: tempInput
bool checkBounds(int); // arg: value
bool checkString(string);//arg: tempValue
void displayPoly(const polynomial); // args: poly
void addNewNode(polynomial&, int, int);//args: p, base, degree
void displayStartingPoly(const polynomial[], int, char);//args: polyList, sizeOfList, operation

int main()
{
    string temp;
    int totalPoly;
    polynomial addPoly, multPoly;

    cout << "Welcome to the polynomial calculator. This program will allow you to\n"
         << "enter from 2-11 (0 will exit the program) polynomials and will return\n"
         << "the product and sum of those polynomials. ***Note: The polynomials must\n"
         << "be in reduced form, be in descending order, and any 0 values should be \n"
         << "inputed as 0 exactly.***\n"
         << "Format: ax^c to represent exponents {space characters are allowed}\n"
         <<"Enjoy!\n\n";

    totalPoly = getTotalPolyn();

    while (totalPoly != 0)
    {
        addPoly = NULL;
        multPoly = NULL;

        polynomial poly[totalPoly];

        for (int i = 0; i < totalPoly; i++)
        {
            poly[i] = NULL;
            temp = getPoly(i);
            temp = stripChar(temp, ' ');

            poly[i] = storePoly(temp);
        }
        // try the initial case outside of loop
        for (int i = 0; i < totalPoly; i++)
        {
            if (i == 0)
            {
                addPoly = addPolynomials(poly[i], poly[i+1]);
                multPoly = multiplyPolynomials(poly[i], poly[i+1]);
                i++;
            }
            else
            {
                addPoly = addPolynomials(addPoly, poly[i]);
                multPoly = multiplyPolynomials(multPoly, poly[i]);
            }
        }
        cout << "\nSum = ";
        displayStartingPoly(poly, totalPoly, '+');
        cout << "    = ";
        displayPoly(addPoly);
        cout << "\n\nProduct = ";
        displayStartingPoly(poly, totalPoly, '*');
        cout << "        = ";
        displayPoly(multPoly);
        cout << "\n\n----------------------------------------\n\n";

        totalPoly = getTotalPolyn();

    }

    return 0;
}

//=============================================================================
// Args:
//      -base: base value
//      -power: how many times to multiply the base
//
//  Used in the string to int conversion.
//=============================================================================
int pow(int base, int power)
{
    int total = 1;

    for (int i = 1; i < power; i++)
    {
        total = total * base;
    }

    return total;
}
//=============================================================================
//  Args:
//      -integer: string value containing integers
//      -sign: current sign of the integer
//
// Returns a positive or negative value based on the first character of the
// string.
//=============================================================================
signed int stringToInt(string integer, char sign)
{
    int total = 0;
    int valueSize = integer.size();

    if (valueSize == 0)
    {
        (sign == '-') ? total = -1 : total = 1;

        return total;
    }

    for (int i = valueSize - 1; i >= 0; i--)
    {
        int multiple = pow(10, valueSize - i);
        int currentPos = integer[i] - '0';

        total = total + (currentPos * multiple);
    }

    if (sign == '-')
    {
        total = total * -1;
    }

    return total;
}
//=============================================================================
//  Args:
//      -poly: string version of polynomial
//      -removedChar: value to be removed from the string
//
//  Removes the given character from a string and returns the new string without
//  that value.
//=============================================================================
string stripChar(string poly, char removedChar)
{
    string nStr;
    int len = poly.size();

    for (int i = 0; i < len; i++)
    {
        if (poly[i] != removedChar)
        {
            nStr = nStr + poly[i];
        }
    }

    return nStr;
}
//=============================================================================
//  Args: None
//
//  Prompts the user to enter the total polynomials to be added and multiplied.
//=============================================================================
int getTotalPolyn()
{
    string temp;
    bool safe;

    do
    {
        cout << "Number of polynomials -> ";
        getline(cin, temp);
        cin.clear();

        temp = lowerX(temp);
        temp = stripChar(temp, ' ');
        safe = checkBadInput(temp, '#');

        if (!safe)
        {
            cout << "Invalid input! Please enter valid size.\n";
        }
    } while (!safe);

    return stringToInt(temp);
}
//=============================================================================
//  Args:
//      -polyStr: user entered polynomial
//
//  Formats every 'X' character in polyStr to 'x', so that error checking can
//  be handled more efficiently.
//=============================================================================
string lowerX(string polyStr)
{
    string newPoly;

    for (int i = 0; i < polyStr.size(); i++)
    {
        if (polyStr[i] == 'X')
        {
            newPoly = newPoly + 'x';
        }
        else
        {
            newPoly = newPoly + polyStr[i];
        }
    }

    return newPoly;
}
//=============================================================================
//  Args:
//      -polyStr: validated user entered polynomial
//
//  This removes the first character from polyStr and returns that value and
//  changes the polyStr data.
//=============================================================================
char grabFirst(string &polyStr)
{
    string temp;
    char firstCh = polyStr[0];

    for (int i = 1; i < polyStr.size(); i++)
    {
        temp = temp + polyStr[i];
    }

    polyStr = temp;

    return firstCh;
}
//=============================================================================
//  Args:
//      -p1, p2: polynomial data types containing a coefficient and degree
//
//  This function adds and formats two polynomials in descending order given
//  two sorted polynomials.
//=============================================================================
polynomial addPolynomials(const polynomial p1, const polynomial p2)
{
    polynomial solution;
    polynomial current1, current2;
    int base, degree;

    base = degree = 0;
    solution = NULL;
    current1 = p1, current2 = p2;

    while (current1 && current2)
    {
        if (current1->exp > current2->exp)
        {
            base = current1->coef;
            degree = current1->exp;
            current1 = current1->next;
        }
        else if (current1->exp < current2->exp)
        {
            base = current2->coef;
            degree = current2->exp;
            current2 = current2->next;
        }
        else
        {
            base = current1->coef + current2->coef;
            degree = current1->exp;
            current1 = current1->next;
            current2 = current2->next;
        }

        addNewNode(solution, base, degree);
    }

    while (current1 || current2)
    {
        if (!current1)
        {
            base = current2->coef;
            degree = current2->exp;
            addNewNode(solution, base, degree);
            current2 = current2->next;
        }
        else
        {
            base = current1->coef;
            degree = current1->exp;
            addNewNode(solution, base, degree);
            current1 = current1->next;
        }
    }

    return solution;
}
//=============================================================================
//  Args:
//      -p1, p2: polynomial data type containing a degree and a coefficient
//
//  Multiplies a each term in p1 by every value in p2, then calls the
//  addPolynomials function to organize the resulting polynomial and combine
//  like terms after each pass.
//=============================================================================
polynomial multiplyPolynomials(const polynomial p1, const polynomial p2)
{
    polynomial sHead, newHead;
    polynomial c1 = p1, c2 = p2;
    int base, degree;

    sHead = NULL;
    newHead = NULL;
    base = degree = 0;

    while (c1)
    {
        while (c2)
        {
            base = c1->coef * c2->coef;
            degree = c1->exp + c2->exp;
            addNewNode(sHead, base, degree);
            c2 = c2->next;
        }

        (!newHead) ? newHead = sHead : newHead = addPolynomials(newHead, sHead);
        sHead = NULL;
        c1 = c1->next;

        if (c1)
        {
            c2 = p2;
        }
    }

    while (c2)
    {
        base = c2->coef;
        degree = c2->exp;
        addNewNode(sHead, base, degree);
        c2 = c2->next;
    }

    return newHead;
}
//=============================================================================
//  Args:
//      -p: polynomial data type containing a coefficient and degree.
//
//  Displays each polynomial in the format:
//          ax^k+bx+c
//  given a degree 2, and changes based on the degree of each polynomial.
//=============================================================================
void displayPoly(const polynomial p)
{
    polynomial current;
    bool fTerm = true;

    current = p;

    while (current)
    {
        if (fTerm && current->coef != 0)
        {
            if (current->exp == 0 || (current->coef != 1 && current->coef != -1))
            {
                cout << current->coef;
            }
            else if (current->coef == -1 && current->exp != 0)
            {
                cout << "-";
            }
            fTerm = false;
        }
        else if (fTerm && current->coef == 0)
        {
            cout << current->coef;
            fTerm = false;
        }
        else if (current->coef < 0)
        {
            if (current->coef == -1 && current->exp != 0)
            {
                cout << " - ";
            }
            else
            {
                cout << " - " << current->coef * -1;
            }
        }
        else
        {
            if (current->coef == 1 && current->exp != 0)
            {
                cout << " + ";
            }
            else
            {
                cout << " + " << current->coef;
            }
        }

        if (current->exp > 1 || current->exp < 0)
        {
            cout << "x^" << current->exp;
        }
        else if (current->exp == 1)
        {
            cout << 'x';
        }

        current = current->next;
    }
}
//=============================================================================
//  Args:
//      -polyStr: verified polynomial input from the user
//
//  Takes in a pre-verified (entered by the user) and stores the data into a
//  linked-list of polynomial data. It stores the degree of each term and the
//  coefficient, if they exist. It then returns a polynomial with stored data.
//=============================================================================
polynomial storePoly(string polyStr)
{
    polynomial head;
    int base, degree;
    char fc, sign;
    string temp;

    head = NULL;
    base = degree = 0;

    while (polyStr.size() != 0)
    {
        fc = grabFirst(polyStr);

        if (fc == 'x')
        {
            sign = returnSign(temp);
            temp = stripChar(temp, sign);
            base = stringToInt(temp, sign);
            temp = "";

            if (polyStr[0] == '-' || polyStr[0] == '+')
            {
                addNewNode(head, base, 1);
                base = degree = 0;
                fc = grabFirst(polyStr);
            }
        }
        else if (fc == '^' && polyStr[0] == '-')
        {
            fc = grabFirst(polyStr);
        }
        else if ((fc == '-' && temp.size() != 0) || fc == '+')
        {
            if (base != 0)
            {
                sign = returnSign(temp);
                temp = stripChar(temp, '-');
                degree = stringToInt(temp, sign);
            }
            else
            {
                sign = returnSign(temp);
                temp = stripChar(temp, sign);
                base = stringToInt(temp, sign);
                degree = 0;
            }

            addNewNode(head, base, degree);
            temp = "";
            degree = base = 0;
        }

        if (fc != '^' && fc != 'x')
        {
            temp = temp + fc;
        }
    }

    sign = returnSign(temp);
    temp = stripChar(temp, sign);
    (base == 0) ? base = stringToInt(temp, sign) : degree = stringToInt(temp, sign);

    addNewNode(head, base, degree);

    return head;
}
//=============================================================================
//  Args:
//      -p: list of polynomials
//      -base: new coefficient of the term
//      -degree: new degree of the term
//
//  Creates a new node containing the new coefficient and degree at the end of
//  the linked list formed by p.
//=============================================================================
void addNewNode(polynomial &p, int base, int degree)
{
    polynomial current, newNode;

    current = p;
    newNode = new ATerm;
    newNode->coef = base;
    newNode->exp = degree;
    newNode->next = nullptr;

    if (!p)
    {
        p = newNode;
    }
    else
    {
        while (current->next)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}
//=============================================================================
//  Args:
//      -term: a term to be converted to integer
//
//  Checks to see if the first character in term is a '-'. If it is, then it
//  returns a '-' otherwise returns '+'.
//=============================================================================
char returnSign(const string term)
{
    char sign;

    (term[0] == '-') ? sign = '-' : sign = '+';

    return sign;
}
//=============================================================================
//  Args:
//      -temp: string to be verified
//
//  Checks to make sure that every value in the string is an integer.
//=============================================================================
bool checkNumericValue(string temp)
{
    bool safe = true;
    int i = 0;
    int len = temp.size();

    if (len > 1 && temp[i] == '0')
    {
        return !safe;
    }

    while (i < len && safe)
    {
        if (temp[i] < '0' || temp[i] > '9')
        {
            safe = false;
        }

        i++;
    }

    return safe;
}
//=============================================================================
//  Args:
//      -poly[]: List containing the initial polynomials
//      -sizeOfList: bounds of the list
//      -operation: desired operation
//
//  Takes in a list of polynomials and displays them in a format in the form of
//              [ax^z + b] {operation} [cx^u + d]
//=============================================================================
void displayStartingPoly(const polynomial poly[], int sizeOfList, char operation)
{
    for (int i = 0; i < sizeOfList; i++)
    {
        if (i == sizeOfList - 1)
        {
            cout << '[';
            displayPoly(poly[i]);
            cout << ']' << endl;
        }
        else
        {
            cout << '[';
            displayPoly(poly[i]);
            cout << "] " << operation << ' ';
        }
    }
}
//=============================================================================
//  Args:
//      -iteration: current position in an array
//
//  Prompts the user to enter a polynomial and then verifies that it was
//  entered correctly.
//=============================================================================
string getPoly(int iteration)
{
    string temp;
    bool safe;

    do
    {
        cout << "Input Polynomial " << iteration + 1 << " -> ";
        getline(cin, temp);
        cin.clear();

        safe = checkBadInput(temp, 'p');

        if(!safe)
        {
            cout << "Invalid input! Please try again!\n";
        }
    } while (!safe);

    return temp;
}
//=============================================================================
//  Args:
//      -temp: temporary input
//      -criteria: criteria to be checked.
//
//  Takes in either a 'p' or '#' as the criteria to decided which verification
//  process to go through. The temp parameter must be formated by stripping all
//  space character and converting each 'X' to 'x'.
//=============================================================================
bool checkBadInput(string temp, char criteria)
{
    //Must strip space characters before calling this function.
    bool safe = true;
    int totalValue;
    int len = temp.size();

    if (len == 0)
    {
        return !safe;
    }

    if (criteria == '#')
    {
        safe = checkNumericValue(temp);

        if (safe)
        {
            totalValue = stringToInt(temp, '+');
            safe = checkBounds(totalValue);
        }
    }
    else
    {
        safe = checkExponents(temp) && checkSigns(temp)  && checkInt(temp) && checkString(temp);
    }

    return safe;
}
//=============================================================================
//  Args:
//      -value: integer value to be checked.
//
//  Checks to see if value is either equal to 0 or in the range of 2 and
//  MAX_POLYNOMIAL.
//=============================================================================
bool checkBounds(int value)
{
    return (0 == value || (2 <= value && value <= MAX_POLYNOMIAL));
}
//=============================================================================
//  Args:
//      -tempValue: temporary polynomial entered by user.
//
// Checks to see that all signs or special characters are followed by either a
// sign or a number based on the value read.
//=============================================================================
bool checkSigns(string tempValue)
{
    int len = tempValue.size();
    bool safe = true;
    int i = 0;

    while (i <= (len - 1))
    {
        if ((tempValue[i] == '-' || tempValue[i] == '+') &&
            (tempValue[i+1] != 'x' && (tempValue[i+1] < '0' ||
                                       tempValue[i+1] > '9')))
        {
            return !safe;
        }
        else if (tempValue[i] == '^')
        {
            if (i != (len - 1) && tempValue[i+1] == '-' &&
                (tempValue[i+2] < '1' || tempValue[i+2] > '9'))
                {
                    return !safe;
                }
            else if ((tempValue[i+1] < '1' || tempValue[i+1] > '9') &&
                     tempValue[i+1] != '-')
            {
                return !safe;
            }
        }
        else if (tempValue[i] == 'x')
        {
            if (i != len - 1 && (tempValue[i+1] != '^' && tempValue[i+1] != '+'
                                && tempValue[i+1] != '-'))
            {
                return !safe;
            }
        }
        i++;
    }

    return safe;
}
//=============================================================================
//  Args:
//      -tempValue: temporary polynomial
//
//  Checks to make sure valid integer input is entered for every coefficient
//  in each term.
//=============================================================================
bool checkInt(string tempValue)
{
    bool safe = true;
    int len = tempValue.size();

    for (int i = 0; i < len; i++)
    {
        if (i == 0 && i < len - 1 && tempValue[i] == '0')
        {
            if (tempValue[i+1] != '+' || tempValue[i+1] != '-')
            {
                return !safe;
            }
        }
        else if (tempValue[i] == '+' || tempValue[i] == '-')
        {
            if (tempValue[i + 1] ==  '0' && (i < len - 2 &&
                                            (tempValue[i+2] != '+' || tempValue[i+2] != '-')))
            {
                return !safe;
            }
            else if ((tempValue[i+1] < '1' || tempValue[i+1] > '9') &&
                     (tempValue[i+1] != 'x' && tempValue[i+1] != '0'))
            {
                return !safe;
            }

        }
    }

    return safe;
}
//=============================================================================
//  Args:
//      -tempPoly: temporary polynomial
//
//  Checks to make sure there are valid exponents in tempPoly.
//=============================================================================
bool checkExponents(string tempPoly)
{
    bool safe = true;

    for (int i = 0; i < tempPoly.size(); i++)
    {
        if(tempPoly[i] == '^')
        {

            (tempPoly[i+1] == '-') ? i = i + 2 : i++;

            if (tempPoly[i] == '0')
            {
                return !safe;
            }

            while (tempPoly[i] != '+' && tempPoly[i] != '-' && i < tempPoly.size())
            {
                if (tempPoly[i] < '0' || tempPoly[i] > '9')
                {
                    return !safe;
                }
                i++;
            }
        }
    }

    return safe;
}

//=============================================================================
//  Args:
//      -tempValue: temporary polynomial
//
//  Checks to make sure there are not any extra special characters.
//=============================================================================
bool checkString(string tempValue)
{
    int len = tempValue.size();
    bool safe = true;

    for (int i = 0; i < len; i++)
    {
        if (tempValue[i] != 'x' && tempValue[i] != '^' && tempValue[i] != '-' &&
            tempValue[i] != '+' && (tempValue[i] < '0' || tempValue[i] > '9'))
                return !safe;
    }

    return safe;
}
