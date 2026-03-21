#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "pedal.h"

void help_dialog(void)
{
  puts("\n## Command line format:");
  puts("`fake_pedal FILTER_LIST_STRING OPTION` [^1]");
  puts("\n\n## Options:");
  puts("- `f INPUT OUTPUT`: apply to a WAV, reading from INPUT and writing to OUTPUT.");
  puts("   e.g. `fake_pedal ... f input.wav output.wav`");
  puts("\n\n## Notes:");
  puts("1. Check filter options in the project's `README.md`.");
  puts("");
}

void process_file(FILE *log, const char *filter_list, const char *input_file, const char *output_file)
{
  FILE *input_wav;
  FILE *output_wav;
  uint8_t header[44];

  fprintf(log, "Opening %s...\n", input_file);
  input_wav = fopen(input_file, "rb");
  fprintf(log, "Finished opening %s.\n", input_file);

  fprintf(log, "Opening %s...", output_file);
  output_wav = fopen(output_file, "wb");
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

int32_t main(int32_t argc, const char **argv)
{
  FILE *log = stderr;

  fputs("==========\nFake Pedal\n==========\n\n", log);

  if (argc)
  {
    ++argv;
    --argc;
  }

  const char *filter_list;

  if (argc == 0)
  {
    puts("String expected. Use `fake_pedal FILTER_LIST_STRING [OPTIONS...]` or `fake_pedal help`");
    goto program_exit;
  }

  if (strcmp(*argv, "help") == 0)
  {
    help_dialog();
    return 0;
  }

  filter_list = *argv++;
  --argc;

  if (argc)
    switch (**argv)
    {
    case 'f':
      if (argc <3)
	puts("Please provide files to read with option `f`.");
      else
	process_file(log, filter_list, argv[1], argv[2]);
      break;
    }

program_exit:
  fputs("Exiting...\n", log);
  return 0;
}
