# Single-neuron simulation for the linearization supplement

`neuron.c` simulates one GL-FLIF neuron and tracks its actual potential and its free potential side by side. It produces the three figures of `../linearization_supplement.tex` and the numbers about measuring the gain γ in that supplement and in Section 1.2 of `../neuron_dropout_FOR.tex`.

Run `./generate.sh` to rebuild everything into `data/`, or `./generate.sh <dir>` to write elsewhere. It needs `gcc` and takes several minutes. The random-number generator is seeded, so the output is the same on every run.

| Output | Used for |
|---|---|
| `trace.dat` | Figure 1: free and actual potential over 120 steps |
| `fi.dat` | Figure 2: f–I curve with and without input noise |
| `gain_a05.dat`, `gain_a1.dat` | Figure 3: gain from free potential to rate against input period |
| `numbers.txt` | operating points, γ from the sweep, step-by-step and windowed regression slopes, and the drive a target receives compared with the linear prediction (main document, Section 6.4) |

The figure coordinates are pasted into the `.tex` file, so the supplement compiles without this folder.
