#include "graph.h"

extern GlobalFeedQueue* gfq; 
extern vector<User*> user; 

void logReadActivity(int reading_userID, Action* action); 


void readPost(){
    while(1){
        int userId = gfq->popFeedNode(); 
        User* _user = user[userId]; 
        Action* action = _user->popFromFeedQueue();
        logReadActivity(userId, action); 
    }
}