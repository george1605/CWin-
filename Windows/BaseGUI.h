#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#define EVT_EXT ".evpr"
using namespace std;
class Process {
public:
	std::string Name = "*.exe";
	map<string, string> StartInfo;
	int ID;
};
class Event {
public:
	Process Proc;
    int Code;
	string Type;
	Event(string Type,string Name) {

	}
};
queue<Event> Events;

void SendEvt(Event k,ofstream& t) {
	string j = "Process(Code=" + to_string(k.Proc.ID) + ",Name=" + k.Proc.Name + ")\n   Event(Code=" + to_string(k.Code) + ");";
	t << j;
}

void AppendEvt() {

}

void ClearEvts() {
	while (!Events.empty()) {
		Events.pop();
	}
}

void ReadEvts(string filename) {

}