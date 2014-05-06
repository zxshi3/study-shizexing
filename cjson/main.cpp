/**
 * this example show you how to use cJSON to parse json file
 *
 * In the test.json, it shows a simple json object
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "cJSON.h"
#include <string>

static void testParseJson();
static void testGenerateJson();

int main(int argc, char** argv)
{
    testParseJson();
    testGenerateJson();
	return 0;
}

static const char filename[] = "test.json";

static std::string loadJson(const std::string & filename) {
	FILE* fp = fopen(filename.c_str(), "r");
	if (fp)
	{
		long size;
		char * buf;
		// read file data
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		buf = (char*)malloc((size_t)size + 1);
		assert(buf);
		memset(buf, 0, size + 1);
		fread(buf, sizeof(char), (size_t)size, fp);
		buf[size] = '\0';
		fclose(fp);
        std::string data = std::string(buf, size);
    	free(buf);
    	buf = 0;
        return data;
	}
	else
	{
		fprintf(stderr, "can not open %s\n", filename.c_str());
        return std::string();
	}	
}

static void testParseJson() {
	cJSON * tmp;
	cJSON * json;
	
    std::string data = loadJson(filename);
	// parse json file
	json = cJSON_Parse(data.c_str());
	//cJSON * tmp;
	tmp = cJSON_GetObjectItem(json, "string");
	if (tmp)
	{
		char* str = tmp->valuestring;
		printf("string : %s\n", str);
	}
	tmp = cJSON_GetObjectItem(json, "float");
	if (tmp)
	{
		double f = tmp->valuedouble;
		printf("float value : %f\n", f);
	}
	tmp = cJSON_GetObjectItem(json, "int");
	if (tmp)
	{
		int i = tmp->valueint;
		printf("int value : %d\n", i);
	}
	tmp = cJSON_GetObjectItem(json, "boolean");
	if (tmp)
	{
		int b = tmp->valueint;
		printf("boolean value : %s\n", (1 == b) ? "true" : "false");
	}

	cJSON_Delete(json);
}

static void testGenerateJson() {
    cJSON* obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "name", "connection_status");
    cJSON* data = cJSON_CreateObject();
    cJSON_AddStringToObject(data, "vendor", "tnlink");
    cJSON_AddTrueToObject(data, "connected");
    cJSON_AddNumberToObject(data, "version", 3);
    cJSON_AddItemToObject(obj, "data", data);
    
    char * json = cJSON_Print(obj);
    printf("json string : \n%s\n", json);
    free(json);
    cJSON_Delete(obj);
}