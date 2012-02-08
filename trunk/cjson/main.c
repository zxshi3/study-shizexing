/**
 * this example show you how to use cJSON to parse json file
 *
 * In the test.json, it shows a simple json object
 */
#include <cJSON.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

static const char[] filename = "test.json";

int main(int argc, char** argv)
{
	FILE* fp = fopen(filename, "r");
	if (fp)
	{
		long size;
		char * buf;
		cJSON * json;
		cJSON * tmp;
		
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

		// parse json file
		json = cJSON_Parse(buf);
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

		free(buf);
		buf = 0;
	}
	else
	{
		fprintf(stderr, "can not open %s\n", filename);
	}
	
	return 0;
}
