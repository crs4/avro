/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to you under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <errno.h>
#include <avro/platform.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "avro/allocation.h"
#include "avro/errors.h"
#include "avro/legacy.h"
#include "avro/schema.h"
#include "avro/value.h"
#include "avro_private.h"
#include "jansson.h"

#define check_return(retval, call) \
	do { \
		int  __rc; \
		__rc = call; \
		if (__rc != 0) { \
			return retval; \
		} \
	} while (0)

int 
avro_value_from_json_t(const json_t *json, avro_value_t *value)
{
	check_param(EINVAL, json, "json");
	check_param(EINVAL, value, "value");  
  fprintf(stderr, "we are in avro_value_from_json_t\n");
  fprintf(stderr, "we are in avro_value_from_json_t -- %d\n", avro_value_get_type(value));
  switch (avro_value_get_type(value)) {
  case AVRO_BOOLEAN:
		{
  fprintf(stderr, "we have a boolean  in avro_value_from_json_t\n");        
      assert (json_is_boolean(json));
			int  val = json_is_true(json);
			check_return(NULL, avro_value_set_boolean(value, val));
      return 0;
		}
#if 0
		case AVRO_BYTES:
		{
			const void  *val;
			size_t  size;
			void  *encoded = NULL;
			size_t  encoded_size = 0;

			check_return(NULL, avro_value_get_bytes(value, &val, &size));

			if (encode_utf8_bytes(val, size, &encoded, &encoded_size)) {
				return NULL;
			}

			json_t  *result = json_string_nocheck((const char *) encoded);
			avro_free(encoded, encoded_size);
			if (result == NULL) {
				avro_set_error("Cannot allocate JSON bytes");
			}
			return result;
		}
#endif
		case AVRO_DOUBLE:
		{
      assert (json_is_real(json));
			double  val = json_real_value(json);
			check_return(NULL, avro_value_set_double(value, val));
      return 0;
		}

		case AVRO_FLOAT:
		{
      assert (json_is_real(json));
			float  val = json_real_value(json);
			check_return(NULL, avro_value_set_float(value, val));
      return 0;
		}

		case AVRO_INT32:
		{
      assert (json_is_integer(json));
			int32_t  val = json_integer_value(json);
			check_return(NULL, avro_value_set_int(value, val));
      return 0;
		}

		case AVRO_INT64:
		{
      assert (json_is_integer(json));
			int64_t  val = json_integer_value(json);
			check_return(NULL, avro_value_set_long(value, val));
      return 0;
		}

		case AVRO_NULL:
		{
			check_return(NULL, avro_value_set_null(value));
      return 0;
		}

		case AVRO_STRING:
		{
			const char  *val = json_string_value(json);
			size_t  size;
			check_return(NULL, avro_value_set_string(value, val));
      return 0;
		}
#if 0
		case AVRO_ARRAY:
		{
			int  rc;
			size_t  element_count, i;
			json_t  *result = json_array();
			if (result == NULL) {
				avro_set_error("Cannot allocate JSON array");
				return NULL;
			}

			rc = avro_value_get_size(value, &element_count);
			if (rc != 0) {
				json_decref(result);
				return NULL;
			}

			for (i = 0; i < element_count; i++) {
				avro_value_t  element;
				rc = avro_value_get_by_index(value, i, &element, NULL);
				if (rc != 0) {
					json_decref(result);
					return NULL;
				}

				json_t  *element_json = avro_value_to_json_t(&element);
				if (element_json == NULL) {
					json_decref(result);
					return NULL;
				}

				if (json_array_append_new(result, element_json)) {
					avro_set_error("Cannot append element to array");
					json_decref(result);
					return NULL;
				}
			}

			return result;
		}
#endif

#if 0
		case AVRO_ENUM:
		{
			avro_schema_t  enum_schema;
			int  symbol_value;
			const char  *symbol_name;

			check_return(NULL, avro_value_get_enum(value, &symbol_value));
			enum_schema = avro_value_get_schema(value);
			symbol_name = avro_schema_enum_get(enum_schema, symbol_value);
			return_json("enum", json_string(symbol_name));
		}
#endif
    
#if 0
		case AVRO_FIXED:
		{
			const void  *val;
			size_t  size;
			void  *encoded = NULL;
			size_t  encoded_size = 0;

			check_return(NULL, avro_value_get_fixed(value, &val, &size));

			if (encode_utf8_bytes(val, size, &encoded, &encoded_size)) {
				return NULL;
			}

			json_t  *result = json_string_nocheck((const char *) encoded);
			avro_free(encoded, encoded_size);
			if (result == NULL) {
				avro_set_error("Cannot allocate JSON fixed");
			}
			return result;
		}
#endif

#if 0
		case AVRO_MAP:
		{
			int  rc;
			size_t  element_count, i;
			json_t  *result = json_object();
			if (result == NULL) {
				avro_set_error("Cannot allocate JSON map");
				return NULL;
			}

			rc = avro_value_get_size(value, &element_count);
			if (rc != 0) {
				json_decref(result);
				return NULL;
			}

			for (i = 0; i < element_count; i++) {
				const char  *key;
				avro_value_t  element;

				rc = avro_value_get_by_index(value, i, &element, &key);
				if (rc != 0) {
					json_decref(result);
					return NULL;
				}

				json_t  *element_json = avro_value_to_json_t(&element);
				if (element_json == NULL) {
					json_decref(result);
					return NULL;
				}

				if (json_object_set_new(result, key, element_json)) {
					avro_set_error("Cannot append element to map");
					json_decref(result);
					return NULL;
				}
			}

			return result;
		}
#endif

#if 0
		case AVRO_RECORD:
		{
			int  rc;
			size_t  field_count, i;
			json_t  *result = json_object();
			if (result == NULL) {
				avro_set_error("Cannot allocate new JSON record");
				return NULL;
			}

			rc = avro_value_get_size(value, &field_count);
			if (rc != 0) {
				json_decref(result);
				return NULL;
			}

			for (i = 0; i < field_count; i++) {
				const char  *field_name;
				avro_value_t  field;

				rc = avro_value_get_by_index(value, i, &field, &field_name);
				if (rc != 0) {
					json_decref(result);
					return NULL;
				}

				json_t  *field_json = avro_value_to_json_t(&field);
				if (field_json == NULL) {
					json_decref(result);
					return NULL;
				}

				if (json_object_set_new(result, field_name, field_json)) {
					avro_set_error("Cannot append field to record");
					json_decref(result);
					return NULL;
				}
			}

			return result;
		}
#endif

#if 0
		case AVRO_UNION:
		{
			int  disc;
			avro_value_t  branch;
			avro_schema_t  union_schema;
			avro_schema_t  branch_schema;
			const char  *branch_name;

			check_return(NULL, avro_value_get_current_branch(value, &branch));

			if (avro_value_get_type(&branch) == AVRO_NULL) {
				return_json("null", json_null());
			}

			check_return(NULL, avro_value_get_discriminant(value, &disc));
			union_schema = avro_value_get_schema(value);
			branch_schema =
			    avro_schema_union_branch(union_schema, disc);
			branch_name = avro_schema_type_name(branch_schema);

			json_t  *result = json_object();
			if (result == NULL) {
				avro_set_error("Cannot allocate JSON union");
				return NULL;
			}

			json_t  *branch_json = avro_value_to_json_t(&branch);
			if (branch_json == NULL) {
				json_decref(result);
				return NULL;
			}

			if (json_object_set_new(result, branch_name, branch_json)) {
				avro_set_error("Cannot append branch to union");
				json_decref(result);
				return NULL;
			}

			return result;
		}
#endif
		default:
			return NULL;
	}
}

