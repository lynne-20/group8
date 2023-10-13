#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>

using namespace std;

map<char, vector<string>> cfg;
map<char, set<char>> first;
map<char, set<char>> follow;
map<char, map<char, string>> ll1ParsingTable;


bool isTerminal(char symbol) {
    return islower(symbol) || symbol == '$';
}

// Calculating the first()
void calFirst(char nonTerminal) {
    if (first.count(nonTerminal) > 0) {
        return;
    }

    for (const string& rule : cfg[nonTerminal]) {
        char firstSymbol = rule[0];
        if (isTerminal(firstSymbol)) {
            first[nonTerminal].insert(firstSymbol);
        } else {
            calFirst(firstSymbol);
            first[nonTerminal].insert(first[firstSymbol].begin(), first[firstSymbol].end());
        }
    }
}

// Calculating the follow for non-terminals
void calFollowNonTerm(char nonTerminal) {
    set<char> temp;
    for (const auto& entry : cfg) {
        char A = entry.first;
        for (const string& rule : entry.second) {
            for (size_t i = 0; i < rule.length(); ++i) {
                char B = rule[i];
                if (B == nonTerminal) {
                    for (size_t j = i + 1; j < rule.length(); ++j) {
                        char beta = rule[j];
                        calFirst(beta);
                        temp.insert(first[beta].begin(), first[beta].end());
                        if (first[beta].count('$') == 0) {
                            temp.erase('$');
                            break;
                        }
                    }
                    if (i == rule.length() - 1 || (i < rule.length() - 1 && first[rule[i + 1]].count('$') > 0)) {
                        temp.insert(follow[A].begin(), follow[A].end());
                    }
                }
            }
        }
    }

    follow[nonTerminal].insert(temp.begin(), temp.end());
}


// Calculating the follow()
void calFollow() {
    follow[cfg.begin()->first].insert('$');

    for (const auto& entry : cfg) {
        char nonTerminal = entry.first;
        calFollowNonTerm(nonTerminal);
    }

}

// Constructing the LL1 Parsing table
void LL1ParsingTable() {
    for (const auto& entry : cfg) {
        char nonTerminal = entry.first;
        for (const string& rule : entry.second) {
            calFirst(nonTerminal);
            for (char terminal : first[nonTerminal]) {
                if (terminal != '$') {
                    ll1ParsingTable[nonTerminal][terminal] = rule;
                }
            }
            if (first[nonTerminal].count('$') > 0) {
                if (nonTerminal != 'S') { // Exclude the start symbol
                    calFollowNonTerm(nonTerminal);
                    for (char terminal : follow[nonTerminal]) {
                        ll1ParsingTable[nonTerminal][terminal] = rule;
                    }
                }
            }
        }
    }
}

int main() {
//Displaying the CFG
    cout << "CFG:" << endl;
    cout<<"A->aBD" << endl;
    cout<<"B->b" << endl;
    cout<<"C->g" << endl;
    cout<<"D->dD|e" << endl;
    cout<<"S->A" << endl;
    cout << "" << endl;

    cfg['A'] = {"a B D"};
    cfg['B'] = {"b"};
    cfg['C'] = {"g"};
    cfg['D'] = {"d D", "e"};
    cfg['S'] = {"A"};



    LL1ParsingTable();


    cout << "FIRST:" << endl;
    for (const auto& entry : first) {
        cout << entry.first << " = { ";
        for (char symbol : entry.second) {
            cout << symbol << ",";
        }
        cout << "}" << endl;
    }


    calFollow();
    cout << "\nFOLLOW:" << endl;
    for (const auto& entry : follow) {
        cout << entry.first << " = { ";
        for (char symbol : entry.second) {
            cout << symbol << ",";
        }
        cout << "}" << endl;
    }

    cout << "\nLL(1) Parsing Table:" << endl;
    for (const auto& row : ll1ParsingTable) {
        char nonTerminal = row.first;
        for (const auto& entry : row.second) {
            char terminal = entry.first;
            string production = entry.second;
            cout << "[" << nonTerminal << ", " << terminal << "] = " << nonTerminal << " -> " << production << endl;
        }
    }

    return 0;
}
