# rrlang

High-level language designed to be ran on my custom CPU ([Matrx007/rrcpu](https://github.com/Matrx007/rrcpu)). The compiler is currently aimed to compile into assembly ([Matrx007/rrasm](https://github.com/Matrx007/rrasm)), rather than straight into machine code.

This language is aimed towards readability:
1. Variable names before types in definitions:  
    `let aVariableName = 0: ThisIsAType;`  
    
2. `fn` keyword before function definitions helps to tell them apart from variables

3. Function names before types in definitions:  
    `fn aFunctionName(aParameter: ThisIsAType): TheReturnType;`
    
4. Forget about std::pair, introducing tuples:  
    `fn find(key: u8*): [key: u8*, val: void*];`
    
5. Nested functions to make code more organized and scope less bloated

6. Simpler syntax overall:  
    `struct Vector { x: f32, y: f32, z: f32 };`  
    Initialize it using correctly ordered tuple:  
    `let vec: Vector = [0.34, 0.756, 0.23];`  
    or using anonymous struct   
    `let vec: Vector = {x: 0.34, z: 0.23, y: 0.756};`
    
7. Easy to work with  
    Fixed type tuples:  
    `fn sendUserDataTuple(data: [u32, u8*, u8, u8, u32, u32]): u8;`  
    Or anonymous structs:  
    `fn sendUserDataStruct(data: {id: u32, name: u8*, age: u8, gender: u8, mother: u32, father: u32}): u8;`  
    Both functions accept structs and tuples as parameters.  
    `sendUserDataTuple({id: 4389437, name: "Rainis Randmaa", age: 17, gender: 1, mother: 8376234, father: 7843478});`  
    `sendUserDataStruct([4389437, "Rainis Randmaa", 17, 1, 8376234, 7843478]);`  

8. Built-in assembly literal, to write more optimized functions

# Compiling

I have not written a makefile to speed up compilation, so right now just use
```
g++ -Wall -std=c++2a src/parser.cpp -o parser
```
or
```
clang++ -Wall -std=c++20 src/parser.cpp -o parser
```

To test AST parser, run
```
./parser tests/std_strings.rr
```

# Roadmap

1. Parsing to AST:  

    Utility functions: ✅ Done  
    Parser MVP: ✅ Done  
    Full syntax support: 🛠 W.I.P  
2. Parsing to logic tree:  

    Utility functions: 🛠 W.I.P  
    Parser MVP: 🛠 W.I.P  
    Full language coverage: ❌ TO-DO
3. Translating into assembly:  

    Document instruction set: ✅ Done  
    Utility functions: ❌ TO-DO   
    Translator MVP: ❌ TO-DO   
    Full assembly coverage: ❌ TO-DO

# Future ideas

(Just fantasysing)

1. Rewrite the compiler in rrlang (Self-hosting compiler)
2. Interpreter for rrlang
3. Option to compile into LLVM, which then can be compiled using Clang to all supported platforms