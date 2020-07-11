#include <iostream>
using namespace std;
int main(){
    int n;
    cin >> n;
    for (int i = 5; i > 0; i--){
        if (n % i == 0){
            n = n/i;
        } else{
            continue;
        }
    }
    cout << n << endl;
    return 0;
}