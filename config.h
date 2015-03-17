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
void configDestroy(ConfigurationRef configRef);

/* Loading Configurations */

enum ConfigurationStatus configParseString(
    ConfigurationRef configRef, 
    const char *string);

/* Accessors */
unsigned short configPort(ConfigurationRef configRef);

/* Mutators */

enum ConfigurationStatus configSetOption(
    ConfigurationRef configRef,
    const char *name,
    const char *value);

#endif
