CC=g++

TARGET=testWebserver

SRC=$(wildcard ./*.cpp) \
	$(wildcard ./soap_generation_files/*.cpp) \
	./plugin/httpget.c \
	./plugin/httppost.c
#	./jsoncpp/jsoncpp.cpp

LIBS=-lgsoap++
CIFLAGS=-I ./plugin \
		-I ./soap_generation_files
#		-I ./jsoncpp

CLFLAGS=-L /home/nu/lib
CFLAGS=$(CIFLAGS) $(CLFLAGS)


all: $(TARGET)
$(TARGET):
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f soap_generation_files/* testWebserver
