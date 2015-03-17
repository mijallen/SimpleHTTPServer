/*
    config.c
*/

#include <stdlib.h>
#include <assert.h>
    #include <stdio.h>
#include <yaml.h>
#include "config.h"

struct Configuration {
    unsigned short port;
};

/* Internal Functions */

ConfigurationRef createConfiguration() {
    ConfigurationRef configRef = malloc(sizeof(struct Configuration));
    assert(configRef != NULL);

    return configRef;
}

void destroyConfiguration(ConfigurationRef configRef) {
    assert(configRef != NULL);
    free(configRef);
}

/* Exported Functions */

ConfigurationRef parseConfigurationString(const char *inputString) {
    assert(inputString != NULL);

    yaml_parser_t parser;
    yaml_event_t event;
    yaml_parser_initialize(&parser);

    int inputLength = strlen(inputString);
    yaml_parser_set_input_string(&parser, (const unsigned char *)inputString, inputLength);

    ConfigurationRef configRef = createConfiguration();

    int done = 0;
    int failed = 0;
    while (!done) {
        if (!yaml_parser_parse(&parser, &event)) {
            failed = 1;
            break;
        }
        fprintf(stderr, "%d\n", event.type);
        done = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }

    if (failed) {
        destroyConfiguration(configRef);
        configRef = NULL;
    }
    yaml_parser_delete(&parser);

    return configRef;
}
