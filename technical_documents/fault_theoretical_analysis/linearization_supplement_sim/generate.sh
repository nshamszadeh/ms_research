#!/usr/bin/env bash
# Regenerates the figure data and the numbers quoted in linearization_supplement.tex
# and Section 1.2 of neuron_dropout_FOR.tex. Takes several minutes.
# Usage: ./generate.sh [output directory, default ./data]
set -euo pipefail
here="$(cd "$(dirname "$0")" && pwd)"
out="${1:-$here/data}"
mkdir -p "$out"
gcc -O2 -o "$out/neuron" "$here/neuron.c" -lm
cd "$out"

# Figures (alpha = 0.5 unless ALPHA is set)
./neuron trace 0.22 0.1 > trace.dat                 # Figure 1
./neuron fi 0 0.1 > fi.dat                          # Figure 2
./neuron sine 0.22 0.1 > gain_a05.dat               # Figure 3, alpha = 0.5
ALPHA=1 ./neuron sine 0.12 0.1 > gain_a1.dat        # Figure 3, alpha = 1

# Numbers quoted in the text
{
  ./neuron stats 0.22 0.1
  ALPHA=1 ./neuron stats 0.12 0.1
  echo "-- windowed regression, alpha = 0.5"
  ./neuron winreg 0.22 0.1
  echo "-- windowed regression, alpha = 1"
  ALPHA=1 ./neuron winreg 0.12 0.1
  echo "-- drive a target receives: filtered spikes against the linear prediction"
  ./neuron drive 0.22 0.1
  ALPHA=1 ./neuron drive 0.12 0.1
  echo "-- 1000-step windows, recorded runs of 2e5 steps, six seeds"
  for seed in 11 22 33 44 55 66; do
    NSTEPS=200000 SEED=$seed ./neuron winreg 0.22 0.1 | grep 'window  1000'
  done
} > numbers.txt
echo "wrote $out"
