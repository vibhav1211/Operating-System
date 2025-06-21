#include "graph.h"

extern FILE* userActivity;
extern FILE* readActivity; 

const char* getActionName(ActionType action); 
extern vector<User*> user; 
mutex printing_lock; // because multiple read threads will try to write in readActivity.log file 

FILE* initLogFile(const char* filename) {
    // Step 1: clear the file
    FILE* f = fopen(filename, "w");
    if (f) fclose(f);

    // Step 2: open in append mode
    return fopen(filename, "a");
}

void logUserActivity(Action* action){
    string temp = "USER "; 
    temp += to_string(action->m_userId); 
    temp+=" created "; 
    temp+= getActionName(action->m_action); 
    temp+="("; 
    temp+=to_string(action->m_actionId); 
    temp+=") action at time: ";
    temp+=to_string(action->m_timestamp); 
    temp+="\n";
    fprintf(userActivity, "%s", temp.c_str()); 
    fflush(userActivity); 
    
}


void logReadActivity(int reading_userID, Action* action){
    string temp = "USER "; 
    temp += to_string(reading_userID); 
    temp+=" read "; 
    temp+= getActionName(action->m_action); 
    temp+="("; 
    temp+=to_string(action->m_actionId); 
    temp+=") action created by USER ";
    temp += to_string(action->m_userId);
    temp += " at time: ";
    temp+=to_string(action->m_timestamp); 
    temp+="\n";

    unique_lock<mutex> ul(printing_lock); 
    fprintf(readActivity, "%s", temp.c_str()); 
    fflush(readActivity); 
    // User x read POST(2) action created by USER y at time t: 
}
// user 1 created POST(2) action at time: 