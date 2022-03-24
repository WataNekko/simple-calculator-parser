# simple-calculator-parser
A simple 4-function calculator parser program to demonstrate the application of the Abstract Syntax Tree

## NOTE
Compile with C++17

```
cd src/
g++ -std=c++17 -Wall main.cpp calculator/*.cpp -o calc
```

Then run the REPL with `./calc`, or optionally specify the file name to which to output the AST in JSON format with an additional argument `./calc output.json`.
