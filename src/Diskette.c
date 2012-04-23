#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[]) {

    HANDLE hDiskette;
    char buffer[1024];
    int i;
    DWORD nByteWritten;

    hDiskette = CreateFile("\\\\.\\A:", GENERIC_WRITE,
                         FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                         OPEN_EXISTING, 0, NULL);

    if (hDiskette == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error al tratar de abrir el archivo %ld\r\n",
                GetLastError());
        ExitProcess(0);
    }

    ZeroMemory(buffer, 1024);

    for (i = 0; i < 1440; i++) {
        sprintf(buffer, "Buffer: %d", i);

        WriteFile(hDiskette, buffer, 1024, &nByteWritten, NULL);
    }

    CloseHandle(hDiskette);
    ExitProcess(0);
    return 0;
}

