#include "graph.h"

extern GlobalActionQueue* gaq; 
extern GlobalFeedQueue* gfq; 
extern vector<User*> user; 

void pushUpdate(){
    while(1){
        // fetch an action from global action queue and put it into neighbours of the corrosponding userID 
        Action * action = gaq->popAction(); 
        int userID = action->getUserId(); 
        User* ptr = user[userID]; 
        for(auto _friend: ptr->friends){
            _friend->pushIntoFeedQueue(action); 
            gfq->pushFeedNode(_friend->getUserId()); 
        }
    }
}