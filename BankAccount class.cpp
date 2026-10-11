#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
using namespace std;

class BankAccount{
    private:

    int _balance;
    static int _total_accounts;

    public:

    BankAccount(int initial_balance){
        if(initial_balance < 0){
            throw invalid_argument("Balance cannot be negative");
        }
        else{
            _balance = initial_balance;
            _total_accounts += 1;
        }
    }

    void deposit(int amount){
        if(amount <=0){
            throw invalid_argument("Deposit amount must be positive");
        }
        else{
            _balance += amount;
        }
    }

    void withdraw(int amount){
        if(amount <= 0 || amount > _balance){
            throw invalid_argument("Invalid withdrawal amount");
        }
        else{
            _balance -= amount;
        }
    }

    int get_balance () const {return _balance;}

    static int get_total_accounts(){
        return _total_accounts;
    }
};

int BankAccount::_total_accounts = 0;

int main()
{
    vector<string> commands  = {};
    vector<int> argumets = {};
    int init_deposit;
    cin >> init_deposit;
    cin.ignore();
    string operational_line;
    getline(cin,operational_line);
    stringstream ss(operational_line);
    string token;
    while (getline(ss, token,',')){
        stringstream token_slicer(token);
        string part_of_token;
        while(getline(token_slicer,part_of_token,':')){
            if(part_of_token == "deposit" || part_of_token == "withdraw"){
                commands.push_back(part_of_token); 
            }
            else{
                int temp = stoi(part_of_token); //временная переменная, потому что push_back не хочет работать просто так
                argumets.push_back(temp);  
            }   
            }
        }
    
    BankAccount bob(init_deposit);
    for(int idx = 0; idx < commands.size(); ++idx){
        if(commands[idx] == "withdraw"){
            bob.withdraw(argumets[idx]);
        }
        else if(commands[idx]=="deposit"){
            bob.deposit(argumets[idx]);
        }
    }
    cout << bob.get_balance() << endl;
    cout << bob.get_total_accounts() << endl;
    return 0;
}
