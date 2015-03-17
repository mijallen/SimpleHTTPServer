/*
    config.h
*/

#ifndef SIMPLE_HTTP_SERVER_CONFIG_H
#define SIMPLE_HTTP_SERVER_CONFIG_H

typedef struct Configuration* ConfigurationRef;

enum ConfigurationStatus {
    CONFIG_SUCCESS,
    CONFIG_NO_SUCH_OPTION_ERROR,
    CONFIG_INVALID_VALUE_ERROR,
    CONFIG_INVALID_ROOT_ERROR
};

/* Creating/Destroying Configurations */
ConfigurationRef configCreate(void);
void configDestroy(ConfigurationRef config);

/* Loading Configurations */

enum ConfigurationStatus configLoadFile(
    ConfigurationRef config,
    const char *filepath);

enum ConfigurationStatus configLoadString(
    ConfigurationRef config, 
    const char *string);

/* Accessors */
unsigned short configPort(ConfigurationRef config);

/* Mutators */

enum ConfigurationStatus configSetOption(
    ConfigurationRef config,
    const char *name,
    const char *value);

#endif
