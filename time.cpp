#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>
using namespace std;


int main() {
    int hh,mm,ss;
    char c;
    string user_input;
    cout << "Enter time in format 'HH, MM, SS': ";
    getline(cin, user_input);
    stringstream(user_input) >> hh >> c >> mm >> c >> ss;
    int total_sec = (hh * 3600) + (mm * 60) + ss;
    for (int i = total_sec; i >= 0; i--) {
        int remaining_hours = i / 3600;
        int remaining_minutes = (i / 60) % 60;
        int remaining_seconds = i % 60;
        cout << setfill('0') << setw(2) << remaining_hours << ":" << setw(2) << remaining_minutes << ":" << setw(2) << remaining_seconds << "\r";
        this_thread::sleep_for(chrono::seconds(1));
    }
}


