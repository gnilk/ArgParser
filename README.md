# ArgParser
A single header library to simplify argument parsing for smaller applications.

Handles most cases except:
- multiple occurrences (like '-vvv' for increased verboseness)
- capturing all end at the end (like specifying multiple input-files)

## Build
Just copy the header into your code or specify the include directory to your checkout directory

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