# Simple Calculator Parser
A simple 4-function calculator parser program to demonstrate the application of the Abstract Syntax Tree (AST).

## Calculator program

The calculator program in [src/](src/) is to be compiled with *C++17*. E.g.,

```
cd src/
g++ -std=c++17 -Wall main.cpp calculator/*.cpp -o calc
```

Then run the REPL with `./calc`, or optionally specify the file name to which to output the AST in JSON format with an additional argument (E.g., `./calc output.json`).

## AST Visualization

The outputted AST in the .json file can be visualized with the Python script [visualize_ast.py](src/visualize_ast.py) (E.g., `python visualize_ast.py output.json`). See help with the command `python visualize_ast.py -h`.
