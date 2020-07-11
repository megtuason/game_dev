#include <iostream>
#include <cstring>
using namespace std;

int main(){
    int N;
    int M;
    string input;

    cout << "Enter number of Rows: ";
    cin >> N;
    cout << "Enter number of Columns: ";
    cin >> M;

    string grid[N][M];

    cout << "Enter string input: ";
    cin >> input;
    
    int i = 0;
    bool help = true;
    while(help){
        for(int r=0; r < N; r++){
            for (int c=0; c < N; c++){
                if (i == input.length()){
                    help = false;
                    break;
                } else{
                    grid[r][c] = input[i];
                    i++;
                }
            }
        }
    }
    cout << "==========" <<endl;

    for(int r=0; r < N; r++){
        for (int c=0; c < M; c++){
            cout << grid[r][c];
        }
        cout << endl;
    }

    cout << "==========" <<endl;
    
    return 0;
}