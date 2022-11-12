# CEGARBoxCPP

## Authorship
All files in this repository were authored by Junwei Xiong.
## Dependencies
This project requires minisat installed to run and compile. Instruction for installing minisat are available [here](https://github.com/niklasso/minisat).

## Compile
A statically compiled file is already available in the repo - compiled on Ubuntu 20.04.3.

Run ``./compile.sh`` to compile CEGARBox

## Input Formula
CEGARBox accepts file input. Input is terminated by a newline and valid input formula are defined by the following grammar:
```
Atom ::= Alphanumeric String
Formula ::=
 Atom || $true || $false || ~Formula ||
 [] Formula || <> Formula || 
 Formula | Formula || Formula & Formula || Formula => Formula || 
 Formula <=> Formula || (Formula) 
```

## Run Theorem Prover

``./main <input_file>``
 

## Benchmarks

MQBF, 3CNF and LWB_K benchmarks can be downloaded from [here](http://www.cril.univ-artois.fr/~montmirail/mosaic/#)
ALC benchmarks can be downloaded from [here](https://web.archive.org/web/20190305011522/http://users.cecs.anu.edu.au/~rpg/BDDTab/)

Use ``Experiment/convert.py`` to convert the benchmarks into a valid format.

## Experiments

Experiments can be run using Experiment/experiment.py and ``Exeriment/experiment_reflexive.py``
