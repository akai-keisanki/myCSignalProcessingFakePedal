filter_objects = filters/clip.o

fake_pedal: main.o filters/pedal.o $(filter_objects)
	cc -o fake_pedal main.o filters/pedal.o $(filter_objects)
	chmod +x fake_pedal

main.o: main.c
	cc -c -o main.o main.c

filters/pedal.o: filters/pedal.c
	cc -c -o filters/pedal.o filters/pedal.c

filters/clip.o: filters/clip.c
	cc -c -o filters/clip.o filters/clip.c

clean:
	rm fake_pedal main.o filters/pedal.o $(filter_objects)

input.wav: pre_input.wav
	ffmpeg -i pre_input.wav -acodec pcm_s16le -ar 44100 -ac 2 -map_metadata -1 -fflags +bitexact input.wav
