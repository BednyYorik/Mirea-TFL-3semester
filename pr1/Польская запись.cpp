#include <iostream>
#include <string>
#include <stack>
#include <sstream>
using namespace std;

bool Operator(char op) {
	switch (op) {
	case ('+'):
	case ('-'):
	case('*'):
	case('/'):
		return true;
	default:
		return false;
	}
}

int prioritet(char oper) {
	switch(oper){
		case '+': 
			return 1;
		case '-': 
			return 1;
		case '*': 
			return 2;
		case '/': 
			return 2;
	}	
}

string Postfix(string& infix) {
	stringstream postfix;

	stack <char> stack;
	char current;

	for (int i = 0, l = infix.size(); i < l; ++i) {
		current = infix[i];
		// Если current число или буква, добавляем ее в выходную строку
		if ((current >= 48 && current <= 57) || (current >= 65 && current <= 90) || (current >= 97 && current <= 122)) {
			postfix << current;
			// Когда число имеет больше одной цифры
			while (isdigit(infix[i + 1])) {
				postfix << infix[i+1];
				i++;
			}
		}
		// Если current открывающая скобка, добавляем ее в стек
		else if (current == '(') {
			stack.push(current);
		}
		// Если current знак операции
		else if (Operator(current)) {
			char down = current;
			while (!stack.empty() && Operator(stack.top()) && ((prioritet(stack.top()) >= prioritet(down)))) {
				postfix << stack.top();
				stack.pop();
			}
			stack.push(down);
		}
		//Если current закрывающая скобка: отправляем стек(до открывающей скобки) в выходную строку и очищаем его. 
		else if (current == ')') {
			while (!stack.empty() && stack.top() != '(') {
				postfix << stack.top();
				stack.pop();
			}
			stack.pop();
		}
	}
	//ОСвобождаем стек в конце
	while (!stack.empty() && stack.top() != '(') {
		postfix << stack.top();
		stack.pop();
	}
	return postfix.str();
}


int main() {
	string infix;
	string postix;
	while (true) {
		cout << "Infix: ";
		cin >> infix;
		if (infix == "end") break;
		else {
			postix = Postfix(infix);
			cout << "Postfix: " << postix << endl;
		}
	}
}