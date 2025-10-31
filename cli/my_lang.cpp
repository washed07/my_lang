#include "ml/compiler/compiler.h"

ml::compiler::Configuration parseArgs(int argc, char **argv) {
  ml::compiler::Configuration config;

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "--debug" || arg == "-g") {
      config.debug = true;
    }
  }

  return config;
}

int main(int argc, char **argv) {
  ml::compiler::Configuration config = parseArgs(argc, argv);
  ml::compiler::Compiler compiler;

  if (argc < 2) {
    std::cerr << "Usage: my_lang <file>" << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
    return 1;
  }

  std::string file_path = argv[1];
  auto program = compiler.compileFile(file_path, config);

  if (program) {
    std::cout << "Compilation successful!" << std::endl;
  } else {
    std::cerr << "Compilation failed." << std::endl;
  }

  if (config.debug) {
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
  }

  return 0;
}