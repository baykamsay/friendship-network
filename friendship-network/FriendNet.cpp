/**
 * Author : Baykam Say
 * ID: 21802030
 * Section : 3
 * Assignment : 5
 */

using namespace std;
#include <iostream>
#include <iomanip>
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
            bool first = true;

            for (int i = 0; i < size; i++) {
                if (visits[i]) {
                    if (first)
                        first = false;
                    else
                        cout << ", ";
                    cout << people[i]->name;
                }
            }
            delete[] visits;
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
    
    if (level == hopNo)
        return;
    
    if (people[id]->next != NULL) { // allow visits if their actual level is smaller by making the array an int
        
        FriendNode* cur = people[id]->next;
        if (!visits[cur->id] || level < hopNo)
            listFriends(cur->id, hopNo, level + 1, visits);
        while (cur->next != NULL) {
            cur = cur->next;
            if (!visits[cur->id] || level < hopNo)
                listFriends(cur->id, hopNo, level + 1, visits);
        }
    }
}

void FriendNet::displayAverageDegrees() {
    int numOfComponents = 0;
    int* components = new int[size];
    int* degrees = new int[size];

    for (int i = 0; i < size; i++) {
        components[i] = -1;
    }
    for (int i = 0; i < size; i++) {
        degrees[i] = 0;
    }

    for (int i = 0; i < size; i++) {
        if (components[i] == -1) {
            findComponents(i, components, numOfComponents);
            numOfComponents++;
        }
    }

    for (int i = 0; i < size; i++) {
        for (FriendNode* cur = people[i]->next; cur != NULL; cur = cur->next) {
            degrees[i]++;
        }
    }

    double* avgDegrees = new double[numOfComponents];

    for (int i = 0; i < numOfComponents; i++) {
        int sum = 0;
        int numOfVertices = 0;

        for (int j = 0; j < size; j++) {
            if (components[j] == i) {
                sum += degrees[j];
                numOfVertices++;
            }
        }
        avgDegrees[i] = (double) sum / numOfVertices;
    }

    cout << "There are " << numOfComponents << " connected components. The average degrees are:" << endl;
    cout << fixed << setprecision(2);
    for (int i = 0; i < numOfComponents; i++) {
        cout << "For component " << i << ": " << avgDegrees[i] << endl;
    }

    delete[] components;
    delete[] degrees;
    delete[] avgDegrees;
}

// DFT
void FriendNet::findComponents(const int id, int* components, int componentNo) {
    components[id] = componentNo;

    for (FriendNode* cur = people[id]->next; cur != NULL; cur = cur->next) {
        if (components[cur->id] == -1)
            findComponents(cur->id, components, componentNo);
    }
}

void FriendNet::displayDiameters() {
    int numOfComponents = 0;
    int* components = new int[size];
    int* dist = new int[size];

    for (int i = 0; i < size; i++) {
        components[i] = -1;
    }

    for (int i = 0; i < size; i++) {

        if (components[i] == -1) {

            // first iteration, find the furthest from random friend
            components[i] = numOfComponents;
            FriendNode* head = new FriendNode();
            head->id = i;
            dist[i] = 0;

            while (head != NULL) {
                FriendNode* temp;
                temp = head;
                head = head->next;
                
                for (FriendNode* cur = people[temp->id]->next; cur != NULL; cur = cur->next) {
                    if (components[cur->id] == -1) {
                        components[cur->id] = numOfComponents;
                        dist[cur->id] = dist[temp->id] + 1;

                        if (head == NULL) {
                            head = new FriendNode();
                            head->id = cur->id;
                        }
                        else {
                            FriendNode* iter = head;

                            while (iter->next != NULL) {
                                iter = iter->next;
                            }
                            iter->next = new FriendNode();
                            iter->next->id = cur->id;
                        }
                    }
                }
                delete temp;
            }
            delete head;
            
            // second iteration, find the furthest from the furthest item
            int furthest  = i;

            for (int j = 0; j < size; j++) {
                if (components[j] == numOfComponents && dist[j] > dist[furthest]) {
                    furthest = j;
                }
            }

            bool* visited = new bool[size];

            for (int i = 0; i < size; i++) {
                visited[i] = false;
            }
            
            head = new FriendNode();
            head->id = furthest;
            dist[furthest] = 0;
            visited[furthest] = true;
            
            while (head != NULL) {
                FriendNode* temp;
                temp = head;
                head = head->next;

                for (FriendNode* cur = people[temp->id]->next; cur != NULL; cur = cur->next) {
                    if (!visited[cur->id]) {
                        visited[cur->id] = true;
                        dist[cur->id] = dist[temp->id] + 1;

                        if (head == NULL) {
                            head = new FriendNode();
                            head->id = cur->id;
                        }
                        else {
                            FriendNode* iter = head;

                            while (iter->next != NULL) {
                                iter = iter->next;
                            }
                            iter->next = new FriendNode();
                            iter->next->id = cur->id;
                        }
                    }
                }
                delete temp;
            }
            delete head;
            delete[] visited;
            numOfComponents++;
        }
    }
    int* diameters = new int[numOfComponents];

    for (int i = 0; i < numOfComponents; i++) {
        diameters[i] = 0;
    }

    for (int i = 0; i < numOfComponents; i++) {
        for (int j = 0; j < size; j++) {
            if (components[j] == i && dist[j] > diameters[i]) {
                diameters[i] = dist[j];
            }
        }
    }

    cout << "There are " << numOfComponents << " connected components. The diameters are:" << endl;
    for (int i = 0; i < numOfComponents; i++) {
        cout << "For component " << i << ": " << diameters[i] << endl;
    }

    delete[] components;
    delete[] dist;
    delete[] diameters;
}