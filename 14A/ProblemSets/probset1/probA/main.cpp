#include <iostream>
#include <cstring>
using namespace std; 
int main(){
    char input[1000];
    cin.getline(input,sizeof(input));

    string message;
    for (int i = 0; i < strlen(input); i++){
        if (input[i] == '^'){
            input[i+1] = toupper(input[i+1]);
            continue;
        } else if (input[i] == '<'){
            message.pop_back();
            continue;
        }
        message += input[i];
    }
    cout << endl << message << endl;
}