# myCSignalProcessingFakePedal

I want to make a fake pedal in C to make some effects in my WAV files.

## Compiling the project To run this software, you must compile it from the source code.

You can clone it from GitHub:
```
sh git clone https://github.com/akai-keisanki/myCSignalProcessingFakePedal.git
```

And `cd` into the directory:
```sh
cd myCSignalProcessingFakePedal/
```

Then you can build it with Make:
```sh
make fake_pedal
```

## Using the right files

Due to technical difficulties, this program can only process 16b WAV files with a of exactly 44B (44100Hz recommended). The file should be named `input.wav` and be in the working directory.

You can use Make to generate an `input.wav` from a `pre_input.wav` file in your input directory, formating it with `ffmpeg` to the required conventions:
```sh
make input.wav
```

An `output.wav` shall be generated after running `fake_pedal` is finished with exit code `0`.

## Working with filters

Just after starting `fake_pedal`, the program will read a string which represents a list of filters to apply, represented by their characters, possibly immediately followed by 1-2 4-digit numeric values.

Here is a simple list of the currently available filters:

- `multiply`: Type `M` followed by a 4-digit value representing the percent of the multiplier applyed to the signal.

- `clip`: Type `C` followed by a 4-digit value representing the per ten thousand of the maximum signal (`1.0`) that should be the new maximum (and negative minimum), clipping higher values to it. It is automatically always used to clip the output to `1.0` after all operations.

- `low_pass`: Type `l` followed by a 4-digit value representing the per ten thousand of the high cut coefficient. Shall not be used 2 or more times in the same execution of the pedal.

- `high_pass`: Type `h` followed by a 4-digit value representing the per ten thousand of the low cut coefficient. Shall not be used 2 or more times in the same execution of the pedal.

- `mid_scoop`: Type `m` followed by 2 4-digit values representing the per ten thousand of the low and high cut coefficients. Shall not be used 2 or more times in the same execution of the pedal.

- `pitch_shift` (BETA): Type `P` followed by a digit representing the signal (`0` for `+`, `1` or other for `-`) and 3 digits representing the per ten of the number of semitones to shift.

- `saturate`: Type `S` followed by a 4-digit value representing the percent of the multiplier applyed to the signal that is saturated with the hyperbolic tangent.

- `delay`: Type `d` followed by 2 4-digit values representing the number of steps (that will be multiplied by 10) to be delayed and the per ten thousand of the multiplicator applyed to the delayed signal to be summed.

- `distortion`: Type `D` followed by a 4-digit value representing the per ten thousand of the maximum signal (`1.0`) that should be the new maximum (and negative minimum), clipping higher values to it and blending with the signal.

Characters without use such as `-`, `,` or `.` can be used as separators.

Here are some example settings:

- Fuzz: `S0300,C0300,M0150,l1500`;
- Warm tone: `M0250,d04003333,l0200,m01000300`;
- Lo-fi tone: `h0600,l0900,S0150,C2000`;
- Warm drive: `l2000,S0150,D0800,m02000700`;


## Adding your own filters

To add your own filters, you must write a C function to proccess a signal and add the option in `filters/pedal.c` and then add it to the makefile in the filter listing and as a target.

**Custom filters are still a work in progress. It is not as flexible as it should be.**
