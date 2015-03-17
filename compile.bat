set PATH=%PATH%;C:\MinGW\bin

gcc -c -std=c99 -Wall -Wextra httpServer.c
gcc -o httpServer.exe httpServer.o -lwsock32
del httpServer.o

pause
