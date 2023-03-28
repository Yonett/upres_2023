main: main.c
	gcc main.c -o main.o

recorder: recorder.c
	gcc recorder.c -o recorder.o

reader: reader.c
	gcc reader.c -o reader.o

all: main.c recorder.c reader.c
	gcc main.c -o main.o
	gcc recorder.c -o recorder.o
	gcc reader.c -o reader.o
	
clean:
	rm main.o
	rm recorder.o
	rm reader.o