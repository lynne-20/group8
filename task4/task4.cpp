#include <iostream>
#include <stack>
#include <string>
#include <cctype>

using namespace std;


const int left_association = 0;
const int right_association = 1;


const string operators = "+-*/";
const int precedence[] = {1, 1, 2, 2};
const int associativity[] = {left_association, left_association, right_association, right_association};

bool isOperator(char c) {
    return operators.find(c) != string::npos;
}

// To get precedence
int getPrec(char op) {
    int index = operators.find(op);
    if (index != string::npos) {
        return precedence[index];
    }
    return -1;
}

// To get associativity
int getAssoc(char op) {
    int index = operators.find(op);
    if (index != string::npos) {
        return associativity[index];
    }
    return -1;
}

bool hasHigherPrecedence(char op1, char op2) {
    int prec1 = getPrec(op1);
    int prec2 = getPrec(op2);
    if (prec1 == prec2) {
        if (getAssoc(op1) == right_association) {
            return false;
        }
        return true;
    }
    return prec1 > prec2;
}

int evaluateExpression(const string& expression) {
    stack<int> values;
    stack<char> operators;

    for (char token : expression) {
        if (token == ' ') {
            continue; // Skip whitespace
        }

        if (isdigit(token)) {
            int value = token - '0';
            values.push(value);
        } else if (isOperator(token)) {
            while (!operators.empty() &&
                   isOperator(operators.top()) &&
                   hasHigherPrecedence(operators.top(), token)) {
                char op = operators.top();
                operators.pop();
                int right = values.top();
                values.pop();
                int left = values.top();
                values.pop();
                if (op == '+') {
                    values.push(left + right);
                } else if (op == '-') {
                    values.push(left - right);
                } else if (op == '*') {
                    values.push(left * right);
                } else if (op == '/') {
                    values.push(left / right);
                }
            }
            operators.push(token);
        } else if (token == '(') {
            operators.push(token);
        } else if (token == ')') {
            while (!operators.empty() && operators.top() != '(') {
                char op = operators.top();
                operators.pop();
                int right = values.top();
                values.pop();
                int left = values.top();
                values.pop();
                if (op == '+') {
                    values.push(left + right);
                } else if (op == '-') {
                    values.push(left - right);
                } else if (op == '*') {
                    values.push(left * right);
                } else if (op == '/') {
                    values.push(left / right);
                }
            }
            if (!operators.empty() && operators.top() == '(') {
                operators.pop(); // Pop the '('
            }
        }
    }

    while (!operators.empty()) {
        char op = operators.top();
        operators.pop();
        int right = values.top();
        values.pop();
        int left = values.top();
        values.pop();
        if (op == '+') {
            values.push(left + right);
        } else if (op == '-') {
            values.push(left - right);
        } else if (op == '*') {
            values.push(left * right);
        } else if (op == '/') {
            values.push(left / right);
        }
    }

    return values.top();
}

int main() {
    string expression;
    char choice;

    do {
        cout << "Please enter a mathematical problem(You can use (),*,/,+ or -: ";
        getline(cin, expression);

        int result = evaluateExpression(expression);
        cout << "Result: " << result << endl;

        cout << "Do you want to enter another expression? (y/n): ";
        cin >> choice;
        cin.ignore();
    } while (choice != 'n' && choice != 'N');

    return 0;
}
