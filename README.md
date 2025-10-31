# 🚀 My Language

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg)]()
[![License](https://img.shields.io/badge/license-MIT-blue.svg)]()
[![C++](https://img.shields.io/badge/C++-17-blue.svg)]()
[![CMake](https://img.shields.io/badge/CMake-3.15+-blue.svg)]()

**My Language** is a modern, educational programming language designed to teach developers how language compilers work under the hood. Built from scratch in C++17, it features a complete lexer, parser, and AST with beautiful tree visualization.

## ✨ Features

- 🔍 **Complete Lexer**: Tokenizes source code with support for keywords, identifiers, literals, operators, and delimiters
- 🌳 **Recursive Descent Parser**: Robust parsing with comprehensive error handling and recovery
- 🎯 **Rich AST**: Full Abstract Syntax Tree with visitor pattern implementation
- 📊 **Beautiful Tree Visualization**: ASCII tree output for debugging and learning
- 🧪 **Comprehensive Testing**: 70+ unit tests covering all language constructs
- ⚡ **Modern C++**: Built with C++17 standards and best practices

## 🏗️ Language Features

### Data Types & Literals
```mylang
// Integers and floats
let x: i32 = 42;
let pi: f64 = 3.14159;

// Strings and characters
let message: string = "Hello, My Language!";
let grade: char = 'A';

// Arrays and ranges
let numbers: i32[] = [1, 2, 3, 4, 5];
let range: i32[] = 0..10;
```

### Functions
```mylang
fn add(x i32, y i32) i32 {
    return x + y;
}

fn greet(name string) {
    outputln("Hello, " + name + "!");
}
```

### Control Flow
```mylang
// Conditionals
if (condition) {
    // do something
} elif {
    // do this instead
} else {
    // do something else
}

// Loops
while (i < 10) {
    i = i + 1;
}

let list: i32[] = [1, 2, 3]
for (i in list) {
    outputln(i);
}

for (0..10) {
    outputln("This prints 10 times!");
}
```

### Object-Oriented Programming
```mylang
rec Person {
    let pub name: str;
    let age: i32;
}

cls Calculator {
    fn pub calculate(x: i32, y: i32) i32 {
        return x + y;
    }
}
```

### Access Modifiers
- `pub` - Public access
- `pri` - Private access (default)
- `pro` - Protected access

### Type Modifiers
- `const` - Constant values
- `static` - Static members
- `init` - Initialize-only

## 🛠️ Building

### Prerequisites
- **C++ Compiler**: Supporting C++17 (GCC 7+, Clang 5+, MSVC 2017+)
- **CMake**: Version 3.20 or higher
- **Ninja**: Build system (recommended)
- **GoogleTest**: For running tests (automatically downloaded)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/yourusername/my_lang.git
cd my_lang

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -G Ninja

# Build the project
ninja

# Run tests
ctest
```

### Build Targets

- **Debug Build**: `cmake .. -DCMAKE_BUILD_TYPE=Debug`
- **Release Build**: `cmake .. -DCMAKE_BUILD_TYPE=Release`

## 🚀 Usage

### Compile and Run
```bash
# Compile a source file
./bin/my_lang source.ml

# Display AST tree
./bin/my_lang source.ml -g
```

### Example Session
```bash
$ .\build\Release\bin\my_lang examples\hello.ml -g
[1:1-1:3 (index 2)] Keyword fn
[1:4 (index 3)-1:8 (index 7)] Identifier main
[1:8 (index 7)-1:9 (index 8)] Delimiter (
[1:9 (index 8)-1:10 (index 9)] Delimiter )
[1:11 (index 10)-1:12 (index 11)] Delimiter {
[2:3 (index 14)-2:11 (index 22)] Identifier outputln
[2:11 (index 22)-2:12 (index 23)] Delimiter (
[2:12 (index 23)-2:33 (index 44)] String "Hello, My Language!"
[2:33 (index 44)-2:34 (index 45)] Delimiter )
[2:34 (index 45)-2:35 (index 46)] Delimiter ;
[3:1 (index 47)-3:2 (index 48)] Delimiter }
[3:2 (index 48)-3:2 (index 48)] Eof
Compilation finished.
Program
  FunctionDeclaration
    Identifier: main
    Type:
      Identifier: void
    Modifiers:
      ModifierStatement
        Accessor: private
        Modifiers:
          None
    Parameters:
      (empty)
    Body:
      BlockStatement
        ExpressionStatement
          CallExpression
            Callee:
              Identifier: outputln
            Arguments:
              Literal: ""Hello, My Language!""
Compilation successful!
Press Enter to exit...
```

## 📁 Project Structure

```
my_lang/
├── 📁 cli/                   # Command-line interface
├── 📁 examples/              # Example programs
├── 📁 include/ml/            # Header files
│   ├── 📁 ast/               # Abstract Syntax Tree
│   ├── 📁 basic/             # Basic utilities
│   ├── 📁 lexer/             # Lexical analyzer
│   └── 📁 parser/            # Parser components
├── 📁 src/                   # Source files
│   ├── 📁 ast/               # AST implementation
│   ├── 📁 basic/             # Basic utilities
│   ├── 📁 compiler/          # Main compiler
│   ├── 📁 lexer/             # Lexer implementation
│   └── 📁 parser/            # Parser implementation
├── 📁 tests/                 # Unit tests
├── 📁 tools/                 # Development tools
├── 🔧 CMakeLists.txt         # Build configuration
├── 📖 LICENSE                # Licensing information
└── 📖 README.md              # This file
```

## 🧪 Testing

The project includes comprehensive unit tests covering:

- **Lexer Tests**: Token generation and recognition
- **Parser Tests**: 45+ test cases for all language constructs
- **Core Tests**: Basic functionality and utilities
- **Integration Tests**: End-to-end compilation testing

```bash
# Run all tests
ctest

# Run specific test suite
ctest -R test_parser

# Verbose output
ctest -V
```

## 🎯 Grammar

The language implements a comprehensive grammar supporting:

- **Expressions**: Binary, unary, literals, identifiers, function calls
- **Statements**: Expression, block, return, break, continue
- **Declarations**: Variables, functions, records, classes
- **Control Flow**: If/else, while, for loops, switch statements
- **Operators**: Arithmetic (+, -, *, /), comparison (==, !=, <, >), logical (&&, ||)
- **Special Operators**: Range (..), attribute access (.), assignment (=)

## 🤝 Contributing

We welcome contributions! Here's how you can help:

1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/amazing-feature`)
3. **Commit** your changes (`git commit -m 'Add amazing feature'`)
4. **Push** to the branch (`git push origin feature/amazing-feature`)
5. **Open** a Pull Request

### Code Style
- Follow C++17 best practices
- Use meaningful variable and function names
- Add comments for complex logic
- Ensure all tests pass

## 📚 Learning Resources

This project is designed for educational purposes. Key learning areas include:

- **Lexical Analysis**: How tokenizers work
- **Parsing Techniques**: Recursive descent parsing
- **AST Design**: Tree structures and visitor patterns
- **Error Handling**: Compiler error reporting
- **Modern C++**: RAII, smart pointers, templates

## 🗺️ Roadmap

- [ ] **Code Generation**: LLVM IR backend
- [ ] **Type System**: Static type checking
- [ ] **Standard Library**: Built-in functions and modules
- [ ] **Optimization**: Basic compiler optimizations
- [ ] **IDE Support**: Language server protocol
- [ ] **Package Manager**: Module system

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Karson P. Califf** - *Creator and Lead Developer*

## 🙏 Acknowledgments

- **GoogleTest** - Testing framework
- **CMake** - Build system
- **C++ Community** - Inspiration and best practices
- **Compiler Design Books** - Theoretical foundation

---

<div align="center">

**[⭐ Star this repo](https://github.com/washed07/my_lang)** if you found it helpful!

Made with ❤️ for the programming language community

</div>