/*
 * TeamSpeak 3 demo plugin
 *
 * Copyright (c) 2008-2016 TeamSpeak Systems GmbH
 */

#ifdef _WIN32
#pragma warning (disable : 4100)
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "teamspeak/public_errors.h"
#include "teamspeak/public_errors_rare.h"
#include "teamspeak/public_definitions.h"
#include "teamspeak/public_rare_definitions.h"
#include "teamspeak/clientlib_publicdefinitions.h"
#include "ts3_functions.h"
#include "plugin.h"

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#define snprintf sprintf_s
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 21

#ifdef _WIN32
static int wcharToUtf8(const wchar_t* str, char** result) {
	int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	*result = (char*)malloc(outlen);
	if(WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif

const char* ts3plugin_name() {
#ifdef _WIN32
	static char* result = NULL;
	if(!result) {
		const wchar_t* name = L"Minimal Plugin";
		if(wcharToUtf8(name, &result) == -1) {
			result = "Minimal Plugin";
		}
	}
	return result;
#else
	return "Minimal Plugin";
#endif
}

const char* ts3plugin_version() {
	return "1.0";
}

int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

const char* ts3plugin_author() {
	return "teamspeak-plugins.org";
}

const char* ts3plugin_description() {
	return "This plugin is built with the minimum of source code needed to code a TS3 client plugin.";
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
	ts3Functions = funcs;
}

int ts3plugin_init() {
	return 0;
}

void ts3plugin_shutdown() {
}

int ts3plugin_offersConfigure() {
	return PLUGIN_OFFERS_NO_CONFIGURE;
}

const char* ts3plugin_commandKeyword() {
	return NULL;
}

int ts3plugin_processCommand(uint64 serverConnectionHandlerID, const char* command) {
	return 1;
}

void ts3plugin_freeMemory(void* data) {
	free(data);
}