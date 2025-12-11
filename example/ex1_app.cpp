#include "ArgParser.h"

struct AppConfig {
    int verboseLevel = 0;
    int someInteger = 42;
    std::string inputFile = "input.file";
    std::string outputFile = "output.file";
};

static AppConfig glb_Config;

// example on how you can simply parse the arguments...
static void ParseConfig(int argc, const char **argv) {
    ArgParser argParser(argc, argv);
    glb_Config.verboseLevel = argParser.CountPresence("-v");
    glb_Config.someInteger = *argParser.TryParse(glb_Config.someInteger, "-n", "--number");
    glb_Config.inputFile = *argParser.TryParse(glb_Config.inputFile, "-i", "--input");
    glb_Config.outputFile = *argParser.TryParse(glb_Config.outputFile, "-o", "--output");
}

static bool ParseConfigErrorHandling(int argc, const char **argv) {
    ArgParser argParser(argc, argv);
    glb_Config.verboseLevel = argParser.CountPresence("-v");

    // we can have an error while trying to parse a number - so this is how you can check it - if you want...
    auto res = argParser.TryParse(glb_Config.someInteger, "-n", "--number");
    if (!res.has_value()) {
        printf("Something went wrong\n");
        return false;
    }
    glb_Config.someInteger = res.value();
    return true;
}


int main(int argc, const char **argv) {
    ArgParser argParser(argc, argv);

    if (argParser.IsPresent("-h", "--help")) {
        printf("ArgParser Help\n");
        printf(" -v      Verbose level (default: %d)\n", glb_Config.verboseLevel);
        printf(" -n      Define 'number' (default: %d)\n", glb_Config.someInteger);
        printf(" -i      Input filename (default: %s)\n", glb_Config.inputFile.c_str());
        printf(" -o      Output filename (default: %s)\n", glb_Config.outputFile.c_str());
        printf("\n");
        printf(" -n/--number    define some integer\n");
        printf(" -i/--input     define the input filename\n");
        printf(" -o/--output    define the output filename\n");
        return 0;
    }
    printf("Try starting with '-h' or '--help'\n");
    return 1;
}