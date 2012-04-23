// Memoria compartida
// Escribe en el segmento de memoria compartida.
#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{

    HANDLE s_hFileMap;
    LPVOID lpDir;
    LPBYTE lpByte;

    s_hFileMap = CreateFileMapping((HANDLE) INVALID_HANDLE_VALUE,
                    NULL, PAGE_READWRITE, 0, 1024, __TEXT("MMFShareData"));

    if (s_hFileMap != NULL) {

        lpDir = MapViewOfFile(s_hFileMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

        if (lpDir != NULL) {
           lpByte = (LPBYTE) lpDir;

           sprintf(lpByte, "Hola Mundo");
        }
        else {
           fprintf(stderr, "Error: %ld\n", GetLastError());
           CloseHandle(s_hFileMap);
           ExitProcess((DWORD) 0);
        }
    }

    Sleep(10000);
    CloseHandle(s_hFileMap);
    ExitProcess((DWORD) 0);
}

