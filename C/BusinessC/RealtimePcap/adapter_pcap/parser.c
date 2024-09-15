#include "parser.h"

void parse_configure(char* interface, char* pcap_dir)
{
	JSON_Value *rootValue = json_parse_file("configure.json");
	JSON_Object *rootObject = json_value_get_object(rootValue);

	strcpy(interface, json_object_get_string(rootObject, "interface"));
	strcpy(pcap_dir, json_object_get_string(rootObject, "pcap_dir"));

	json_value_free(rootValue);
}

	
