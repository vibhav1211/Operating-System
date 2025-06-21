#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <string>
#include <set>
#include <cmath>
#include <thread>
#include <fstream>
#include <string>
#include <csignal>
#include "graph.h"
using namespace std; 

extern const int number_of_users=37700; 
int number_of_edges = 0; 

vector<User*> user(number_of_users); 
GlobalActionQueue* gaq; 
GlobalFeedQueue* gfq; 

void userSimulate(); 
void pushUpdate(); 
void readPost();


FILE* userActivity; 
FILE* readActivity; 
FILE* initLogFile(const char* filename); 


int main(){
    
    // initialise the static graph 
    for(int i=0; i<number_of_users; i++){
        user[i] = new User(i, CHRONOLOGICAL); 
    }

    ifstream file("musae_git_edges.csv");
    string line;

    // Skip header if it exists
    getline(file, line); // assumes first line is: "source,target"
    while (getline(file, line)) {
        stringstream ss(line);
        string srcStr, dstStr;

        if (getline(ss, srcStr, ',') && getline(ss, dstStr)) {
            int u = stoi(srcStr);
            int v = stoi(dstStr); 
            // undirected edge from u to v i.e. u is friend of v and v is friend of u 
            user[u]->addFriend(user[v]); 
            user[v]->addFriend(user[u]); 
            number_of_edges++;
        }
    }
    file.close();

    // ORGANISE THIS LATER 
    int sum=0;
    multiset<int> ms; 
    for(int i=0; i<number_of_users; i++){
        int temp = 10*(1 + log2(user[i]->numOfFriends()));
        ms.insert(temp); 
        sum+=(temp); 
        if(ms.size()>100){
            temp = *ms.begin(); 
            sum-=(temp); 
            ms.erase(ms.begin()); 
        }
    }
    cout<<sum<<endl;
    gaq = new GlobalActionQueue(sum+100); 
    gfq = new GlobalFeedQueue(100000); 


    userActivity = initLogFile("userActivity.log"); 
    readActivity = initLogFile("readActivity.log"); 
    // userActivity.open("userActivity.log", std::ios::app); 

    thread user_simulate_thread(userSimulate); 

    vector<thread> push_update_threads; 
    vector<thread> read_post_threads; 

    for(int i=0; i<25; i++){
        push_update_threads.push_back(thread(pushUpdate)); 
    }
    for(int i=0; i<10; i++){
        read_post_threads.push_back(thread(readPost)); 
    }

    user_simulate_thread.join();
    cout<<"userSimulate Joined"<<endl;

    for(int i=0; i<25; i++){
        push_update_threads[i].join(); 
    }
    for(int i=0; i<10; i++){
        read_post_threads[i].join();
    }       

    

}
