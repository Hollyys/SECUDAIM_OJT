#include "parser.h"

void parse_configure(Config* config)
{
	JSON_Value *rootValue = json_parse_file("configure.json");
	JSON_Object *rootObject = json_value_get_object(rootValue);

	strcpy(config->interface, json_object_get_string(rootObject, "interface"));
	strcpy(config->pcap_dir, json_object_get_string(rootObject, "pcap_dir"));
	strcpy(config->adapter, json_object_get_string(rootObject, "adapter"));

	json_value_free(rootValue);
}

	
