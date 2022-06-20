#include "twain.h"
#include <node_api.h>

/********************************/
/*  包装twain.h中的DSM_Entry方法  */
/********************************/
napi_value Wrap_DSM_Entry(napi_env env, napi_callback_info info) {
    napi_status status;
    napi_value word;
    char *str = "Hello N-API!";

    status = napi_create_string_utf8(env, str, strlen(str), &word);
    assert(status == napi_ok);

    return word;
}

napi_value init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc = {
    "DSM_Entry",
    NULL,
    Wrap_DSM_Entry,
    NULL,
    NULL,
    NULL,
    napi_writable | napi_enumerable | napi_configurable,
    NULL
  };
  status = napi_define_properties(env, exports, 1, &desc);
  if (status != napi_ok) return NULL;
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
