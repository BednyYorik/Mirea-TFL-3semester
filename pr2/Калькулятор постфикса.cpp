#include <iostream>
#include <stack>
#include <string>

using namespace std;

int main()
{
    stack <int> stack;
    string str;
    cout << "Postfix: ";
    getline(cin, str);

    for (int i = 0; i < str.length(); i++)
    {
        string temp = "";
        if (isdigit(str[i]))
        {
            temp += str[i];
            while (isdigit(str[i + 1])) {
                temp += str[i+1];
                i++;
            }
            stack.push(stoi(temp));
        }
        else if (str[i] == '+')
        {
            int a, b;
            a = stack.top();
            stack.pop();
            b = stack.top();
            stack.pop();
            stack.push(b + a);
        }
        else if (str[i] == '-')
        {
            int a, b;
            a = stack.top();
            stack.pop();
            b = stack.top();
            stack.pop();
            stack.push(b - a);
        }
        else if (str[i] == '*')
        {
            int a, b;
            a = stack.top();
            stack.pop();
            b = stack.top();
            stack.pop();
            stack.push(b * a);
        }
        else if (str[i] == '/')
        {
            int a, b;
            a = stack.top();
            stack.pop();
            b = stack.top();
            stack.pop();
            stack.push(b / a);
        }

    }
    cout << "Infix: " << stack.top();
}