# SUPAC++ Labs 1 and 2

## Compile
```
make
```

## Run
```
./AnalyseData
```
By default the program reads `input2D_float.txt` and `error2D_float.txt` from
`/workspaces/SUPACPP_Labs/Exercises/Lab1and2/`. To use other files:
```
./AnalyseData path/to/input.txt path/to/errors.txt
```

Menu: print N lines, magnitudes, straight-line fit, and x^y. Options 2-4 also write `magnitudes.txt`, `line_fit.txt` and `x_to_the_y.txt` to the current directory.

## Files
- `AnalyseData.cxx/.h` - main steering program and menu helpers
- `CustomFunctions.cxx/.h` - reading, printing, analysis and saving functions
- `Makefile` - build script (`make clean` removes build products)