#include <stdlib.h>
#include <windows.h>
#include <iostream>

using namespace std;

#define MAX_PROCESSES 10

HANDLE process_handles[MAX_PROCESSES];

void make_process(int priority, int pnumber) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    char cmd[] = "child";
    if (!CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, FALSE,
                       CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
        cout <<"Blad podczas tworzenia procesu.\n";
        exit(1);
    }

    if (!SetPriorityClass(pi.hProcess, priority)) {
        cout <<"Blad podczas zmiany priorytetu procesu.\n";
        exit(1);
    }
    process_handles[pnumber] = pi.hProcess;
}

int main() {

    char command;
    bool quit = false;

    while(cin >> command && !quit){
        int priority;
        int pnumber = 0;

        switch (command){
            case 'm':
                if (pnumber >= MAX_PROCESSES) {
                    cout << "Za duzo procesow!";
                    break;
                }
                cout << "Podaj priorytet procesu:";
                cin >> priority;

                make_process(priority, pnumber);
                pnumber++;
                break;
            case 'c':
                int process;

                cout << "Podaj numer procesu:";
                cin >> process;

                if (process < 0 || process >= pnumber) {
                    cout << "Nie ma takiego procesu!\n";
                }
                else {
                    TerminateProcess(process_handles[process], 0);
                    CloseHandle(process_handles[process]);
                    process_handles[process] = nullptr;
                }
                break;
            case 'q':
                quit = true;
                break;
            default:
                cout << "Nie ma takiej komendy!\n";
        }
    }

    return 0;
}
