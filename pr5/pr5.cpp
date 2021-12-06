#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct node 
{
public:
    string Name;
    string A;
    string B;
public:
    node(string name, string a, string b) {
        Name = name;
        A = a;
        B = b;
    }
};

class machine 
{
    vector<string>states_list, alphabet, final_states;
    vector<node*>states;
    string init_state;
public:
    // методы для ввода исходных данных
    void set_states() 
    {
        cout << "Enter set of states:" << endl;
        string ans = "";
        while (ans != ".") 
        {
            cin >> ans;
            if (ans != ".") states_list.emplace_back(ans);
        }
    }
    void set_alphabet() 
    {
        cout << "Enter the input alphabet:" << endl;
        string ans = "";
        while (ans != ".") 
        {
            cin >> ans;
            if (ans != ".") alphabet.emplace_back(ans);
        }
    }
    void set_final_states() 
    {
        cout << "Enter a set of final states:" << endl;
        string ans = "";
        while (ans != ".") 
        {
            cin >> ans;
            if (ans != ".") final_states.emplace_back(ans);
        }
    }
    void set_init_state(string init) { init_state = init; }

    // методы для получения состояния записей в таблице или указателей на них
    node* get_note(string name) 
    {
        for (int i = 0; i < states.size(); i++) 
        {
            if (states[i]->Name == name) 
            {
                return states[i];
            }
        }
        return nullptr;
    }

    bool is_state_exist(string name)
    {
        for (int i = 0; i < states.size(); i++) 
        {
            if (states[i]) {
                if (states[i]->Name == name) 
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool is_final(string name) 
    {
        for (int i = 0; i < final_states.size(); i++) 
        {
            if (name == final_states[i]) return true;
        }
        return false;
    }

    string is_comp(string a, string b) 
    {
        string temp = "";
        if (a == "") return b;
        for (int j = 0; j < b.size(); j++) 
        {
            for (int i = 0; i < a.size(); i++) 
            {
                if (b[j] == a[i]) break;
                if (i == a.size() - 1) temp += b[j];
            }
        }
        return temp;
    }

    bool is_legal(string name) 
    {
        if ((name != init_state) && (!is_final(name))) 
        {
            for (int j = 0; j < states.size(); j++) 
            {
                if (states[j]->Name != name) 
                {
                    if ((states[j]->A == name) || (states[j]->B == name)) 
                    {
                        return true;
                    }
                }
            }
            return false;
        }
        else return true;
    }

    // получение НКА таблицы
    void create_NFA(string transition) 
    {
        node* note = nullptr;
        for (int i = 0; i < transition.size(); i++) 
        {
            if (transition[i] == '(') {
                if (!is_state_exist(string(1, transition[i + 1]))) 
                {
                    if (transition[i + 3] == 'a') 
                    {
                        note = new node(string(1, transition[i + 1]), string(1, transition[i + 5]), "");
                    }
                    else 
                    {
                        note = new node(string(1, transition[i + 1]), "", string(1, transition[i + 5]));
                    }
                    states.emplace_back(note);
                }
                else 
                {
                    if (transition[i + 3] == 'a') 
                    {
                        get_note(string(1, transition[i + 1]))->A += string(1, transition[i + 5]);
                    }
                    else 
                    {
                        get_note(string(1, transition[i + 1]))->B += string(1, transition[i + 5]);
                    }
                }
            }
        }
    }

    // постороение ДКА таблицы
    void rebuild_to_DFA() 
    {
        bool flag = false;
        string temp, a, b;
        for (int i = 0; i < states.size(); i++) 
        {
            if (!is_state_exist(states[i]->A) || !is_state_exist(states[i]->B)) 
            {
                if (!is_state_exist(states[i]->A)) temp = states[i]->A;
                else temp = states[i]->B;
                node* note = new node(temp, "", "");
                states.emplace_back(note);
                for (int j = 0; j < temp.size(); j++) 
                {
                    a = get_note(string(1, temp[j]))->A;
                    b = get_note(string(1, temp[j]))->B;
                    get_note(temp)->A += is_comp(get_note(temp)->A, a);
                    get_note(temp)->B += is_comp(get_note(temp)->B, b);
                    sort((get_note(temp)->A).begin(), (get_note(temp)->A).end());
                    sort((get_note(temp)->B).begin(), (get_note(temp)->B).end());
                }
            }
        }
        for (int i = 0; i < states.size(); i++) 
        {
            if (!is_legal(states[i]->Name)) states.erase(states.begin() + i);
        }
    }

    void result() 
    {
        cout << "DFA:" << endl;
        cout << "Set of states: ";
        for (int i = 0; i < states.size(); i++) 
        {
            cout << states[i]->Name << ' ';
        }
        cout << endl << "Input alphabet: ";
        for (int i = 0; i < alphabet.size(); i++) 
        {
            cout << alphabet[i] << ' ';
        }
        cout << endl << "State-transition function:" << endl;
        for (int i = 0; i < states.size(); i++) 
        {
            for (int j = 0; j < 2; j++) {
                if (j % 2 == 0) cout << "D(" << states[i]->Name << ", " << alphabet[0] << ") = " << states[i]->A << endl;
                else  cout << "D(" << states[i]->Name << ", " << alphabet[1] << ") = " << states[i]->B << endl;
            }
        }
        cout << endl << "Initial states: " << init_state << endl;
        cout << endl << "Final states: ";
        for (int i = 0; i < states.size(); i++) 
        {
            for (int k = 0; k < final_states.size(); k++) 
            {
                if (states[i]->Name.find(final_states[k]) != string::npos) 
                {
                    cout << states[i]->Name << ' ';
                    break;
                }
            }
        }
        cout << endl;
    }
};

int main()
{
    string test = "(1,a,1) (1,a,2) (1,b,3) (2,a,2) (2,b,1) (2,b,3) (3,a,3) (3,b,3)";
    machine work;
    work.set_states();
    work.set_alphabet();
    work.create_NFA(test);
    work.set_init_state("1");
    work.set_final_states();
    work.rebuild_to_DFA();
    work.result();
    return 0;
}
