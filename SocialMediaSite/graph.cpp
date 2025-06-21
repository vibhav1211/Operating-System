#include "graph.h"
using namespace std; 

void logUserActivity(Action* action);

const char* actionName[3] = {"POST", "COMMENT", "LIKE"}; 
const char* getActionName(ActionType action){
    return actionName[action]; 
}

// ACTION 
Action::Action(){}
Action::Action(int userId, int actionId, ActionType action, uint64_t timestamp): 
        m_userId{userId}, 
        m_actionId{actionId}, 
        m_action{action}, 
        m_timestamp{timestamp}
    {
    }


void Action::printAction(){
        
    }

int Action::getUserId(){
        return m_userId; 
    }


// GLOBAL ACTION QUEUE
GlobalActionQueue::GlobalActionQueue(){}
GlobalActionQueue::GlobalActionQueue(int size):
    mx_size{size}
    {
        queue.resize(size); 
    }

void GlobalActionQueue::pushAction(Action* action){
    unique_lock<mutex> ul(this->mt); 
    while(this->cnt >= this->mx_size){
        // wait 
        this->cv.wait(ul); 
    }
    this->cv.notify_all(); 
    ul.unlock(); 

    this->queue[this->head] = action; 
    this->head = (this->head + 1)%this->mx_size;

    ul.lock(); 
    this->cnt++; 
    this->cv.notify_all(); 
    ul.unlock(); 
    // action->printAction(); 
}

Action* GlobalActionQueue::popAction(){
    unique_lock<mutex> ul(this->mt); 
    while(this->cnt<=0){
        // wait
        this->cv.wait(ul);
    }
    Action* temp = this->queue[this->tail]; 
    this->tail = (this->tail + 1)%this->mx_size; 
    this->cnt--; 

    this->cv.notify_all(); 
    ul.unlock(); 
    return temp; 
}

// GLOBAL FEED QUEUE

GlobalFeedQueue::GlobalFeedQueue(){}
GlobalFeedQueue::GlobalFeedQueue(int size):
    mx_size{size}
    {
        queue.resize(size); 
    }

void GlobalFeedQueue::pushFeedNode(int node){
    unique_lock<mutex> ul(this->mt);
    while(this->cnt==this->mx_size){
        // wait
        cv.wait(ul); 
    }
    this->queue[head] = node; 
    head = (head +1)%this->mx_size; 

    this->cnt++; 
    cv.notify_all(); 
    ul.unlock(); 

}

int GlobalFeedQueue::popFeedNode(){
    unique_lock<mutex> ul(this->mt); 
    while(this->cnt==0){
        // wait
        cv.wait(ul); 
    }
    int temp = this->queue[tail]; 
    tail =(tail+1)%this->mx_size; 
    this->cnt--;

    cv.notify_all(); 
    ul.unlock(); 
    return temp; 
}



// USER

User::User(){}
User::User(int userId, Priority priority):
        m_userId{userId}, 
        m_priority{priority}
    {
        m_feedQueue.resize(m_maxfeedQueueSize); 
    }

void User::addFriend(User * user){
        this->friends.push_back(user); 
    }

void User::pushIntoWallQueue(Action* action){
        this->m_wallQueue.push_back(action); 
        logUserActivity(action); 
    }

void User::pushIntoFeedQueue(Action* action){
    unique_lock<mutex> ul(this->mt); 
    while(this->feedQueueCurrentSize==this->m_maxfeedQueueSize){
        // wait
        cv.wait(ul); 
    }
    m_feedQueue[head] = action; 
    head = (head +1 )%this->m_maxfeedQueueSize; 
    this->feedQueueCurrentSize++; 

    cv.notify_all(); 
    ul.unlock(); 
}

Action* User::popFromFeedQueue(){
    unique_lock<mutex> ul(this->mt); 
    while(this->feedQueueCurrentSize==0){
        // wait
        cv.wait(ul); 
    }
    Action* temp = m_feedQueue[tail]; 
    tail = (tail + 1)%this->m_maxfeedQueueSize; 
    this->feedQueueCurrentSize--; 

    cv.notify_all(); 
    ul.unlock(); 

    return temp; 
}

int User::numOfFriends(){
        return friends.size(); 
    }

int User::getNewActionId(){
    return (this->m_wallQueue.size()) +1; 
} 

int User::getUserId(){
    return m_userId;
}