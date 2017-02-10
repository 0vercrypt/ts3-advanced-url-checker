/*
 * TeamSpeak 3 demo plugin
 *
 * Copyright (c) 2008-2016 TeamSpeak Systems GmbH
 */

#include <iostream>

#ifdef _WIN32
#pragma warning (disable : 4100)
#include <Windows.h>
#endif

//#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <regex>
#include <iterator>
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
	if (WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif

const char* ts3plugin_name() {
#ifdef _WIN32
	static char* result = NULL;
	if (!result) {
		const wchar_t* name = L"Advanced URL checker";
		if (wcharToUtf8(name, &result) == -1) {
			result = "Advanced URL checker";
		}
	}
	return result;
#else
	return "Advanced URL checker";
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
	return "This plugin checks incoming URLs (links) for redirections and shows you the real landing URL.";
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

int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored)
{
	printf("getting message: %s\n", message);
	if (strlen(message) > 0)
	{
		std::string msg(message);

		std::vector<std::string> lstMatchesUrl;
		std::vector<std::string> lstMatchesText;

		std::regex reg1("\\[url\\]([^\\[\\s]*)\\[\\/url\\]");
		std::sregex_token_iterator it(msg.begin(), msg.end(), reg1, 1);
		std::sregex_token_iterator it_end;

		while (it != it_end) {
			lstMatchesUrl.push_back(it->str());
			lstMatchesText.push_back("");
			++it;
		}

		// get url
		std::regex reg2("\\[url\\=([^\\]\\s]*)\\]([^\\[]*)\\[\\/url\\]");
		std::sregex_token_iterator it2(msg.begin(), msg.end(), reg2, 1);
		std::sregex_token_iterator it3(msg.begin(), msg.end(), reg2, 2);

		while (it2 != it_end) {
			lstMatchesUrl.push_back(it2->str());
			++it2;
		}

		while (it3 != it_end) {
			lstMatchesText.push_back(it3->str());
			++it3;
		}

		for (int i = lstMatchesUrl.size() - 1; i >= 0; i--)
		{
			std::string strUrl(lstMatchesUrl[i]);
			std::string strText(lstMatchesText[i]);
			printf("url %d: %s\n", i, strUrl.c_str());
			printf("text %d: %s\n", i, strText.c_str());

			if (lstMatchesText[i].length() > 0)
			{
				if (lstMatchesText[i] != lstMatchesUrl[i])
				{
					std::string msg = "Attention! URL " + lstMatchesText[i] + " points to " + lstMatchesUrl[i] + "!";
					ts3Functions.printMessageToCurrentTab(msg.c_str());
				}
			}
		}
	}
	return 0;
}