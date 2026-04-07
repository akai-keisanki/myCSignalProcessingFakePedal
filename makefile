FILTERS_SRCS = $(wildcard filters/*.c)
FILTERS_OBJS = $(FILTERS_SRCS:.c=.o)

ROOT_SRCS = main.c pedal.c filter_interpreter.c
ROOT_OBJS = $(ROOT_SRCS:.c=.o)

TARGET = fake_pedal

CC = cc

LINKER_FLAGS = -lm

$(TARGET): $(ROOT_OBJS) $(FILTERS_OBJS)
	$(CC) -o $(TARGET) $(ROOT_OBJS) $(FILTERS_OBJS) $(LINKER_FLAGS)
	chmod +x $(TARGET)

%.o: %.c
	$(CC) -c -o $@ $^

filters/%.o: filters/%.c
	$(CC) -c -o $@ $^

clean:
	rm $(TARGET) $(ROOT_OBJS) $(FILTERS_OBJS)

input.wav: pre_input.wav
	ffmpeg -i pre_input.wav -acodec pcm_s16le -ar 44100 -ac 2 -map_metadata -1 -fflags +bitexact input.wav
