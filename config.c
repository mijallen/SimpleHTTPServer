/*
    config.c
*/

#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <stdio.h>
#include <yaml.h>
#include "config.h"

struct Configuration {
    unsigned short port;
};

/* Creating/Destroying Configurations */

ConfigurationRef configCreate() {
    ConfigurationRef config = malloc(sizeof(struct Configuration));
    if (config != NULL) {
        config->port = 0;
    }

    return config;
}

void configDestroy(ConfigurationRef config) {
    assert(config != NULL);
    free(config);
}

/* Loading Configurations */

enum ConfigurationStatus _configLoadDocument(ConfigurationRef config, yaml_document_t *document) {
    enum ConfigurationStatus status = CONFIG_SUCCESS;

    yaml_node_t *rootNode =  yaml_document_get_root_node(document);
    if (rootNode->type == YAML_MAPPING_NODE) {

        yaml_node_pair_t *pair = rootNode->data.mapping.pairs.start;
        for (; pair != rootNode->data.mapping.pairs.top; pair++) {
            yaml_node_t *keyNode = yaml_document_get_node(document, pair->key);
            yaml_node_t *valueNode = yaml_document_get_node(document, pair->value);
            
            status = configSetOption(
                config,
                (const char *)keyNode->data.scalar.value,
                (const char *)valueNode->data.scalar.value);

            if (status != CONFIG_SUCCESS) {
                break;
            }
        }
    } else {
        status = CONFIG_INVALID_ROOT_ERROR;
    }

    return status;
}

enum ConfigurationStatus configLoadString(ConfigurationRef config, const char *inputString) {
    assert(config != NULL);
    assert(inputString != NULL);

    enum ConfigurationStatus status = CONFIG_SUCCESS;
    int inputLength = strlen(inputString);
    yaml_parser_t parser;
    yaml_document_t document;

    yaml_parser_initialize(&parser);
    yaml_parser_set_input_string(&parser, (const unsigned char *)inputString, inputLength);
    yaml_parser_load(&parser, &document);

    status = _configLoadDocument(config, &document);

    yaml_parser_delete(&parser);
    yaml_document_delete(&document);

    return status;
}

enum ConfigurationStatus configLoadFile(ConfigurationRef config, const char *filepath) {
    assert(config != NULL);
    assert(filepath != NULL);

    enum ConfigurationStatus status = CONFIG_SUCCESS;
    yaml_parser_t parser;
    yaml_document_t document;
    
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, (const unsigned char *)filepath);
    yaml_parser_load(&parser, &document);

    status = _configLoadDocument(config, &document);

    yaml_parser_delete(&parser);
    yaml_document_delete(&document);

    return status;
}

/* Accessors */

unsigned short configPort(ConfigurationRef config) {
    return config->port;
}

/* Mutators */

enum ConfigurationStatus configSetOption(ConfigurationRef config, const char *name, const char *value) {
    assert(config != NULL);
    assert(name != NULL);
    assert(value != NULL);

    // ideally should use some sort of mapping
    if (strcmp(name, "port") == 0) {
        
        errno = 0;
        unsigned short portNumber = (unsigned short)strtoul(value, NULL, 10);

        if (errno == EINVAL || errno == ERANGE) {
            return CONFIG_INVALID_VALUE_ERROR;
        }
        config->port = portNumber;

    } 
    else {
        return CONFIG_NO_SUCH_OPTION_ERROR;
    }

    return CONFIG_SUCCESS;
}