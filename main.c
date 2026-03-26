#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pedal.h"
#include "filters/util_filter_base.h"
#include "filter_interpreter.h"

void help_dialog(void)
{
  puts("\nCOMMAND LINE FORMAT\n-------------------");
  puts("  fake_pedal FILTER_LIST_OPTION FILTER_LIST_SOURCE [OPTION]");
  puts("\nFILTER LIST OPTIONS\n-------------------");
  puts("  - \"s STRING\": apply to a filter string.");
  puts("\nFILTER LIST SRING\n-----------------");
  puts("  Each filter is given in the format: \"xVVVV\" or \"xVVVVWWWW\"");
  puts("  - \"x\" is the character which represents the filter to be applied");
  puts("  - \"VVVV\" and \"WWWW\" are 4-digit numeric values.");
  puts("  E.g.:");
  puts("  - \"D0300\": soft clip with threshold = 0300;");
  puts("  - \"d05003000\": delay with delay = 0300 and mix = 3000;");
  puts("  - \"D0300d05003000\": soft clip followed by delay.");
  puts("\nOPTIONS\n-------");
  puts("  - \"f INPUT OUTPUT\": apply to a WAV, reading from \"INPUT\" and writing to \"OUTPUT\".");
  puts("  e.g. `fake_pedal ... f input.wav output.wav`");
  puts("  - \"l\": apply to live audio, reading from default audio input device and playing to default audio output device.");
  puts("\n");
}

void process_file(FILE *log, struct filter **filter_list, const char *input_file, const char *output_file)
{
  FILE *input_wav;
  FILE *output_wav;
  uint8_t header[44];

  fprintf(log, "Opening %s...\n", input_file);
  input_wav = fopen(input_file, "rb");
  if (!input_wav)
  {
    fprintf(log, "Failed opening %s.\n", input_file);
    return;
  }
  fprintf(log, "Finished opening %s.\n", input_file);

  fprintf(log, "Opening %s...", output_file);
  output_wav = fopen(output_file, "wb");
  if (!output_wav)
  {
    fprintf(log, "Failed opening %s.\n", output_file);
    return;
  }
  fprintf(log, "Finished opening %s.\n", output_file);


  fprintf(log, "Reading %s...\n", input_file);
  fread(header, 1, 44, input_wav);
  fprintf(log, "Finished reading %s.\n", input_file);

  fprintf(log, "Reading %s...\n", output_file);
  fwrite(header, 1, 44, output_wav);
  fprintf(log, "Finished reading %s.\n", output_file);
  

  fprintf(log, "Beginning filter application from %s to %s...\n", input_file, output_file);
  pedal_in_files(output_wav, input_wav, filter_list);
  fprintf(log, "Finished filter application from %s to %s.\n", input_file, output_file);


  fprintf(log, "Closing %s...\n", input_file);
  fclose(input_wav);
  fprintf(log, "Finished closing %s.\n", input_file);

  fprintf(log, "Closing %s...\n", output_file);
  fclose(output_wav);
  fprintf(log, "Finished closing %s.\n", output_file);

  fputs("Finished! Output file complete.\n", log);
}

void process_live(FILE *log, struct filter **filter_list)
{
  FILE *input_wav;
  FILE *output_wav;

  input_wav = popen("sox -d -q -c 1 -r 44100 -b 16 -e signed-integer -t raw -", "r");
  if (!input_wav)
  {
    fputs("Failed opening input pipe.", log);
    return;
  }

  output_wav = popen("play -q -c 1 -r 44100 -b 16 -e signed-integer -t raw -", "w");
  if (!output_wav)
  {
    fputs("Failed opening output pipe.", log);
    return;
  }

  setvbuf(input_wav, NULL, _IONBF, 0);
  setvbuf(output_wav, NULL, _IONBF, 0);

  pedal_in_files(output_wav, input_wav, filter_list);

  pclose(input_wav);
  pclose(output_wav);
}

int32_t main(int32_t argc, const char **argv)
{
  FILE *log = stderr;

  fputs("==========\nFake Pedal\n==========\n\n", log);

  if (argc)
  {
    ++argv;
    --argc;
  }

  struct filter **filter_list;

  if (argc < 1)
  {
    puts("Arg expected. Use \"fake_pedal FILTER_LIST_OPTION FILTER_LIST_SOURCE [OPTION]\" or \"fake_pedal help\"");
    goto program_exit;
  }

  if (strcmp(*argv, "help") == 0)
  {
    help_dialog();
    goto program_exit;
  }

  if (argc < 2)
  {
    puts("Filter list source expected. Use \"fake_pedal FILTER_LIST_OPTION FILTER_LIST_SOURCE\"");
    goto program_exit;
  }

  switch (**argv++)
  {
  case 's':
    filter_list = interpret_filter_string(*argv);
    break;
  }

  ++argv;
  argc -= 2;

  if (argc)
    switch (**argv)
    {
    case 'f':
      if (argc < 3)
	puts("Please provide files to read with option `f`.");
      else
	process_file(log, filter_list, argv[1], argv[2]);
      break;

    case 'l':
      process_live(log, filter_list);
      break;
    }

  free_filter_list(filter_list);

program_exit:
  fputs("Exiting...\n", log);
  return 0;
}
