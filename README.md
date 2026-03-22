# myCSignalProcessingFakePedal

This project implements signal processing features in C for WAV files, emulating a pedalboard.

## Setup

### Requirements

- SoX (Sound eXchange): required for the live processing option.
  - <https://sox.sourceforge.net>
  - Install via `sudo apt install sox` for Debian/Ubuntu or `brew install sox` for macOS.

### Compiling the project

To run this software, you must compile it from the source code.

You can clone it from GitHub:
```
sh git clone https://github.com/akai-keisanki/myCSignalProcessingFakePedal.git
```

And `cd` into the directory:
```sh
cd myCSignalProcessingFakePedal/
```

Then you can build it with Make and (G)CC:
```sh
make fake_pedal
```

## Usage

### Using the right files

Due to technical difficulties, this program only processes 16b WAV files with 44B-header (44100Hz recommended). The file should be named `input.wav` and be in the working directory.

You can use Make to generate an `input.wav` from a `pre_input.wav` file in your input directory, formating it with `ffmpeg` to the required conventions:
```sh
make input.wav
```

An `output.wav` shall be generated after `fake_pedal`'s running is finished with exit code `0`.

### Command-line usage

`fake_pedal` requires a filter list string as its first argument and a following option to work.
The options are:

- `f`: process files. Must be followed by 2 arguments: the input and output proper WAV files.
- `l`: process and play live audio with SoX.

You can get a brief description of the command line structure with `fake_pedal help`.

### Using filters

Each filter in the filter list string is represented by its character immediately followed by 1-2 4-digit numeric values. They are listed in order of application.

Here is a simple list of the currently available filters:

- `multiply`: Type `M` followed by a 4-digit value representing the percent of the multiplier applied to the signal.

- `clip`: Type `C` followed by a 4-digit value representing the per ten thousand of the maximum signal (`1.0`) that should be the new maximum (and negative minimum), clipping higher values to it. It is automatically always used to clip the output to `1.0` after all operations.

- `low_pass`: Type `l` followed by a 4-digit value representing the per ten thousand of the high cut coefficient. [^1]

- `high_pass`: Type `h` followed by a 4-digit value representing the per ten thousand of the low cut coefficient. [^1]

- `mid_scoop`: Type `m` followed by 2 4-digit values representing the per ten thousand of the low and high cut coefficients. [^1]

- `pitch_shift` (BETA): Type `P` followed by a digit representing the signal (`0` for `+`, `1` or other for `-`) and 3 digits representing the per ten of the number of semitones to shift.

- `saturate`: Type `S` followed by a 4-digit value representing the percent of the multiplier applied to the signal that is saturated with the hyperbolic tangent.

- `delay`: Type `d` followed by 2 4-digit values representing the number of steps (that will be multiplied by 10) to be delayed and the per ten thousand of the multiplicator to be applied to the delayed signal to be summed.

- `distortion`: Type `D` followed by a 4-digit value representing the per ten thousand of the maximum signal (`1.0`) that should be the new maximum (and negative minimum), clipping higher values to it and blending with the signal.

- `harmonize`: Type `H` followed by 2 4-digit values: the first is a digit representing the signal (`0` for `+`, `1` or other for `-`) and 3 digits representing the per ten of the number of semitones to shift to the added signal and the second is the per ten thousand of the multiplicator to be applied to the shifted signal to be summed.

> [!TIP]
> Characters without use such as `-`, `,` or `.` can be used as separators.

[^1]: Must not be used 2 or more times in the same execution of the pedal (which results in unexpected behavior).

### Examples

Example filter list strings that you can try:

- Fuzz: `S0300,C0300,M0150,l1500`;
- Warm tone: `M0150,d04003333,l0200,m01000300`;
- Nasal drive: `S0300,l0700,h0700,D0500`;
- Lo-fi tone: `h0600,l0900,S0150,C1000`;
- Warm drive: `l2000,S0150,D0800,m02000700`;
- Bigger impression: `H10125000,S0150,m02000200,d03001000`;
- Hard drive: `S0300,d03005000,C0300,M0200,m02000200,l1500,M0200`;

## Extra: Adding your own filters

In order to add your own filters, you must write a C function to proccess a signal and add the option in `filters/pedal.c` and then add it to the makefile in the filter listing and as a target.

> [!WARNING]
> **Custom filters are still a work in progress. It is not as flexible as it should be.**

## Acknowledgements

- This project utilizes [SoX (Sound eXchange)](https://sox.sourceforge.net) for high-performance live audio signal IO via command-line piping.
