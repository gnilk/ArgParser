# ArgParser
A state-less single header library to simplify argument parsing for smaller applications.

Features:
- Flags present/non-present (on/off toggles)
- Count the presence of an option across all arguments (like; verbose level)
- Arguments carrying single value
- Catch all at the end

## Examples
Check unit tests in 'test_argparser.cpp' or the example in 'example/ex1_app.cpp'.

## Build
Just copy the header into your code or specify the include directory to your checkout directory/src

## Testing
You need 'https://github.com/gnilk/testrunner' in order to run the unit tests.

Just build the example application:
'''bash
mkdir bld
cd bld
cmake ..
make ex1
./ex1
'''

## Notes
This a simple library and covers most use cases. As it is stateless it comes with one noticeable drawback. In the case of processing
multiple sources to some configurable output. You normally want to catch all end-arguments (as they specify the inputs).

Like:
```shell
./app -o myoutput.bin file1 file2 file3 file4 ...
```

In this case the `CopyEndArgs` don't know if '-o' has a parameter value or not (as the argument parse is stateless).
Thus, `myutput.bin` will be part of the intput array obtained with `CopyEndArgs`. For simple use cases one can check if `-o` is 
the last argument present and discard the first value coming out from `CopyEndArgs`. But in case you have this type use-case
for you application your are probably better off writing your own custom arg-parsing.

# API

## Overview
```c++
ArgParser(size_t argc, const char **argv)
bool IsPresent(const std::string &shortParamName, const std::string &longParamName = {}) const
std::optional<TValue> TryParse(const std::string &shortParamName, const std::string &longParamName = {}) const {
std::optional<TValue> TryParse(const TValue &&defaultValue, const std::string &shortParamName, const std::string &longParamName = {}) const {
std::optional<TValue> TryParse(const TValue &defaultValue, const std::string &shortParamName, const std::string &longParamName = {}) const {
int TryParse(std::vector<TValue> &outValues, const std::string &shortParamName, const std::string &longParamName = {}) const {
int CountPresence(const std::string &shortParamName, const std::string &longParamName = {}) const {
int CopyEndArgs(std::vector<TValue> &outValues) const {
bool IsLastArgument(const std::string &shortParamName, const std::string &longParamName = {}) const {
```


## Constructor
```c++
ArgParser(size_t argc, const char **argv)
```
Create the arg parser object, pass the arg/argv from main.

## IsPresent - check if an option is present
Checks if an option is present on the command line.

```c++
    bool IsPresent(const std::string &shortParamName, const std::string &longParamName = {}) const
```

Use like:
```c++
config.some_feature = argParser.IsPresent("-f", "--feature");
```

## TryParse - Parse an option with a single value
Parse an argument with an optional value (or default). The default is used if the argument is not found on the commandline.
The `std::optional` return value is invalid if the argument was found but could not be copied or if the argument was found but no value supplied.

TryParse supports; string, integer, booleans and floating point numbers
```c++
    std::optional<TValue> TryParse(const std::string &shortParamName, const std::string &longParamName = {}) const {
    std::optional<TValue> TryParse(const TValue &&defaultValue, const std::string &shortParamName, const std::string &longParamName = {}) const {
    std::optional<TValue> TryParse(const TValue &defaultValue, const std::string &shortParamName, const std::string &longParamName = {}) const {
```

Use like:
```c++
config.name = argParser.TryParse("myname", "-n", "--name");
```

If found the `config.name` will be set to the commandline parameter otherwise the default will be used.

## TryParse - multiple values
If your application supports multiple values (OF THE SAME TYPE) for some argument you can pass a vector. The ArgParser will copy everything
after the argument up to the start of the next argument. 
```c++
    int TryParse(std::vector<TValue> &outValues, const std::string &shortParamName, const std::string &longParamName = {}) const {
```

Use like:
```c++
std::vector<std::string> input_files;
argParser.TryParse(input_files, "-i", "--input");
```

The above would catch `file1`, `file2` and `file3` and stop by `-n`.
```shell
./app -i file1 file2 file3 -n 45
```

## CountPresence
The number of times an argument is present.
```c++
    int CountPresence(const std::string &shortParamName, const std::string &longParamName = {}) const {
```

Use like:
```c++
int verboseLevel = argParser.CountPresence("-v");
```

## CopyEndArgs
Copy all arguments from the last argument (anything starting with '-') to the end.

<b>Note:<b>IF the last argument has a parameter that parameter WILL be included in the output.
```c++
    int CopyEndArgs(std::vector<TValue> &outValues) const {
```

Use Like:
```c++
std::vector<std::string> filesToProcess;
argParser.CopyEndArgs(filesToProcess);
```


## IsLastArgument
Checks if an argument is the last argument. This can help to offset the problem of `CopyEndArgument`.
```c++
    bool IsLastArgument(const std::string &shortParamName, const std::string &longParamName = {}) const {
```

