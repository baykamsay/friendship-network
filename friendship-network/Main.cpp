/**
 * Author : Baykam Say
 * ID: 21802030
 * Section : 3
 * Assignment : 5
 */

using namespace std;
#include <iostream>
#include "FriendNet.h"

int main() {
    FriendNet F("input.txt");
    F.listFriends("Selim", 2);
    F.listFriends("Funda", 1);
    F.listFriends("Cigdem", -1);
    cout << endl;
    F.listFriends("Ibrahim", 2);
    F.listFriends("Ibrahim", 3);
    cout << endl;
    return 0;
}