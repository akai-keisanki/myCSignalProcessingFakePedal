# ![`fake_fedal` logo](img/fake_pedal_icon.png) myCSignalProcessingFakePedal
![created at](https://img.shields.io/github/created-at/akai-keisanki/myCSignalProcessingFakePedal)
![license](https://img.shields.io/github/license/akai-keisanki/myCSignalProcessingFakePedal)
![release](https://img.shields.io/github/v/release/akai-keisanki/myCSignalProcessingFakePedal)
![top language](https://img.shields.io/github/languages/top/akai-keisanki/myCSignalProcessingFakePedal)
![forks](https://img.shields.io/github/forks/akai-keisanki/myCSignalProcessingFakePedal)
![stars](https://img.shields.io/github/stars/akai-keisanki/myCSignalProcessingFakePedal)

This project implements signal processing features in C for WAV files, emulating a pedalboard.

## Features

- WAV audio file signal processing
- Efficient live audio signal processing with PortAudio
- 10+ available DSP filters
- FPFDSL[^1] to store filter setups
- 5+ example FPFDSL files
- Support to custom DSP filters

## Setup

### Requirements

- PortAudio: required for the live processing option.
  - Install via `sudo apt install portaudio19-dev` for Debian/Ubuntu.

### Compiling The Project

To run this software, you must compile it from the source code.

You can clone it from GitHub:
```sh
git clone https://github.com/akai-keisanki/myCSignalProcessingFakePedal.git
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

> [!IMPORTANT]
> Due to technical difficulties, this program only processes 16b WAV files with 44B-header (44100Hz recommended). The file should be named `input.wav` and be in the working directory.
> 
> You can use Make to generate an `input.wav` from a `pre_input.wav` file in your input directory, formating it with `ffmpeg` to the required conventions: 
> ```sh
> make input.wav
> ```
> 
> An `output.wav` shall be generated after `fake_pedal`'s running is finished with exit code `0`.

### Command-Line Usage

`fake_pedal` requires a filter input option as its first argument and a following IO option to work with.

The filter input options are:

- `f`: read from a following file (name) with the FPFDSL[^1] format.

The IO options are:

- `f`: process files. Must be followed by 2 arguments: the input and output proper WAV files.
- `l`: process and play live audio with PortAudio.

You can get a brief description of the command line structure with `fake_pedal help`.

### Using Filters

#### Available filters

- `multiply`
  - FPFDSL Identifiers: `Multiply`, `Multiplier`, `Gain`
  - Parameters:
    - `mul`/`multiplier`/`coefficient`/`gain`

- `clip`
  - FPFDSL Identifiers: `Clip`, `Clipper`, `HardClip`, `HardClipper`
  - Parameters:
    - `thr`/`threshold` as a fraction of the total signal

- `low_pass`
  - FPFDSL Identifiers: `LowPass`, `HighCut`
  - Parameters:
    - `cut`/`hcut`/`high_cut` as a fraction
    - `mix`

- `high_pass`
  - FPFDSL Identifiers: `HighPass`, `LowCut`
  - Parameters:
    - `cut`/`lcut`/`low_cut` as a fraction
    - `mix`

- `mid_scoop`
  - FPFDSL Identifiers: `MidScoop`, `MidScooper`
  - Parameters:
    - `lcut`/`low_cut` as a fraction
    - `hcut`/`high_cut` as a fraction
    - `mix`

- `bit_crush`
  - FPFDSL Identifiers: `BitCrush`, `BitCrusher`, `LoFi`
  - Parameters:
    - `res`/`resolution`/`crush` as an ammount of bits
    - `mix`

- `pitch_shift` (BETA)
  - FPFDSL Identifiers: `PitchShift`, `PitchShifter`
  - Parameters:
    - `uni`/`unities`/`semitones` as an integer

- `saturate`
  - FPFDSL Identifiers: `Saturate`, `Saturator`
  - Parameters:
    - `mul`/`multiplier`/`coefficient`/`gain`
    - `mix`

- `delay`
  - FPFDSL Identifiers: `Delay`, `Delayer`
  - Parameters:
    - `dly`/`delay`/`steps` as an unsigned integer
    - `mix`

- `drive`
  - FPFDSL Identifiers: `Drive`, `Driver`, `OverDrive`, `OverDriver`
  - Parameters:
    - `thr`/`threshold` as a fraction of the total signal
    - `mix`

- `compress`
  - FPFDSL Identifiers: `Compress`, `Compressor`
  - Parameters:
    - `rat`/`ratio` (positive for downwards, negative for upwards)
    - `thr`/`threshold` as a fraction of the total signal
    - `atk`/`attack` as a fraction (higher imples quicker changes)
    - `mix`

- `auto_wah`
  - FPFDSL Identifiers: `AutoWah`, `AutomaticWah`
  - Parameters:
    - `atk`/`attack` as a fraction (higher imples quicker changes)

- `harmonize` (BETA)
  - FPFDSL Identifiers: `Harmonize`, `Harmonizer`, `Octave`
  - Parameters:
    - `uni`/`unities`/`semitones` as an integer
    - `mix`

#### FPFDSL[^1]

The FPFDSL is a straightfoward and visual format where every name, value or delimiter -- some element -- is represented by a string separated by spaces from the other elements.

It's details will not be covered by this description, but some examples are available in this repository in the [`fpfdsl_examples/`](fpfdsl_examples/) directory.

## Extra: Adding your own filters

In order to add your own filters, you must write `.c` and `.h` files (e.g. `my_filter.c` and `my_filter.h`) in the [`filters/`](filters/) directory and update the [`filters/filter_includes.def`](filters/filter_includes.def) and [`filters/available_filters.def`](filters/available_filters.def) files with the filter's information.

Your filter's `.h` file must include `util_filter_base.h` and define a `struct filter *init_filter_my_filter(...)` (where `my_filter` must be named the same your files) function that takes the filter's parameters as arguments.

The `.def` files have an own structure that must be followed. Check the other listed filters there for more detail.


[^1]: FPFDSL stands for Fake Pedal Filter Domain-Specific Language.
