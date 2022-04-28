gcc -O2 -c graphics.c
c++ -O2 -c Cell.cpp
c++ -O2 -c Wire.cpp
c++ -O2 -c -Wno-unused-result Circuit.cpp
c++ -O2 -c -Wno-unused-result main.cpp
c++ -O2 graphics.o Cell.o Wire.o Circuit.o main.o -o Partition -L/usr/openWin/lib -lX11 -lm

rm *.o

echo "finish compiling, try Placer now"
echo ""
echo "Usage: ./Partition <benchmark-name> [--detail <1-3>]"
echo "example: ./Partition cm150a --detail 2"