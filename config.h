/*
    config.h
*/

#ifndef SIMPLE_HTTP_SERVER_CONFIG_H
#define SIMPLE_HTTP_SERVER_CONFIG_H

typedef struct Configuration* ConfigurationRef;

ConfigurationRef parseConfigurationString(const char *string);

#endif
