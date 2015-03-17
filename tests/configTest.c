/*
    configTest.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "../config.h"

#define test(message, expr) fprintf(stderr, "TEST: %s: ("#expr")\n", message);\
                            assert(expr);\
                            fprintf(stderr, " + PASSED\n");

int main(int argc, char **argv) {
    enum ConfigurationStatus status;

    ConfigurationRef config = configCreate();
    test("Config should not be null", config != NULL);

    status = configLoadString(config, "port: 51717\ntest: option");
    test("Should report errors for non-existent options", status == CONFIG_NO_SUCH_OPTION_ERROR);

    status = configLoadString(config, "port: derp");
    test("Should report error for non-number ports", status == CONFIG_INVALID_VALUE_ERROR);

    status = configLoadString(config, "port: 51717");
    test("Should report no errors", status == CONFIG_SUCCESS);

    unsigned short port = configPort(config);
    test("Port should be assigned correctly", port == 51717);

    return 0;
}