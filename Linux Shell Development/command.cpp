#include "./command.hpp"
#include "./utils.hpp"

#include <unistd.h>
#include <fcntl.h>
using namespace std;


Command::Command(const string & __str): command(__str), infd(STDIN_FILENO), ofd(STDOUT_FILENO){
    parse();
}
Command::~Command(){
    if(infd!=STDIN_FILENO){
        close(infd);
    }
    if(ofd != STDOUT_FILENO){
        close(ofd);
    }
}
void Command::parse(){
    bool output = (command.find('>')!=string::npos);
    bool input = (command.find('<')!=string::npos);
    vector<string> tokens(3);

    if(input && output){
        int pos1 = command.find('<');
        int pos2 = command.find('>');
        tokens[0] = command.substr(0, pos1);
        tokens[1] = command.substr(pos1+1, pos2-pos1-1);
        tokens[2] = command.substr(pos2+1);
    }
    else if(input){
        int pos = command.find('<');
        tokens[0] = command.substr(0, pos);
        tokens[1] = command.substr(pos+1);
        tokens[2] = "";
    }
    else if(output){
        int pos = command.find('>');
        tokens[0] = command.substr(0, pos);
        tokens[1] = "";
        tokens[2] = command.substr(pos+1);
    }
    else{
        tokens[0] = command;
        tokens[1] = "";
        tokens[2] = "";
    }
    trim(tokens[0]);
    trim(tokens[1]);
    trim(tokens[2]);
    infile = tokens[1];
    ofile = tokens[2];
    args = parseArgs(tokens[0], ' ');
}

void Command::set_fd(){
    if(!infile.empty()){
        if((infd = open(infile.c_str(), O_RDONLY)) < 0){
            perror("open() failed");
            exit(1);
        }
        if(dup2(infd, STDIN_FILENO) < 0){
            perror("dup2() failed");
            exit(1);
        }
    }

    if(!ofile.empty()){
        if((ofd = open(ofile.c_str(), O_WRONLY|O_CREAT)) < 0){
            perror("open() failed");
            exit(1);
        }
        if(dup2(ofd, STDOUT_FILENO) < 0){
            perror("dup2() failed");
            exit(1);
        }
    }
}