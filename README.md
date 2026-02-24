# myCSignalProcessingFakePedal

I want to make a fake pedal in C to make some effects in my WAV files.

## Compiling the project

To run this software, you must compile it from the source code.

You can clone it from GitHub:
```sh
git clone https://github.com/akai-keisanki/myCSignalProcessingFakePedal.git
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

Due to technical difficulties, this program can only process 16b WAV files with a of exactly 44B. The file should be named `input.wav` and be in the working directory.

You can use Make to generate an `input.wav` from a `pre_input.wav` file in your input directory, formating it with `ffmpeg` to the required conventions:
```sh
make input.wav
```

An `output.wav` shall be generated after running `fake_pedal` is finished with exit code `0`.

## Working with filters

Just after starting `fake_pedal`, the program will read a string which represents a list of filters to apply, represented by their characters, possibly immediately followed by 1-2 4-digit numeric values.

Here is a simple list of the currently available filters:

- `multiply`: Type `M` followed by a 4-digit value representing the per mille of the multiplier applyed to the signal.

- `clip`: Type `C` followed by a 4-digit value representing the per ten thousand of the maximum signal (`1.0`) that should be the new maximum (and negative minimum), clipping higher values to it. It is automatically always used to clip the output to `1.0` after all operations.

- `equalise`: Type `E` followed by 2 4-digit values representing the per mille of multipliers applyed to the high and

- `drive`: Type `D` followed by a 4-digit value representing the per ten thousand of the maximum signal (`1.0`) that should be the new maximum (and negative minimum), flattening higher values to it.

## Adding your own filters

To add your own filters, you must write a C function to proccess a signal and add the option in `filters/pedal.c` and then add it to the makefile in the filter listing and as a target.

**Custom filters are still a work in progress. It is not as flexible as it should be.**
