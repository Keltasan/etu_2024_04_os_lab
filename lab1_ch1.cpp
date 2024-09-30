//lab1
//1. disk list(done)
//2. disk info(done)
//3. create/remove dir
//4. create file(done)
//5. copy/move file
//6. get/edit file info
#include <iostream>
#include <windows.h>
#include <string>
#include <cmath>

using namespace std;

string currentPath = "D:\\";

void MainMenu();
void GetDList();
void GetDType();
void GetVInfo();
void GetDSpaceInfo();
void CreateRemoveDir(char action);
void CreateNewFile();
void FileCopyMove(char action);
void FileAttributesGet();
void FileAttributesSet();
void GetFileInformationByHandle();
void GetFileTime();
void SetFileTime();
string GetDName();
void ChangeCurrentPath();

int main(int argc, char* argv[]) {
    MainMenu();

    return 0;
}

void MainMenu(){
    int flag = -1;
    do{
        cout << "\n Choose category that you want: \n";
        cout << "Disk list: \n";
        cout << "    11 -- Show all available disk drives \n";
        cout << "Disk info: \n";
        cout << "    21 -- Show drive type \n";
        cout << "    22 -- Show volume info \n";
        cout << "    23 -- Show disk free space \n";
        cout << "Directories: \n";
        cout << "    31 -- Create new dir \n";
        cout << "    32 -- Remove existing dir \n";
        cout << "Create file: \n";
        cout << "    41 -- Create new file \n";
        cout << "Copy/move files \n";
        cout << "    51 -- Copy file \n";
        cout << "    52 -- Move file \n";
        cout << "    53 -- Move file with extended properties \n";
        cout << "Attributes \n";
        cout << "    61 -- Get file attributes \n";
        cout << "    62 -- Set file attributes \n";
        cout << "    63 -- Get file attributes by handle \n";
        cout << "    64 -- Get file time \n";
        cout << "    65 -- Set file time \n";
        cout << "0 -- Exit program \n";
        cout << "\n Enter a code of function: ";

        cin >> flag;
        cout << "\n";


        switch (flag){
            case 0:
                cout << "Bye! \n";
                break;
            case 1:
                cout << "Current path: " << currentPath << "\n";
                break;
            case 11:
                GetDList();
                break;
            case 21:
                GetDType();
                break;
            case 22:
                GetVInfo();
                break;
            case 23:
                GetDSpaceInfo();
                break;
            case 31:
                CreateRemoveDir('c');
                break;
            case 32:
                CreateRemoveDir('r');
                break;
            case 41:
                CreateNewFile();
                break;
            case 51:
                FileCopyMove('c');
                break;
            case 52:
                FileCopyMove('m');
                break;
            case 53:
                FileCopyMove('e');
                break;
            case 61:
                FileAttributesGet();
                break;
            case 62:
                FileAttributesSet();
                break;
            case 63:
                GetFileInformationByHandle();
                break;
            case 64:
                GetFileTime();
                break;
            case 65:
                SetFileTime();
                break;
            default:
                cout << "Incorrect input, try again \n";
                break;
        }

        system("pause");
    }
    while (flag != 0);
}
// ----- Disk List -----
void GetDList(){
    int dNumber = 1;
    int dDetection;
    char dLetter[4] = {0, ':', '\\', 0};
    DWORD dr = GetLogicalDrives();
    for (int i = 0; i < 26; i++){
        dDetection = ((dr >> i) & 0x00000001);

        if (dDetection == 1){
            dLetter[0] = char(65 + i);

            cout << dNumber << ". " << dLetter << "\n";
            dNumber++;
        }
    }
}
// ----- Disk Info -----
void GetDType(){
    int d;
    string n;

    n = GetDName();

    // <string variable>.c_str() means that you convert to <const char * type>, because "" isn't <const char *> type

    d = GetDriveType(n.c_str());
    cout << n << " is";
    //cout << n + " is";

    if (d == DRIVE_UNKNOWN)
    {
        cout << " UNKNOWN" << endl;
    }
    if (d == DRIVE_NO_ROOT_DIR)
    {
        cout << " DRIVE NO ROOT DIR" << endl;
    }
    if (d == DRIVE_REMOVABLE)
    {
        cout << " REMOVABLE" << endl;
    }
    if (d == DRIVE_FIXED)
    {
        cout << " FIXED" << endl;
    }
    if (d == DRIVE_REMOTE)
    {
        cout << " REMOTE" << endl;
    }
    if (d == DRIVE_CDROM)
    {
        cout << " CDROM" << endl;
    }
    if (d == DRIVE_RAMDISK)
    {
        cout << " RAMDISK" << endl;
    }
}

void GetVInfo(){
    char volumeNameBuffer[100];
    char fileSystemNameBuffer[100];
    DWORD flags;
    string n;
    unsigned long volumeSerialNumber;

    n = GetDName();

    BOOL GetVolumeInformationFlag = GetVolumeInformationA(
            n.c_str(),
            volumeNameBuffer,
            100,
            &volumeSerialNumber,
            nullptr, //&MaximumComponentLength,
            &flags, //&FileSystemFlags,
            fileSystemNameBuffer,
            100
    );

    if (GetVolumeInformationFlag != 0)
    {
        cout << " Volume Name is " << volumeNameBuffer << endl;//When volume name is empty, that means, that your volume is unnamed
        cout << " Volume Serial Number is " << volumeSerialNumber << endl;
        cout << " File System is " << fileSystemNameBuffer << endl;
        cout << " File System Flags: " << endl;
        if (flags & FILE_CASE_SENSITIVE_SEARCH)
            cout << "- Case sensitive search" << endl;
        if (flags & FILE_CASE_PRESERVED_NAMES)
            cout << "- Preserved case names" << endl;
        if (flags & FILE_UNICODE_ON_DISK)
            cout << "- Unicode on disk" << endl;
        if (flags & FILE_PERSISTENT_ACLS)
            cout << "- Persistent ACLs" << endl;
        if (flags & FILE_FILE_COMPRESSION)
            cout << "- File compression" << endl;
        if (flags & FILE_VOLUME_QUOTAS)
            cout << "- Volume quotas" << endl;
    }
    else
    {
        cout << " Not Present (GetVolumeInformation)" << endl;
    }
}

void GetDSpaceInfo(){
    string dName = GetDName();

    //sec - sector, clus - cluster
    DWORD secPerClus;
    DWORD bytePerSec;
    DWORD freeClus;
    DWORD totalClus;
    int dFreeSpace = GetDiskFreeSpace(dName.c_str(), &secPerClus, &bytePerSec, &freeClus, &totalClus);

    if (dFreeSpace != 0)
    {
        cout << "Disk " << dName << " free space info:\n";
        cout << "Number of Sectors Per Cluster: " << (unsigned long long)secPerClus << "\nTotal Number Of Bytes per Sector: " << (unsigned long long)bytePerSec << "\nTotal Number Of Free Clusters: " << (unsigned long long)freeClus << "\nTotal Number Of Clusters: " << (unsigned long long)totalClus << "\nReturned value: " << (unsigned long long)dFreeSpace << "\n";
        cout << "Total Number Of Bytes per Cluster: " << (unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\nTotal Number Of Free Sectors: " << (unsigned long long)freeClus*(unsigned long long)secPerClus;
        cout << "\nTotal Number Of Sectors: " << (unsigned long long)totalClus*(unsigned long long)secPerClus << "\nTotal Number Of Free Bytes: " << (unsigned long long)freeClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\nTotal Number of Bytes: " << (unsigned long long)totalClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\n";
        cout << "Total Number Of Used Clusters: " << (unsigned long long)totalClus - (unsigned long long)freeClus;
        cout << "\nTotal Number Of Used Sectors: " << (unsigned long long)totalClus*(unsigned long long)secPerClus - (unsigned long long)freeClus*(unsigned long long)secPerClus << "\nTotal Number Of Used Bytes: " << (unsigned long long)totalClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec - (unsigned long long)freeClus*(unsigned long long)secPerClus*(unsigned long long)bytePerSec << "\n";
    }
    else
    {
        cout << "Returned value: " << (unsigned long long)dFreeSpace << "\n There is no such disk as " << dName << " :(\n";
    }
}

// -----Directories-----
void CreateRemoveDir(char action){
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string dirName;
    cout << "Please, enter directory name:\n";
    cin >> dirName;

    std::string fullPath = currentPath + "\\" + dirName;

    if (action == 'c'){
        if (CreateDirectory(fullPath.c_str(), NULL)){
            cout << "Directory was successfully created!\n";
        }
        else{
            cout << "Error with creating a directory:(\n";
        }
    }
    else{
        if (RemoveDirectory(fullPath.c_str())){
            cout << "Directory was successfully removed\n";
        }
        else{
            cout << "Error with removing directory:(\n";
        }
    }
}
// ----- Create File -----
void CreateNewFile(){
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;

    std::string fullPath = currentPath + "\\" + fileName;
    HANDLE hFile = CreateFile(fullPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Error with creating file!\n" << std::endl;
    } else {
        std::cout << "File succesfully created:\n" << fullPath << std::endl;
        CloseHandle(hFile);
    }
}
// ----- Copy/move file -----
void FileCopyMove(char action){
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName, copiedFileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;
    string sourcePath = currentPath + "\\" + fileName;
    if (action == 'c') {
        cout << "Enter copied file name:\n";
        cin >> copiedFileName;
        string destinationPath = currentPath + "\\" + copiedFileName;

        if (CopyFileA(sourcePath.c_str(), destinationPath.c_str(), FALSE)) {
            cout << "File successfully copied!\n";
        } else {
            cout << "Error while copying file:(\n";
        }
    }
    if (action == 'm'){
        string destinationPath;
        cout << "Enter destination path:\n";
        cin >> destinationPath;
        destinationPath += "\\" + fileName;

        if (MoveFile(sourcePath.c_str(), destinationPath.c_str())){
            cout << "File successfully moved:\n";
        }
        else{
            cout << "Error while moving file. Error code:" << GetLastError() << endl;
        }
    }
    if (action == 'e'){
        string destinationPath;
        cout << "Enter destination path:\n";
        destinationPath += "\\" + fileName;

        DWORD flag = 2;
        cout << "Please, choose the possible flags for moving file:\n"
             << "1 -- MOVEFILE_REPLACE_EXISTING (replacing existing, it it exists)\n"
             << "2 -- MOVEFILE_COPY_ALLOWED (classic move) -- DEFAULT\n"
             << "4 -- MOVEFILE_DELAY_UNTIL_REBOOT (moving after rebooting the system)\n"
             << "8 -- MOVEFILE_WRITE_THROUGH (moving file, than returning value)\n"
             << "16 -- MOVEFILE_CREATE_HARDLINK\n"
             << "32 -- MOVEFILE_FAIL_IF_NOT_TRACKABLE\n";
        cin >> flag;

        if(MoveFileEx(currentPath.c_str(),destinationPath.c_str(),(DWORD)flag)){
            cout << "File successfully moved with extensions!\n";
        }
        else{
            cout << "Error while moving file. Error code:" << GetLastError() << endl;
        }
    }
}
// ----- Attributes -----
void FileAttributesGet(){
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName, copiedFileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;
    string filePath = currentPath + "\\" + fileName;

    DWORD fileAttributes = 0;
    fileAttributes = GetFileAttributes(filePath.c_str());

    cout << "File \"" << filePath << "\" attributes:\n";
    if (fileAttributes & FILE_ATTRIBUTE_ARCHIVE)
    {
        cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_COMPRESSED)
    {
        cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_DEVICE)
    {
        cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
    {
        cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_HIDDEN)
    {
        cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
    {
        cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_NORMAL)
    {
        cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
    {
        cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_NO_SCRUB_DATA)
    {
        cout << "Data stream not to be read by the data integrity scanner (FILE_ATTRIBUTE_NO_SCRUB_DATA)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_OFFLINE)
    {
        cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_READONLY)
    {
        cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)
    {
        cout << "Data is not fully presented locally (FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_RECALL_ON_OPEN)
    {
        cout << "Data hasn't physical representation on system (FILE_ATTRIBUTE_RECALL_ON_OPEN)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
    {
        cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
    {
        cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_SYSTEM)
    {
        cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_TEMPORARY)
    {
        cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
    }
    if (fileAttributes & FILE_ATTRIBUTE_VIRTUAL)
    {
        cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
    }
}
void FileAttributesSet(){
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName, copiedFileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;
    string filePath = currentPath + "\\" + fileName;

    DWORD fileAttributes = 0;
    fileAttributes = GetFileAttributes(filePath.c_str());

    DWORD attNum = 128;
    cout << "Please, choose the possible attributes for the file/directory:\n"
         << "1 -- FILE_ATTRIBUTE_READONLY (read-only)\n"
         << "2 -- FILE_ATTRIBUTE_HIDDEN (hidden)\n"
         << "4 -- FILE_ATTRIBUTE_SYSTEM (system used)\n"
         << "32 -- FILE_ATTRIBUTE_ARCHIVE (archive)\n"
         << "128 -- FILE_ATTRIBUTE_NORMAL -- DEFAULT\n"
         << "256 -- FILE_ATTRIBUTE_TEMPORARY (temporary storage)\n"
         << "4096 -- FILE_ATTRIBUTE_OFFLINE (don't avaliable immediatly)\n"
         << "8192 -- FILE_ATTRIBUTE_NOT_CONTENT_INDEXED (not indexed)\n";
    cin >> attNum;

    if (SetFileAttributes(filePath.c_str(), (DWORD) attNum))
    {
        cout << "The file's (directory's) \"" << filePath << "\" attributes has been successfully changed to:\n";
        cout << "File \"" << filePath << "\" attributes:\n";
        if (attNum & FILE_ATTRIBUTE_ARCHIVE)
        {
            cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
        }
        if (attNum & FILE_ATTRIBUTE_COMPRESSED)
        {
            cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
        }
        if (attNum & FILE_ATTRIBUTE_DEVICE)
        {
            cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
        }
        if (attNum & FILE_ATTRIBUTE_DIRECTORY)
        {
            cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
        }
        if (attNum & FILE_ATTRIBUTE_ENCRYPTED)
        {
            cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
        }
        if (attNum & FILE_ATTRIBUTE_HIDDEN)
        {
            cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
        }
        if (attNum & FILE_ATTRIBUTE_NORMAL)
        {
            cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
        }
        if (attNum & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
        {
            cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
        }
        if (attNum & FILE_ATTRIBUTE_OFFLINE)
        {
            cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
        }
        if (attNum & FILE_ATTRIBUTE_READONLY)
        {
            cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
        }
        if (attNum & FILE_ATTRIBUTE_REPARSE_POINT)
        {
            cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
        }
        if (attNum & FILE_ATTRIBUTE_SPARSE_FILE)
        {
            cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
        }
        if (attNum & FILE_ATTRIBUTE_SYSTEM)
        {
            cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
        }
        if (attNum & FILE_ATTRIBUTE_TEMPORARY)
        {
            cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
        }
        if (attNum & FILE_ATTRIBUTE_VIRTUAL)
        {
            cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
        }
    }
    else
    {
        cout << "Something wrong! The file's (directory's) \"" << filePath << "\" attributes hasn't been changed\n";
        cout << "Last error code is \"" << GetLastError() << "\"\n"; // here i need to insert last error text string
    }
}

void GetFileInformationByHandle ()
{
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName, copiedFileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;
    string filePath = currentPath + "\\" + fileName;

    HANDLE hFile = CreateFile(filePath.c_str(), // file name
                              GENERIC_READ,          // open for reading
                              0,                     // do not share
                              NULL,                  // default security
                              OPEN_EXISTING,         // existing file only
                              FILE_ATTRIBUTE_NORMAL, // normal file
                              NULL);
    int size=0;
    //PBY_HANDLE_FILE_INFORMATION lpFileInformation = new _BY_HANDLE_FILE_INFORMATION();
    BY_HANDLE_FILE_INFORMATION* lpFileInformation = new BY_HANDLE_FILE_INFORMATION();
    int resalt = GetFileInformationByHandle(hFile,lpFileInformation);
    size = lpFileInformation->nFileSizeLow;

    DWORD localAttributes = lpFileInformation->dwFileAttributes;

    if (localAttributes & FILE_ATTRIBUTE_ARCHIVE)
    {
        cout << "Archive (FILE_ATTRIBUTE_ARCHIVE)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_COMPRESSED)
    {
        cout << "Compressed (FILE_ATTRIBUTE_COMPRESSED)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_DEVICE)
    {
        cout << "Device (FILE_ATTRIBUTE_DEVICE)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_DIRECTORY)
    {
        cout << "Directory (FILE_ATTRIBUTE_DIRECTORY)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_ENCRYPTED)
    {
        cout << "Encrypted (FILE_ATTRIBUTE_ENCRYPTED)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_HIDDEN)
    {
        cout << "Hidden (FILE_ATTRIBUTE_HIDDEN)\n";
    }
    /*if (localAttributes & FILE_ATTRIBUTE_INTEGRITY_STREAM)
    {
        cout << "Data stream configured with integrity (FILE_ATTRIBUTE_INTEGRITY_STREAM)\n";
    }*/
    if (localAttributes & FILE_ATTRIBUTE_NORMAL)
    {
        cout << "Normal (FILE_ATTRIBUTE_NORMAL)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)
    {
        cout << "Not indexed (FILE_ATTRIBUTE_NOT_CONTENT_INDEXED)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_OFFLINE)
    {
        cout << "Don't avaliable immediatly (FILE_ATTRIBUTE_OFFLINE)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_READONLY)
    {
        cout << "Read-only (FILE_ATTRIBUTE_READONLY)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_REPARSE_POINT)
    {
        cout << "Reparse point/representation link (FILE_ATTRIBUTE_REPARSE_POINT)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_SPARSE_FILE)
    {
        cout << "Sparse file (FILE_ATTRIBUTE_SPARSE_FILE)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_SYSTEM)
    {
        cout << "System used (FILE_ATTRIBUTE_SYSTEM)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_TEMPORARY)
    {
        cout << "Temporary storage (FILE_ATTRIBUTE_TEMPORARY)\n";
    }
    if (localAttributes & FILE_ATTRIBUTE_VIRTUAL)
    {
        cout << "Reserved for system (FILE_ATTRIBUTE_VIRTUAL)\n";
    }

    char buffer[256];

    SYSTEMTIME st;
    FILETIME ft;
    string strMessage;

    // first

    ft.dwLowDateTime = (lpFileInformation->ftCreationTime).dwLowDateTime;
    ft.dwHighDateTime = (lpFileInformation->ftCreationTime).dwHighDateTime;

    FileTimeToSystemTime(&ft, &st);

    sprintf( buffer,
             "%d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear,
             st.wMonth,
             st.wDay,
             st.wHour,
             st.wMinute,
             st.wSecond,
             st.wMilliseconds );
    strMessage = buffer;

    std::cout << "CREATION TIME = " << strMessage << endl;

    // second

    ft.dwLowDateTime = (lpFileInformation->ftLastWriteTime).dwLowDateTime;
    ft.dwHighDateTime = (lpFileInformation->ftLastWriteTime).dwHighDateTime;

    FileTimeToSystemTime(&ft, &st);

    sprintf( buffer,
             "%d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear,
             st.wMonth,
             st.wDay,
             st.wHour,
             st.wMinute,
             st.wSecond,
             st.wMilliseconds );
    strMessage = buffer;

    std::cout << "LAST WRITE TIME = " << strMessage << endl;

    // third

    ft.dwLowDateTime = (lpFileInformation->ftLastAccessTime).dwLowDateTime;
    ft.dwHighDateTime = (lpFileInformation->ftLastAccessTime).dwHighDateTime;

    FileTimeToSystemTime(&ft, &st);

    sprintf( buffer,
             "%d-%02d-%02d %02d:%02d:%02d.%03d",
             st.wYear,
             st.wMonth,
             st.wDay,
             st.wHour,
             st.wMinute,
             st.wSecond,
             st.wMilliseconds );
    strMessage = buffer;

    std::cout << "LAST ACCESS TIME = " << strMessage << endl;

    cout << "Volume serial number: " << lpFileInformation->dwVolumeSerialNumber << "\n";
    cout << "Local size high/low: " << lpFileInformation->nFileSizeHigh << " " << lpFileInformation->nFileSizeLow << "\n";
    cout << "Number Of Links: " << lpFileInformation->nNumberOfLinks << "\n";
    cout << "Index High/low: " << lpFileInformation->nFileIndexHigh << " " << lpFileInformation->nFileIndexLow << "\n";

    CloseHandle(hFile);
}

// ---------- 6 -- LOCAL GET FILE INFORMATION BY HANDLE ----------

void GetFileTime ()
{
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName, copiedFileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;
    string filePath = currentPath + "\\" + fileName;

    HANDLE hFile = CreateFile(filePath.c_str(), // file name
                              GENERIC_READ,          // open for reading
                              0,                     // do not share
                              NULL,                  // default security
                              OPEN_EXISTING,         // existing file only
                              0, // normal file
                              NULL);

    FILETIME creationTime;
    FILETIME lastWriteTime;
    FILETIME lastAccessTime;

    if (GetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime)){
        char buffer[256];

        SYSTEMTIME st;
        FileTimeToSystemTime(&creationTime, &st);

        sprintf( buffer,
                 "%d-%02d-%02d %02d:%02d:%02d.%03d",
                 st.wYear,
                 st.wMonth,
                 st.wDay,
                 st.wHour,
                 st.wMinute,
                 st.wSecond,
                 st.wMilliseconds );
        string strMessage = buffer;

        std::cout << "System time = " << strMessage << std::endl;

        FileTimeToSystemTime(&lastAccessTime, &st);

        sprintf( buffer,
                 "%d-%02d-%02d %02d:%02d:%02d.%03d",
                 st.wYear,
                 st.wMonth,
                 st.wDay,
                 st.wHour,
                 st.wMinute,
                 st.wSecond,
                 st.wMilliseconds );
        strMessage = buffer;

        std::cout << "Access time = " << strMessage << std::endl;

        FileTimeToSystemTime(&lastWriteTime, &st);

        sprintf( buffer,
                 "%d-%02d-%02d %02d:%02d:%02d.%03d",
                 st.wYear,
                 st.wMonth,
                 st.wDay,
                 st.wHour,
                 st.wMinute,
                 st.wSecond,
                 st.wMilliseconds );
        strMessage = buffer;

        std::cout << "Change time = " << strMessage << std::endl;
    }
    else
    {
        cout << "Something wrong!" << "\n";
    }
    CloseHandle(hFile);
}

// ---------- 6 -- LOCAL GET FILE INFORMATION BY HANDLE ----------

void SetFileTime ()
{
    char t;
    cout << "Want to change current path?(Y/N):\n";
    cin >> t;

    if (t == 'Y'){
        ChangeCurrentPath();
    }

    string fileName, copiedFileName;
    cout << "Please, enter file name:\n";
    cin >> fileName;
    string filePath = currentPath + "\\" + fileName;

    HANDLE hFile = CreateFile(filePath.c_str(), // file name
                              GENERIC_WRITE,          // open for reading
                              0,                     // do not share
                              NULL,                  // default security
                              OPEN_EXISTING,         // existing file only
                              0, // normal file
                              NULL);

    string one;
    string two;
    string three;

    cout << "Time format: 2024-01-03 18:00:00.000\n";
    cout << "Creation time:\n";
    fflush(stdin);
    getline(cin, one);

    cout << "Last write time:\n";
    fflush(stdin);
    getline(cin, two);


    cout << "Last access time:\n";
    fflush(stdin);
    getline(cin, three);

    FILETIME creationTime;//= buffTime.creationTime;
    FILETIME lastWriteTime;// = buffTime.lastWriteTime;
    FILETIME lastAccessTime;// = buffTime.lastAccessTime;

    SYSTEMTIME systime_1;
    SYSTEMTIME systime_2;
    SYSTEMTIME systime_3;

    memset(&systime_1,0,sizeof(systime_1));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(one.c_str(), "%d-%d-%d%d:%d:%d:%d.%d",
             &systime_1.wYear,
             &systime_1.wMonth,
             &systime_1.wDay,
             &systime_1.wHour,
             &systime_1.wMinute,
             &systime_1.wSecond,
             &systime_1.wMilliseconds);

    SystemTimeToFileTime(&systime_1, &creationTime);

    memset(&systime_2,0,sizeof(systime_2));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(two.c_str(), "%d-%d-%d%d:%d:%d:%d.%d",
             &systime_2.wYear,
             &systime_2.wMonth,
             &systime_2.wDay,
             &systime_2.wHour,
             &systime_2.wMinute,
             &systime_2.wSecond,
             &systime_2.wMilliseconds);

    SystemTimeToFileTime(&systime_2, &lastWriteTime);

    memset(&systime_3,0,sizeof(systime_3));
    // Date string should be "yyyy-MM-dd hh:mm"
    sscanf_s(three.c_str(), "%d-%d-%d%d:%d:%d:%d.%d",
             &systime_3.wYear,
             &systime_3.wMonth,
             &systime_3.wDay,
             &systime_3.wHour,
             &systime_3.wMinute,
             &systime_3.wSecond,
             &systime_3.wMilliseconds);

    SystemTimeToFileTime(&systime_3, &lastAccessTime);

    if(SetFileTime(hFile, &creationTime, &lastAccessTime, &lastWriteTime))
    {
        cout << "Time changed!" << endl;
    }
    else
    {
        cout << "Time hasn't been changed!" << endl;
    }

    CloseHandle(hFile);
}
// ----- Additional Functions -----
string GetDName ()
{
    string d;

    cout << "Please, input disk name you want:\n";
    cin >> d;
    d = d + ":\\";

    return d;
}

void ChangeCurrentPath(){
    cout << "Please, enter new directory path (example \"C:\\DirName1\\DirName2):\n";
    cin >> currentPath;
    SetCurrentDirectory(currentPath.c_str());
}