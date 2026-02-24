FILTERS_SRCS = $(wildcard filters/*.c)
FILTERS_OBJS = $(FILTERS_SRCS:.c=.o)

ROOT_SRCS = main.c pedal.c
ROOT_OBJS = $(ROOT_SRCS:.c=.o)

TARGET = fake_pedal

$(TARGET): $(ROOT_OBJS) $(FILTERS_OBJS)
	cc -o $(TARGET) $(ROOT_OBJS) $(FILTERS_OBJS)
	chmod +x $(TARGET)

main.o: main.c
	cc -c -o main.o main.c

pedal.o: pedal.c
	cc -c -o pedal.o pedal.c

filters/%.o: filters/%.c
	cc -c -o $@ $^

clean:
	rm fake_pedal main.o pedal.o filters/*.o

input.wav: pre_input.wav
	ffmpeg -i pre_input.wav -acodec pcm_s16le -ar 44100 -ac 2 -map_metadata -1 -fflags +bitexact input.wav
