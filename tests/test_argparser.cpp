#include "ArgParser.h"
#include <testinterface.h>

extern "C" int test_argparser(ITesting *t) {
    return kTR_Pass;
}

extern "C" int test_argparser_simple(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-h",
            NULL,
    };
    ArgParser argParser(2,argv);
    TR_ASSERT(t, argParser.TryParse("-h"));

    return kTR_Pass;
}

extern "C" int test_argparser_integer_short(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-i",
            "45",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-i","--integer", 60) == 45);
    // this is not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse("-u","--integer", 60) == 60);

    return kTR_Pass;
}

// same as 'short' but using the long name instead of short name
extern "C" int test_argparser_integer_long(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "--integer",
            "45",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-i","--integer", 60) == 45);
    // this is not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse("-u","--unteger", 60) == 60);

    return kTR_Pass;
}

extern "C" int test_argparser_string_short(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-n",
            "SomeName",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-n","--name", std::string("DefaultName")) == "SomeName");
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse("-s","--string", std::string("DefaultName")) == "DefaultName");

    return kTR_Pass;
}

// same as 'short' but using the long name instead of short name
extern "C" int test_argparser_string_long(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "--name",
            "SomeName",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-n","--name", std::string("DefaultName")) == "SomeName");
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse("-s","--string", std::string("DefaultName")) == "DefaultName");

    return kTR_Pass;
}

extern "C" int test_argparser_float_short(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-f",
            "12.3",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-f","--float", 45.6) == 12.3);
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse("-d","--double", 67.8) == 67.8);

    return kTR_Pass;
}

// same as 'short' but using the long name instead of short name
extern "C" int test_argparser_float_long(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "--float",
            "12.3",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-f","--float", 45.6) == 12.3);
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse("-d","--double", 67.8) == 67.8);

    return kTR_Pass;
}


extern "C" int test_argparser_single_in_middle(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-abc",
            NULL,
    };
    ArgParser argParser(2,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse("-b"));
    // this should is not be present so we should return the default
    TR_ASSERT(t, !argParser.TryParse("-u"));

    return kTR_Pass;
}

extern "C" int test_argparser_wrong_argc(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "--name",
            "SomeName",
            NULL,
    };
    ArgParser argParser(2,argv);

    // this should fail - note: test will pass but an error msg will be sent to stderr
    TR_ASSERT(t, !argParser.TryParse("-n", "--name", std::string("default")));

    return kTR_Pass;
}
