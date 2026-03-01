FILTERS_SRCS = $(wildcard filters/*.c)
FILTERS_OBJS = $(FILTERS_SRCS:.c=.o)

ROOT_SRCS = main.c pedal.c
ROOT_OBJS = $(ROOT_SRCS:.c=.o)

TARGET = fake_pedal

CC = cc

LINKER_FLAGS = -lm

$(TARGET): $(ROOT_OBJS) $(FILTERS_OBJS)
	$(CC) -o $(TARGET) $(ROOT_OBJS) $(FILTERS_OBJS) $(LINKER_FLAGS)
	chmod +x $(TARGET)

main.o: main.c
	$(CC) -c -o main.o main.c

pedal.o: pedal.c
	$(CC) -c -o pedal.o pedal.c

filters/%.o: filters/%.c
	$(CC) -c -o $@ $^

clean:
	rm fake_pedal main.o pedal.o filters/*.o

input.wav: pre_input.wav
	ffmpeg -i pre_input.wav -acodec pcm_s16le -ar 44100 -ac 2 -map_metadata -1 -fflags +bitexact input.wav
