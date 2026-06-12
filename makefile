SRCS = $(wildcard *.c) $(wildcard utils/*.c) $(wildcard filters/*.c)
OBJS = $(SRCS:.c=.o)

TARGET = fake_pedal

CC = cc

LINKER_FLAGS = -lm -lportaudio

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LINKER_FLAGS)
	chmod +x $(TARGET)

%.o: %.c
	$(CC) -c -o $@ $^

filters/%.o: filters/%.c
	$(CC) -c -o $@ $^

clean:
	rm -f $(TARGET) $(OBJS)

input.wav: pre_input.wav
	ffmpeg -i pre_input.wav -acodec pcm_s16le -ar 44100 -ac 2 -map_metadata -1 -fflags +bitexact input.wav
