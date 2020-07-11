#include <iostream>
using namespace std;

int checkSum(int num){
    int sum = 0;
    while(num != 0){
        sum = sum + num % 10;
        num = num / 10;
    }
    if (sum % 7 == 0){
        return 1;
    } else {
        return 0;
    }
}

int newNum(int n, int q){
    int count = 1;
    string state;
    while(state != "pass!"){
        n = 7*(q+count);
        if (n % 10 == 7 && n % 7 == 0 && checkSum(n) == 1){
            state = "pass!";
        } else {
            count++;
        }
    }
    return n;
}

int main(){
    int input;
    cin >> input;
    int q = input/7;
    int n = 7*q;
    if (n % 10 == 7 && n % 7 == 0 && checkSum(n) == 1){
        if (n < input){
            cout << "Passcode: " << newNum(n, q) << endl;
            return 0;
        }
        cout << "Passcode: " << n << endl;
    } else {
        cout << "Passcode: " << newNum(n, q) << endl;
    }
    return 0;
}