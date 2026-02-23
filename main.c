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

  fprintf(stderr, "Opening input.wav...\n");
  input_wav = fopen("input.wav", "rb");
  fprintf(stderr, "Finished opening input.wav.\n");
  fprintf(stderr, "Opening output.wav...");
  output_wav = fopen("output.wav", "wb");
  fprintf(stderr, "Finished opening output.wav.\n");


  fprintf(stderr, "Reading input.wav...\n");
  fread(header, 1, 44, input_wav);
  fprintf(stderr, "Finished reading input.wav.\n");

  fprintf(stderr, "Reading output.wav...\n");
  fwrite(header, 1, 44, output_wav);
  fprintf(stderr, "Finished reading output.wav.\n");
  

  fprintf(stderr, "Beginning filter application from input.wav to output.wav...\n");
  apply_filters(output_wav, input_wav, filter_list);
  fprintf(stderr, "Finished filter application from input.wav to output.wav.\n");


  fprintf(stderr, "Closing input.wav...\n");
  fclose(input_wav);
  fprintf(stderr, "Finished closing input.wav.\n");

  fprintf(stderr, "Closing output.wav...\n");
  fclose(output_wav);
  fprintf(stderr, "Finished closing output.wav.\n");


  fprintf(stderr, "Finished! Output file complete.\n");

  fprintf(stderr, "Exiting...\n");
  return 0;
}
