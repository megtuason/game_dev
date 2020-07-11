#include <iostream>
using namespace std;

int printMap(int size, int playerPos[], int targetPos[]){
    string pos[size][size];
    string map;
    string borderTop;
    int *p;
    int *t;
    p = playerPos;
    t = targetPos;

    for(int i = 0; i < size; i++){
        borderTop += "----";
    }
    borderTop += "-\n";
    map = borderTop;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if (i == *p && j == *(p+1)){
                pos[i][j] = "o";
            } else if (i == *t && j == *(t+1)){
                pos[i][j] = "x";
            } else {
                pos[i][j] = " ";
            }
            map += ("| " + pos[i][j] + " ");
        }
        map += ("|\n" + borderTop);
    }
    cout << map << endl;
    if (*p == *t && *(p+1) == *(t+1)){
        return 1;
    } else{
        return 0;
    }
}

int* movePlayer(int size, int playerPos[], string direction){
    int *p;
    p = playerPos;
    
    if (direction == "north"){
        if (*p == 0){
            *p = (size-1);
        } else {
            *p = *p-1;
        }
    } else if (direction == "south"){
        if (*p == (size-1)){
            *p = 0;
        } else {
            *p = *p+1;
        }
    } else if (direction == "east"){
        if (*(p+1) == (size-1)){
            *(p+1) = 0;
        } else {
            *(p+1) = *(p+1)+1;
        }
    } else if (direction == "west"){
        if (*(p+1) == 0){
            *(p+1) = (size-1);
        } else {
            *(p+1) = *(p+1)-1;
        }
    }
    return playerPos;
}

void editMap(int size){
    string map = "    ";
    string borderTop = "  ";

    for(int i = 0; i < size; i++){
        map += (to_string(i) + "   ");
        borderTop += "----";
    }
    borderTop += "-\n";
    map += "\n";
    map += borderTop;

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            if (j == 0){
                map += to_string(i);
            }
            map += (" |  ");
        }
        map += (" |\n" + borderTop);
    }
    cout << map << endl;
}

int main() {
    int worldSize = 3;
    int playerPos[2] = {2, 2};
    int targetPos[2] = {1, 1};

    int choice;
    while (choice != 3){
        cout << "====================\n1 Start Game\n2 Configure Settings\n3 Exit" << endl;
        cin >> choice;
        if (choice == 1){
            string move;
            cout << "===================\n" << "o --> Player\nx --> Target" << endl;
            cout << "Type 'exit' to leave the game.\n" << endl;
            printMap(worldSize, playerPos, targetPos);
            while (move != "exit"){
                cout << "DIRECTIONS: north, south, east, west" << endl;
                cout << "Type a direction to move: ";
                string previous = move;
                cin >> move;
                if (move != "north" && move != "south" && move != "east" && move != "west" && move != "exit"){
                    cout << "Invalid input! " << move << endl;
                } else{
                    if (printMap(worldSize, movePlayer(worldSize, playerPos, move), targetPos) == 1){
                        cout << "You won!" << endl;
                        break;
                    }
                }
            }
            playerPos[0] = 2;
            playerPos[1] = 2;
            targetPos[0] = 1;
            targetPos[0] = 1;
        } else if (choice == 2){
            cout << "Set World Size: ";
            cin >> worldSize;
            editMap(worldSize);
            cout << "Set Player ROW Position: ";
            cin >> playerPos[0];
            cout << "Set Player COLUMN Position: ";
            cin >> playerPos[1];
            cout << "Set Target ROW Position: ";
            cin >> targetPos[0];
            cout << "Set Target COLUMN Position: ";
            cin >> targetPos[1];
            cout << "Game settings updated." << endl;

        } else if (choice == 3){
            cout << "Good bye!" << endl;
        } else{
            cout << "Invalid input!" << endl;
        }
    }
    return 0;
}