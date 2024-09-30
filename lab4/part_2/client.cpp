#include <windows.h>
#include <iostream>

const size_t BUFFER_SIZE = 1024;
const std::string PIPE_NAME("\\\\.\\pipe\\lab4");
const char* EXIT_STR = "stop";
HANDLE hPipe;
size_t callback;


void show_menu(){
    std::cout << "Choose parameter:" << std::endl;
    std::cout << "1. Create named pipe and connection" << std::endl;
    std::cout << "2. Reading mode" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Input: ";
}


void CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
    ++callback;
}

void readingMode() {
    std::cout << "Reading.." << std::endl;
    if (hPipe != INVALID_HANDLE_VALUE) {
        OVERLAPPED over;
        size_t offset_i = 0;
        over.Offset = offset_i;
        over.OffsetHigh = offset_i >> 31;

        char buffer[BUFFER_SIZE];
        buffer[0] = '\0';

        while (strcmp(buffer, EXIT_STR) != 0) {
            callback = 0;

            ZeroMemory(buffer, BUFFER_SIZE);

            ReadFileEx(hPipe, buffer, BUFFER_SIZE, &over, FileIOCompletionRoutine);
            SleepEx(-1, TRUE);

            std::cout << "Message: \"" << buffer << "\". " << std::endl;
        }
        CloseHandle(hPipe);
    } else {
        std::cout << "Error: Can't connect to the pipe." << std::endl;

    }
}
void creationPipe(){
    WaitNamedPipeA(PIPE_NAME.c_str(), NMPWAIT_WAIT_FOREVER);

    hPipe = CreateFileA(PIPE_NAME.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
    std::cout << "Named pipe has successfully created" << std::endl;
}

int main()
{
    int choice;
    do{

        show_menu();
        std::cin>>choice;
        switch (choice) {
            case 1:
                creationPipe();
                system("pause");
                break;
            case 2:
                readingMode();
                system("pause");
                break;
        }
    }while (choice != 3);


    return 0;
}