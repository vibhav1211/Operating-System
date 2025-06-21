#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm> // for std::shuffle
#include <random> 
#include "graph.h"
using namespace std; 

extern const int number_of_users;
extern vector<User*> user; 
extern GlobalActionQueue * gaq; 

void userSimulate(){
    auto now = std::chrono::system_clock::now();
    std::vector<int> nodes(number_of_users);
    for (int i = 0; i < number_of_users; ++i) {
        nodes[i] = i;
    }
    int itr = 1; 
    while(itr--){
        // Seed random number generator
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(nodes.begin(), nodes.end(), std::default_random_engine(seed));

        vector<int> temp; 
        int tot=0; 
        int tot2 = 0; 
        // select the first 100 nodes 
        for(int i=0; i<100; i++){
            
            int userId = nodes[i]; 
            // cout<<"Node: "<<userId<<" -> "; 
            User * currentUser = user[userId]; 

            int numOfActions = 10*(1 + log2(currentUser->numOfFriends())); 
            tot+= numOfActions;
            temp.push_back(numOfActions); 
            tot2+= numOfActions*currentUser->numOfFriends(); 
            for(int j=0; j<numOfActions; j++){

                int actionId = currentUser->getNewActionId(); 
                ActionType actionType = (ActionType)(rand()%3); 
                uint64_t actionTimestamp = std::chrono::duration_cast<std::chrono::microseconds>(
                                                std::chrono::system_clock::now().time_since_epoch()
                                            ).count();

                Action* act = new Action(userId, actionId, actionType, actionTimestamp); 

                currentUser->pushIntoWallQueue(act); 
                gaq->pushAction(act); 

            }
            

        }

        // cout<<temp.size()<<endl;
        // for(auto vv: temp) cout<<vv<<" "; 
        // cout<<endl; 
        cout<<"Batch completed"<<endl;
        cout<<"Total actions created: "<<tot<<endl;
        cout<<"Total number of required read operations: "<<tot2<<endl;
        // std::this_thread::sleep_for(chrono::seconds(120)); 
    }
}