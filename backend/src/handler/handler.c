#include <containers/eq.h>
#include <containers/hash.h>

#include "handler.h"
#include "../core/core.h"

#define FAILURE DArrayChar_finalize(&buf);\
ret =\
    HTTPResponse_initialize(\
        response,\
        HTTP_RESPONSE_200,\
        BODY_TYPE_JSON\
    );\
if (ret) {\
    return 1;\
}\
ret = DArrayChar_initialize(&buf, 28);\
if (ret) {\
    return 1;\
}\
DArrayChar_push_back_batch(&buf, "Access-Control-Allow-Origin", 28);\
ret = DArrayChar_initialize(&buf_val, 2);\
if (ret) {\
    DArrayChar_finalize(&buf);\
    return 1;\
}\
DArrayChar_push_back_batch(&buf_val, "*", 2);\
ret =\
    HashMapStringString_fetch(\
        &response->header,\
        &buf,\
        &tgt_header\
    );\
if (ret) {\
    DArrayChar_finalize(&buf);\
    DArrayChar_finalize(&buf_val);\
    return 1;\
}\
*tgt_header = buf_val;\
response->body.json.root->is_null = false;\
response->body.json.root->type = OBJECT;\
ret =\
    HashMapStringJSONNodePtr_initialize(\
        &response->body.json.root->data.object,\
        1,\
        containers_eq_dstr,\
        containers_hash_dstr\
    );\
if (ret) {\
    return 1;\
}\
ret = DArrayChar_initialize(&buf, 8);\
if (ret) {\
    return 1;\
}\
DArrayChar_push_back_batch(&buf, "success", 8);\
HashMapStringJSONNodePtr_fetch(\
    &response->body.json.root->data.object,\
    &buf,\
    &tgt\
);\
ret = JSONNodePtr_initialize(tgt);\
if (ret) {\
    DArrayChar_finalize(&buf);\
    return 1;\
}\
(*tgt)->is_null = false;\
(*tgt)->type = BOOLEAN;\
(*tgt)->data.boolean = false;\
return 0;

int solve_handler(HTTPRequest *request, HTTPResponse *response) {
    if (request->body_type != BODY_TYPE_JSON) {
        return 1;
    }
    if (request->body.json.root->type != OBJECT) {
        return 1;
    }
    String buf;
    String buf_val;
    String *tgt_header;
    int ret = DArrayChar_initialize(&buf, 20);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "block_row", 10);\
    bool found;
    HashMapStringJSONNodePtr_find(
        &request->body.json.root->data.object,
        &buf,
        &found
    );
    if (!found) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    JSONNodePtr *tgt;
    HashMapStringJSONNodePtr_fetch(
        &request->body.json.root->data.object,
        &buf,
        &tgt
    );
    if (
        (*tgt)->is_null ||
        (*tgt)->type != NUMBER) {
        FAILURE
    }
    uint8_t block_row = (*tgt)->data.number;
    if (block_row > 11) {
        FAILURE
    }
    DArrayChar_clear(&buf);
    DArrayChar_push_back_batch(&buf, "block_column", 13);
    HashMapStringJSONNodePtr_find(
        &request->body.json.root->data.object,
        &buf,
        &found
    );
    if (!found) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    HashMapStringJSONNodePtr_fetch(
        &request->body.json.root->data.object,
        &buf,
        &tgt
    );
    if (
        (*tgt)->is_null ||
        (*tgt)->type != NUMBER) {
        FAILURE
    }
    uint8_t block_column = (*tgt)->data.number;
    if (block_row > 11) {
        FAILURE
    }
    uint8_t n = block_row * block_column;
    uint16_t size = n * n;
    String grid;
    ret = DArrayChar_initialize(&grid, size);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_clear(&buf);
    DArrayChar_push_back_batch(&buf, "data", 5);
    HashMapStringJSONNodePtr_find(
        &request->body.json.root->data.object,
        &buf,
        &found
    );
    if (!found) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&grid);
        return 1;
    }
    HashMapStringJSONNodePtr_fetch(
        &request->body.json.root->data.object,
        &buf,
        &tgt
    );
    if (
        (*tgt)->is_null ||
        (*tgt)->type != ARRAY ||
        (*tgt)->data.array.size != n) {
        DArrayChar_finalize(&grid);
        FAILURE
    }
    JSONNodePtr *iter_arr = (*tgt)->data.array.data;
    for (uint8_t i = 0; i < n; ++i, ++iter_arr) {
        if (
            (*iter_arr)->is_null ||
            (*iter_arr)->type != ARRAY ||
            (*iter_arr)->data.array.size != n) {
            DArrayChar_finalize(&grid);
            FAILURE
        }
        JSONNodePtr *iter_item = (*iter_arr)->data.array.data;
        for (uint8_t j = 0; j < n; ++j, ++iter_item) {
            if (
                !(*iter_item)->is_null &&
                (*iter_item)->type != NUMBER) {
                DArrayChar_finalize(&grid);
                FAILURE
            }
            uint8_t val =
                (*iter_item)->is_null ?
                CELL_EMPTY :
                (*iter_item)->data.number;
            DArrayChar_push_back(&grid, (char*)&val);
        }
    }
    ret = solve((uint8_t*)grid.data, block_row, block_column);
    if (ret) {
        DArrayChar_finalize(&grid);
        FAILURE
    }
    ret =
        HTTPResponse_initialize(
            response,
            HTTP_RESPONSE_200,
            BODY_TYPE_JSON
        );
    if (ret) {
        return 1;
    }
    DArrayChar_finalize(&buf);
    ret = DArrayChar_initialize(&buf, 28);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "Access-Control-Allow-Origin", 28);
    ret = DArrayChar_initialize(&buf_val, 2);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_push_back_batch(&buf_val, "*", 2);
    ret =
        HashMapStringString_fetch(
            &response->header,
            &buf,
            &tgt_header
        );
    if (ret) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&buf_val);
        return 1;
    }
    *tgt_header = buf_val;

    response->body.json.root->is_null = false;
    response->body.json.root->type = OBJECT;
    ret =
        HashMapStringJSONNodePtr_initialize(
            &response->body.json.root->data.object,
            2,
            containers_eq_dstr,
            containers_hash_dstr
        );
    if (ret) {
        return 1;
    }
    ret = DArrayChar_initialize(&buf, 8);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "success", 8);
    ret =
        HashMapStringJSONNodePtr_fetch(
            &response->body.json.root->data.object,
            &buf,
            &tgt
        );
    ret = JSONNodePtr_initialize(tgt);
    if (ret) {
        DArrayChar_finalize(&grid);
        return 1;
    }
    (*tgt)->is_null = false;
    (*tgt)->type = BOOLEAN;
    (*tgt)->data.boolean = true;
    ret = DArrayChar_initialize(&buf, 8);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "result", 7);
    ret =
        HashMapStringJSONNodePtr_fetch(
            &response->body.json.root->data.object,
            &buf,
            &tgt
        );
    ret = JSONNodePtr_initialize(tgt);
    if (ret) {
        DArrayChar_finalize(&grid);
        return 1;
    }
    (*tgt)->is_null = false;
    (*tgt)->type = ARRAY;
    ret = DArrayJSONNodePtr_initialize(&(*tgt)->data.array, n);
    if (ret) {
        DArrayChar_finalize(&grid);
        return 1;
    }
    uint8_t *iter_grid = (uint8_t*)grid.data;
    for (uint8_t i = 0; i < n; ++i) {
        JSONNodePtr temp_arr;
        ret = JSONNodePtr_initialize(&temp_arr);
        if (ret) {
            DArrayChar_finalize(&grid);
            return 1;
        }
        temp_arr->is_null = false;
        temp_arr->type = ARRAY;
        ret = DArrayJSONNodePtr_initialize(&temp_arr->data.array, n);
        if (ret) {
            DArrayChar_finalize(&grid);
            return 1;
        }
        DArrayJSONNodePtr_push_back(&(*tgt)->data.array, &temp_arr);
        for (uint8_t j = 0; j < n; ++j, ++iter_grid) {
            JSONNodePtr temp_item;
            ret = JSONNodePtr_initialize(&temp_item);
            if (ret) {
                DArrayChar_finalize(&grid);
                return 1;
            }
            temp_item->is_null = false;
            temp_item->type = NUMBER;
            temp_item->data.number = *iter_grid & ~CELL_EMPTY;
            DArrayJSONNodePtr_push_back(
                &temp_arr->data.array,
                &temp_item
            );
        }
    }
    DArrayChar_finalize(&grid);
    return 0;
}

int preflight_handler(HTTPRequest *request, HTTPResponse *response) {
    if (!request) {
        return 1;
    }
    int ret =
        HTTPResponse_initialize(
            response,
            HTTP_RESPONSE_204,
            BODY_TYPE_NONE
        );
    if (ret) {
        return 1;
    }
    String buf;
    String buf_val;
    String *tgt_header;
    ret = DArrayChar_initialize(&buf, 28);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "Access-Control-Allow-Origin", 28);
    ret = DArrayChar_initialize(&buf_val, 2);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_push_back_batch(&buf_val, "*", 2);
    ret =
        HashMapStringString_fetch(
            &response->header,
            &buf,
            &tgt_header
        );
    if (ret) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&buf_val);
        return 1;
    }
    *tgt_header = buf_val;

    ret = DArrayChar_initialize(&buf, 11);
    if (ret) {
        puts("fuck");
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "Connection", 11);
    ret = DArrayChar_initialize(&buf_val, 11);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_push_back_batch(&buf_val, "keep-alive", 11);
    ret =
        HashMapStringString_fetch(
            &response->header,
            &buf,
            &tgt_header
        );
    if (ret) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&buf_val);
        return 1;
    }
    *tgt_header = buf_val;

    ret = DArrayChar_initialize(&buf, 29);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "Access-Control-Allow-Methods", 29);
    ret = DArrayChar_initialize(&buf_val, 5);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_push_back_batch(&buf_val, "POST", 5);
    ret =
        HashMapStringString_fetch(
            &response->header,
            &buf,
            &tgt_header
        );
    if (ret) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&buf_val);
        return 1;
    }
    *tgt_header = buf_val;

    ret = DArrayChar_initialize(&buf, 23);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "Access-Control-Max-Age", 23);
    ret = DArrayChar_initialize(&buf_val, 6);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_push_back_batch(&buf_val, "86400", 6);
    ret =
        HashMapStringString_fetch(
            &response->header,
            &buf,
            &tgt_header
        );
    if (ret) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&buf_val);
        return 1;
    }
    *tgt_header = buf_val;

    ret = DArrayChar_initialize(&buf, 31);
    if (ret) {
        return 1;
    }
    DArrayChar_push_back_batch(&buf, "Access-Control-Allow-Headers", 29);
    ret = DArrayChar_initialize(&buf_val, 86);
    if (ret) {
        DArrayChar_finalize(&buf);
        return 1;
    }
    DArrayChar_push_back_batch(&buf_val, "Origin, X-Requested-With, Content-Type, Accept, Request, Access-Control-Allow-Headers", 86);
    ret =
        HashMapStringString_fetch(
            &response->header,
            &buf,
            &tgt_header
        );
    if (ret) {
        DArrayChar_finalize(&buf);
        DArrayChar_finalize(&buf_val);
        return 1;
    }
    *tgt_header = buf_val;
    DArrayChar_initialize(&buf, 10000);
    HTTPResponse_serialize(response, &buf);
    return 0;
}
