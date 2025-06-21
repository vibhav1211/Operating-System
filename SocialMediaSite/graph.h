#include <iostream> 
#include<vector>
#include <mutex>
#include <condition_variable>
using namespace std; 


enum ActionType{
    POST, COMMENT, LIKE 
};



enum Priority{
    CHRONOLOGICAL, PRIORITY 
};


class Action{ 
public: 
    int m_userId; 
    int m_actionId; 
    ActionType m_action; 
    uint64_t m_timestamp; 
    


    Action();
    Action(int userId, int actionId, ActionType action, uint64_t timestamp);
    void printAction();
    int getUserId();
    
};

class GlobalActionQueue{
    vector<Action*> queue; 
    int tail=0,head=0; 
    int mx_size; 
    int cnt=0; 
    mutex mt; 
    condition_variable cv; 

public: 
    
    GlobalActionQueue(); 
    GlobalActionQueue(int size); 

    void pushAction(Action* action); 
    Action* popAction(); 

}; 

class GlobalFeedQueue{
    int tail=0, head=0; 
    int cnt=0; 
    int mx_size; 
    vector<int> queue; 
    mutex mt; 
    condition_variable cv; 

public: 
    GlobalFeedQueue(); 
    GlobalFeedQueue(int size); 
    
    void pushFeedNode(int node); 
    int popFeedNode(); 

}; 

class User{
    
    int m_userId;  
    int m_maxfeedQueueSize = 10500; 
    vector<Action*> m_wallQueue; 
    vector<Action*> m_feedQueue; 
    Priority m_priority; 
    int feedQueueCurrentSize =0; 
    int wallIndex=0;
    int tail=0,head=0; 
    mutex mt; 
    condition_variable  cv;

public: 
    vector<User*> friends; // if A -> B it means A is friend of B

    User();
    User(int userId, Priority priority);
    void addFriend(User * user);
    void pushIntoWallQueue(Action* action);
    void pushIntoFeedQueue(Action* action);
    int getNewActionId(); 
    Action* popFromFeedQueue();
    int numOfFriends();
    int getUserId(); 

}; 