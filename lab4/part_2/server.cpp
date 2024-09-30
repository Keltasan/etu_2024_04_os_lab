#include <windows.h>
#include <iostream>

const size_t BUFFER_SIZE = 1024;
const std::string PIPE_NAME("\\\\.\\pipe\\lab4");
const char* EXIT_STR = "stop";
HANDLE hPipe;
OVERLAPPED over;
void show_menu(){
    std::cout << "Choose parameter:" << std::endl;
    std::cout << "1. Create named pipe" << std::endl;
    std::cout << "2. Connect" << std::endl;
    std::cout << "3. Write message" << std::endl;
    std::cout << "4. Disconnect" << std::endl;
    std::cout << "5. Exit" << std::endl;
    std::cout << "Input: ";
}
void pipeCreation(){
    hPipe = CreateNamedPipeA(PIPE_NAME.c_str(),
                             PIPE_ACCESS_OUTBOUND | FILE_FLAG_OVERLAPPED | WRITE_DAC,
                             PIPE_TYPE_MESSAGE | PIPE_WAIT,
                             1, 0, 0, 0, NULL);
    if (hPipe){
        std::cout<<"Named pipe has successfully created"<<std::endl;

    }
    else std::cout<<"Creation failed"<<std::endl;
}

void writingPipe() {
    size_t i;

    over.hEvent = CreateEvent(NULL, false, false, NULL);

    char buffer[BUFFER_SIZE];
    std::string string_buffer;
    while (strcmp(buffer, EXIT_STR) != 0) {
        ZeroMemory(buffer, 0);
        std::cout << "Enter message (" << EXIT_STR << " to exit): ";
        getline(std::cin, string_buffer);

        if (string_buffer.length() - 1 > BUFFER_SIZE) {
            std::cout << "Error: Message more than " << BUFFER_SIZE << " bytes." << std::endl;
            continue;
        } else {
            // CopyMemory(buffer, string_buffer.c_str(), string_buffer.length() * sizeof(char));
            for (i = 0; i < string_buffer.length(); ++i)
                buffer[i] = string_buffer[i];
            buffer[i] = '\0';
        }

        WriteFile(hPipe, buffer, strlen(buffer) + 1, NULL, &over);
        WaitForSingleObject(over.hEvent, INFINITE);
        std::cout << "Message was sent!" << std::endl;
    }
}

int main()
{

    int choice;
    do {

        show_menu();
        std::cin >> choice;
        switch (choice) {
            case 1:
                pipeCreation();
                system("pause");
                break;
            case 2:
                if (ConnectNamedPipe(hPipe, NULL)) {
                    std::cout << "Successfully" << std::endl;

                } else {
                    std::cout << "Error: Can't connect to pipe." << std::endl;
                    return GetLastError();
                }
                system("pause");
                break;
            case 3:
                writingPipe();
                system("pause");
                break;
            case 4:
                DisconnectNamedPipe(hPipe);
                CloseHandle(over.hEvent);
                system("pause");
                break;
            default:
                std::cout << "Try again" << std::endl;
        }
    }while(choice!=5);



    return 0;
}