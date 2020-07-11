

#include <iostream>


int main(int argc, const char * argv[]) {
    using namespace std;

    string User;
    string news;
    cin >> User;

    int pos1 = User.find_first_of("abcdefghijklmnopqrstuvwxyz");
    int pos2 = User.find_last_of("abcdefghijklmnopqrstuvwxyz");

    int strRange = pos2 - pos1 + 1;

    cout <<  User.substr(pos1, strRange) << endl;


    
    return 0;
}
