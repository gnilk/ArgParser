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
    TR_ASSERT(t, argParser.IsPresent("-h"));

    return kTR_Pass;
}

extern "C" int test_argparser_simple2(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-h",
            NULL,
    };
    ArgParser argParser(2,argv);
    TR_ASSERT(t, argParser.IsPresent("-h","--help"));

    return kTR_Pass;
}

extern "C" int test_argparser_integer_nodefault(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "-i",
            "45",
            NULL,
    };
    ArgParser argParser(3,argv);
    // this is present, so we should grab it
    TR_ASSERT(t, argParser.TryParse<int>("-i","--integer") == 45);
    // this is not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse<int>(60, "-u","--integer") == 60);

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
    TR_ASSERT(t, argParser.TryParse(60, "-i","--integer") == 45);
    // this is not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse(60, "-u","--integer") == 60);

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
    TR_ASSERT(t, argParser.TryParse(60, "-i","--integer") == 45);
    // this is not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse(60, "-u","--unteger") == 60);

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
    TR_ASSERT(t, argParser.TryParse(std::string("DefaultName"), "-n","--name") == "SomeName");
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse(std::string("DefaultName"), "-s","--string") == "DefaultName");

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
    TR_ASSERT(t, argParser.TryParse(std::string("DefaultName"), "-n","--name") == "SomeName");
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse(std::string("DefaultName"), "-s","--string") == "DefaultName");

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
    TR_ASSERT(t, argParser.TryParse(45.6, "-f","--float") == 12.3);
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse(67.8, "-d","--double") == 67.8);

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
    TR_ASSERT(t, argParser.TryParse(45.6, "-f","--float") == 12.3);
    // this should not be present so we should return the default
    TR_ASSERT(t, argParser.TryParse(67.8, "-d","--double") == 67.8);

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
    TR_ASSERT(t, argParser.IsPresent("-b"));
    // this should is not be present so we should return the default
    TR_ASSERT(t, !argParser.IsPresent("-u"));

    return kTR_Pass;
}

extern "C" int test_argparser_wrong_argc(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "--name",
            "SomeName",
            NULL,
    };
    ArgParser argParser(2,argv);    // to few arguments here

    // this should fail - note: test will pass but an error msg will be sent to stderr
    TR_ASSERT(t, !argParser.TryParse(std::string("default"), "-n", "--name"));

    return kTR_Pass;
}

extern "C" int test_argparser_catchmany(ITesting *t) {
    const char *argv[]= {
            "prgname.exe",
            "--input",
            "one",
            "two",
            "three",
            "-o",
            "other",
            NULL,
    };
    ArgParser argParser(7,argv);

    // this should fail - note: test will pass but an error msg will be sent to stderr
    std::vector<std::string> names;
    TR_ASSERT(t, argParser.TryParse(names, "-i", "--input") > 0);
    for(auto &s : names) {
        printf("%s\n", s.c_str());
    }
    return kTR_Pass;
}

extern "C" int test_argparser_count_simple(ITesting *t) {
    const char *argv_simple[]= {
            "prgname.exe",
            "-vvv",
            NULL,
    };
    ArgParser argParser(2,argv_simple);

    // Count the presence of 'v'
    auto num = argParser.CountPresence("-v");
    printf("NumV: %d\n", num);
    TR_ASSERT(t, num == 3);

    return kTR_Pass;
}

extern "C" int test_argparser_count_complex(ITesting *t) {
    const char *argv_simple[]= {
            "prgname.exe",
            "-aFv",
            "-vbc",
            "-dvK",
            NULL,
    };
    ArgParser argParser(4,argv_simple);

    // Count the presence of 'v' across all arguments
    auto num = argParser.CountPresence("-v");
    printf("NumV: %d\n", num);
    TR_ASSERT(t, num == 3);

    return kTR_Pass;
}

extern "C" int test_argparser_copyend_simple(ITesting *t) {
    const char *argv_simple[]= {
            "prgname.exe",
            "file1",
            "file2",
            "file3",
            NULL,
    };
    ArgParser argParser(4,argv_simple);

    // Count the presence of 'v' across all arguments
    std::vector<std::string> filenames;
    auto num = argParser.CopyEndArgs(filenames);
    printf("NumV: %d\n", num);
    TR_ASSERT(t, num == 3);
    for(auto &s : filenames) {
        printf("  %s\n", s.c_str());
    }

    return kTR_Pass;
}

extern "C" int test_argparser_copyend_complex(ITesting *t) {
    const char *argv_simple[]= {
            "prgname.exe",
            "-abf",
            "file1",
            "file2",
            "file3",
            "file4",
            NULL,
    };
    ArgParser argParser(6,argv_simple);

    // Count the presence of 'v' across all arguments
    std::vector<std::string> filenames;
    auto num = argParser.CopyEndArgs(filenames);
    printf("NumV: %d\n", num);
    TR_ASSERT(t, num == 4);
    for(auto &s : filenames) {
        printf("  %s\n", s.c_str());
    }

    return kTR_Pass;
}

extern "C" int test_argparser_copyend_complex2(ITesting *t) {
    const char *argv_simple[]= {
            "prgname.exe",
            "-i",               // note: that -i takes an argument '-i <file1>'
            "file1",
            "-flags",
            "file2",
            "file3",
            "file4",
            NULL,
    };
    ArgParser argParser(7,argv_simple);

    // Count the presence of 'v' across all arguments
    std::vector<std::string> filenames;
    auto num = argParser.CopyEndArgs(filenames);
    printf("NumV: %d\n", num);
    TR_ASSERT(t, num == 3);
    for(auto &s : filenames) {
        printf("  %s\n", s.c_str());
    }

    return kTR_Pass;
}

// this semi-supported, a helper exists to verify this type of argument parsing
extern "C" int test_argparser_copyend_complex3(ITesting *t) {
    const char *argv_simple[]= {
            "prgname.exe",
            "-i",               // note: that -i takes an argument '-i <file1>'
            "input1",           // when doing 'copyend' this will also be copied...
            "output1",
            "output2",
            "output3",
            NULL,
    };
    ArgParser argParser(6,argv_simple);

    // Count the presence of 'v' across all arguments
    std::vector<std::string> filenames;
    auto num = argParser.CopyEndArgs(filenames);

    bool bIsLast = argParser.IsLastArgument("-i");
    TR_ASSERT(t, bIsLast);

    if (bIsLast) {
        // expect one more
        printf("EXPECT ONE MORE!\n");
    }
    printf("NumV: %d\n", num);
    TR_ASSERT(t, num == 4);
    for(auto &s : filenames) {
        printf("  %s\n", s.c_str());
    }

    return kTR_Pass;
}
