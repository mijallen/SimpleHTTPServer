/*
    configTest.c
*/

#include <stdlib.h>
#include <assert.h>
#include "../config.h"

int main(int argc, char **argv) {
    const char *testString = "test: string";
    ConfigurationRef configRef = parseConfigurationString(testString);
    assert(configRef != NULL);
    return 0;
}