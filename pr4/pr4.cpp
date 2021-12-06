#include <iostream>
#include <fstream>
#include <regex>

#define NUM_OF_KWORDS 2

using namespace std;

int kwords_size = 2;
string* kwords = new string[kwords_size]{ "for", "do" };
enum states { H, ID, NM, ASGN, DLM, ERR };
enum tok_names { KWORD, IDENT, NUM, OPER, DELIM };

struct token {
    string tName;
    string tVal;
    token* next;
    token(string name, string val) {
        this->tName = name;
        this->tVal = val;
        this->next = nullptr;
    }
};

struct Tlex 
{
    token* first;
    token* last;

    Tlex() 
    {
        this->first = nullptr;
        this->last = nullptr;
    }

    bool is_empty()
    {
        return first == nullptr;
    }

    void add_token(string tName, string tVal) 
    {
        token* q = new token(tName, tVal);
        if (is_empty()) {
            first = q;
            last = q;
            return;
        }
        last->next = q;
        last = q;
    }

    void print()
    {
        token* q = first;
        if (is_empty()) return;
        cout << endl;
        while (q) {
            cout << q->tName << ": " << q->tVal << endl;
            q = q->next;
        }
        cout << endl;
    }
};

bool is_kword(string id)
{
    for (int i = 0; i < kwords_size; i++) 
    {
        if (kwords[i] == id) return true;
        return false;
    }
}

bool is_num(string temp) 
{
    regex example("^([+-])?(\\d)+((\\.)?([\\d]+))?(\\.[eE](\\d)+([+-]))?");
    return regex_match(temp.begin(), temp.end(), example);
}

int lexer(string path) 
{
    Tlex tl;
    ifstream f;
    f.open(path);
    if (f.is_open()) {
        string error_symbol = "\0";
        char sbl;
        enum states CS = H;
        string temp;
        sbl = f.get();
        while (!f.eof()) 
        {
            switch (CS) 
            {
            case H:
                if ((sbl == ' ') || (sbl == '\t') || (sbl == '\n'))
                {
                    sbl = f.get();
                    continue;
                }
                if (isalpha(sbl) || (sbl == '_')) CS = ID;
                else if ((sbl >= '0') && (sbl <= '9') || (sbl == '.') || (sbl == '+') || (sbl == '-')) CS = NM;
                else if (sbl == ':') CS = ASGN;
                else CS = DLM;
                break;
            case ID:
                while (isalpha(sbl) || ((sbl >= '0') && (sbl <= '9')) || (sbl == '_')) 
                {
                    temp += sbl;
                    sbl = f.get();
                }
                if (is_kword(temp)) tl.add_token("KWORD", temp);
                else tl.add_token("IDENT", temp);
                temp = "";
                CS = H;
                break;
            case NM:
                while ((sbl >= '0') && (sbl <= '9') || (sbl == '.') || (sbl == '+') || (sbl == '-') || (sbl == 'E') || (sbl == 'e')) 
                {
                    temp += sbl;
                    sbl = f.get();
                }
                while (isalpha(sbl))
                {
                    temp += sbl;
                    sbl = f.get();
                }
                if (is_num(temp)) 
                {
                    tl.add_token("NUM", temp);
                    CS = H;
                }
                else if (temp == "+") 
                {
                    sbl = '+';
                    CS = DLM;
                }
                else if (temp == "-") 
                {
                    sbl = '-';
                    CS = DLM;
                }
                else {
                    CS = ERR;
                    error_symbol = temp;
                }
                temp = "";
                break;
            case ASGN:
                sbl = f.get();
                if (sbl == '=') 
                {
                    tl.add_token("OPER", ":=");
                    CS = H;
                    sbl = f.get();
                }
                else
                {
                    error_symbol = sbl;
                    CS = ERR;
                }
                break;
            case DLM:
                if (sbl == '/') 
                {
                    while (sbl != ' ') 
                    {
                        temp += sbl;
                        sbl = f.get();
                    }
                    if (temp == "/") 
                    {
                        sbl = '/';
                    }
                    else 
                    {
                        error_symbol = temp[0];
                        CS = ERR;
                        temp = "";
                        break;
                    }
                }
                if (sbl == '*') {
                    while (sbl != ' ') 
                    {
                        temp += sbl;
                        sbl = f.get();
                    }
                    if (temp == "*") 
                    {
                        sbl = '*';
                    }
                    else 
                    {
                        error_symbol = temp[0];
                        CS = ERR;
                        temp = "";
                        break;
                    }
                }
                if (sbl == ')' || sbl == '(' || sbl == ';') 
                {
                    tl.add_token("DELIM", string(1, sbl));
                    CS = H;
                }
                else if ((sbl == '<') || (sbl == '>') || (sbl == '=') || (sbl == '+') || (sbl == '-') || (sbl == '/') || (sbl == '*')) 
                {
                    tl.add_token("OPER", string(1, sbl));
                    CS = H;
                }
                else 
                {
                    error_symbol = sbl;
                    CS = ERR;
                }
                temp = "";
                sbl = f.get();
                break;
            case ERR:
                cout << "\nUnknown character: " << error_symbol << "\n";
                CS = H;
                break;
            default:
                break;
            }
        }
    }
    else cout << "unable to open file " << path;
    f.close();
    tl.print();
    return 0;
}

int main()
{
    // test.txt
    // test2.txt
    string path;
    cin >> path;
    lexer(path);
    return 0;
}
