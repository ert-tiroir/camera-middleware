
g++ -o websocket \
    -I. websocket.cpp \
    -I./wsserver-video ./wsserver-video/wsvideo/*.cpp \
    -I./wsserver-video/libs/wsserver ./wsserver-video/libs/wsserver/wsserver/*.cpp ./wsserver-video/libs/wsserver/wsserver/**/*.cpp
gcc -o receiver receiver.c
