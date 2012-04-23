// Memoria compartida 2, muestra lo que escribio otro proceso en el segmento
// de memoria compartida
#include <windows.h>
#include <stdio.h>

int
main(int argc, char *argv[])
{

    HANDLE s_hFileMap;
    LPVOID lpDir;
    LPBYTE lpByte;

    // La funcion CreateFileMapping, tiene dos propositos crear un segmento de
    // memoria compartida y compartir un archivo en memoria, para el primer proposito
    // es importante ejecutar pasarle un handle invalido en el primer valor
    // por que le indica que es un segmento y no un archivo.
    // Crea un segmento de memoria indentificado por la palabra clave MMFShareData
    s_hFileMap = CreateFileMapping((HANDLE) INVALID_HANDLE_VALUE,
                    NULL, // Atributos de seguridad, no requiere para el mismo usuario
                    PAGE_READWRITE, // Que forma se abre el segmento de memoria
                    0, 1024, // Valores del tamaño del segmento
                    __TEXT("MMFShareData")); // Identificador del segmento de memoria


    if (s_hFileMap != NULL) {

        // La funcion MapViewOfFile se encarga de mapear en memoria el segmento
        // de memoria solicitado.
        lpDir = MapViewOfFile(s_hFileMap,
                FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

        if (lpDir != NULL) {
           lpByte = (LPBYTE) lpDir;

           printf("%s", lpByte);
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

