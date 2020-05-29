/**
 * Author : Baykam Say
 * ID: 21802030
 * Section : 3
 * Assignment : 5
 */

#ifndef __FRIENDNET_H
#define __FRIENDNET_H

#include <string>
using namespace std;

class FriendNet {

public:
    FriendNet(const string fileName); // constructor
    FriendNet(const FriendNet& aNet); // copy constructor
    ~FriendNet(); // destructor

    void listFriends(const string personName, const int hopNo);
        
    void displayAverageDegrees();
    void displayDiameters();

private:
    // define your data members here
    struct FriendNode {
        int id = -1;
        FriendNode* next = NULL;
    };
    struct Person {
        string name = "";
        FriendNode* next = NULL;
    };
    int size;
    Person** people;
    // define private class functions here, if you have any
    int findPerson(const string personName);
    void listFriends(const int id, const int hopNo, int level, bool* visits);
    int findComponent(const int id);
    // YOU HAVE TO USE THE ADJACENCY LIST REPRESENTATION
};

#endif // !__FRIENDNET_H
