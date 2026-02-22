#include <stdint.h>
#include <stdio.h>

#include "filters/pedal.h"

int32_t main()
{
  char filter_list[0xff];
  FILE *input_wav;
  FILE *output_wav;
  uint8_t header[44];

  scanf("%s", filter_list);

  fprintf(stderr, "Opening input.wav...");
  input_wav = fopen("input.wav", "rb");
  fprintf(stderr, "Finished opening input.wav.");
  fprintf(stderr, "Opening output.wav...");
  output_wav = fopen("ouput.wav", "wb");
  fprintf(stderr, "Finished opening output.wav.");


  fprintf(stderr, "Reading input.wav...");
  fread(header, 1, 44, input_wav);
  fprintf(stderr, "Finished reading input.wav.");

  fprintf(stderr, "Reading output.wav...");
  fwrite(header, 1, 44, output_wav);
  fprintf(stderr, "Finished reading output.wav.");
  

  fprintf(stderr, "Beginning filter application from input.wav to output.wav...");
  apply_filters(output_wav, input_wav, filter_list);
  fprintf(stderr, "Finished filter application from input.wav to output.wav.");


  fprintf(stderr, "Closing input.wav...");
  fclose(input_wav);
  fprintf(stderr, "Finished closing input.wav.");

  fprintf(stderr, "Closing output.wav...");
  fclose(output_wav);
  fprintf(stderr, "Finished closing output.wav.");


  fprintf(stderr, "Finished! Output file complete.");

  fprintf(stderr, "Exiting...");
  return 0;
}
