#include <iostream>
#include <sstream>
#include <string>
using namespace std;

int main(){

    int l;
    int w;
    int n;

    cout << "Enter grid settings: " << endl;

    for (int i = 0; i < 3; i++){
        if (i == 0){
            cin >> l;
        } else if (i == 1){
            cin >> w;
        } else {
            cin >> n;
        }
    }

    if (l < 3 || w > 5 || n < 1 || n > 10){
        cout << "Invalid Input!" << endl;
    } else {
        int grid[l][w];
        cout << "Enter grid numbers:" << endl;
        for (int i = 0; i < l; i++){
            for (int j = 0; j < w; j++){
                cin >> grid[i][j];
            }
        }

        int target[2];
        int count = 0;
        while (count != n){
            for(int i = 0; i < 2; i++){
                if (i == 0){
                    cin >> target[0];
                }
                else if (i == 1){
                    cin >> target[1];
                }
            }
            cout << "TARGET: " << target[0] << target[1] << endl;
            
            if (grid[target[0]][target[1]] == 0){
                grid[target[0]][target[1]] = 1;
            } else{
                grid[target[0]][target[1]] = 0;
            }

            for (int i = 0; i < l; i++){
                for (int j = 0; j < w; j++){
                    cout << grid[i][j] << " ";
                }
                cout << endl;
            }
            count++;
        }
    }

    return 0;
}