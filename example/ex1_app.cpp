#include "ArgParser.h"

struct AppConfig {
    bool isVerbose = false;
    int someInteger = 42;
    std::string inputFile = "input.file";
    std::string outputFile = "output.file";
};

static AppConfig glb_Config;

// example on how you can simply parse the arguments...
static void ParseConfig(int argc, const char **argv) {
    ArgParser argParser(argc, argv);
    glb_Config.isVerbose = argParser.TryParse("-v");
    glb_Config.someInteger = *argParser.TryParse("-n", "--number", glb_Config.someInteger);
    glb_Config.inputFile = *argParser.TryParse("-i", "--input", glb_Config.inputFile);
    glb_Config.outputFile = *argParser.TryParse("-o", "--output", glb_Config.outputFile);
}

static bool ParseConfigErrorHandling(int argc, const char **argv) {
    ArgParser argParser(argc, argv);
    glb_Config.isVerbose = argParser.TryParse("-v");

    // we can have an error while trying to parse a number - so this is how you can check it - if you want...
    auto res = argParser.TryParse("-n", "--number", glb_Config.someInteger);
    if (!res.has_value()) {
        printf("Something went wrong\n");
        return false;
    }
    glb_Config.someInteger = res.value();
    return true;
}


int main(int argc, const char **argv) {
    ArgParser argParser(argc, argv);

    if (argParser.TryParse("-h", "--help")) {
        printf("ArgParser Help\n");
        printf(" -v      Set app to verbose (default: %s)\n", glb_Config.isVerbose?"on":"off");
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