/**
 * Author : Baykam Say
 * ID: 21802030
 * Section : 3
 * Assignment : 5
 */

using namespace std;
#include <iostream>
#include <fstream>
#include "FriendNet.h"

FriendNet::FriendNet(const string fileName) {
    fstream input(fileName);

    if (input.is_open()) {
        input >> size;
        people = new Person * [size];

        for (int i = 0; i < size; i++) {
            int id;
            input >> id;
            people[id] = new Person();
            input >> people[id]->name;
            int numOfFriends;
            input >> numOfFriends;

            if (numOfFriends) {
                people[id]->next = new FriendNode();
                FriendNode* cur = people[id]->next;
                input >> cur->id;

                for (int j = 1; j < numOfFriends; j++) {
                    cur->next = new FriendNode();
                    cur = cur->next;
                    input >> cur->id;
                }
            }
        }
    }
    else {
        size = 0;
        people = NULL;
    }
}

FriendNet::FriendNet(const FriendNet& aNet) {
    size = aNet.size;

    if (size) {
        people = new Person * [size];
        
        for (int i = 0; i < size; i++) {
            people[i] = new Person();
            people[i]->name = aNet.people[i]->name;

            if (aNet.people[i]->next != NULL) {
                people[i]->next = new FriendNode();
                FriendNode* cur = people[i]->next;
                FriendNode* aNetCur = aNet.people[i]->next;
                cur->id = aNetCur->id;

                while (aNetCur->next != NULL) {
                    cur->next = new FriendNode();
                    cur = cur->next;
                    aNetCur = aNetCur->next;
                    cur->id = aNetCur->id;
                }
            }
        }
    }
    else {
        people = NULL;
    }
}

FriendNet::~FriendNet() {
    for (int i = 0; i < size; i++) {
        if (people[i]->next != NULL) {
            FriendNode* cur = people[i]->next;

            while (cur->next != NULL) {
                FriendNode* temp = cur;
                cur = cur->next;
                delete temp;
            }
            delete cur;
        }
        delete people[i];
    }
    delete[] people;
}

void FriendNet::listFriends(const string personName, const int hopNo) {
    int id = findPerson(personName);
    if (id == -1)
        cout << personName << " does not exist in the network.";
    else {
        cout << "People accessible from " << personName << " within " << hopNo << " hops: ";

        if (hopNo > 0 && people[id]->next != NULL) {
            bool* visits = new bool[size];

            for (int i = 0; i < size; i++) {
                visits[i] = false;
            }
            listFriends(id, hopNo, 0, visits);
            visits[id] = false;
            for (int i = 0; i < size; i++) {
                if (visits[i])
                    cout << ", " << people[i]->name;
            }
        }
        else {
            cout << "NOBODY";
        }
    }
    cout << endl;
}

int FriendNet::findPerson(const string personName) {
    for (int i = 0; i < size; i++) {
        if (personName == people[i]->name) {
            return i;
        }
    }
    return -1;
}

void FriendNet::listFriends(const int id, const int hopNo, int level, bool* visits) {
    visits[id] = true;
    cout << id << " ";
    if (level == hopNo)
        return;
    
    if (people[id]->next != NULL) { // allow visits if their actual level is smaller by making the array an int
        
        FriendNode* cur = people[id]->next;
        if (!visits[cur->id])
            listFriends(cur->id, hopNo, level + 1, visits);
        while (cur->next != NULL) {
            cur = cur->next;
            if (!visits[cur->id])
                listFriends(cur->id, hopNo, level + 1, visits);
        }
    }
}