#include <stdlib.h>
#include <stdio.h>
#include <symbol.h>
#include <glib.h>
#include <stdint.h>

int main(int argc, char** argv) {

    const char* filename = argv[1];
    char* errmsg;
    symbol_t* sym = sym_create();

    if (!sym_from_json_file(sym, filename, &errmsg)) {
        fprintf(stderr, "%s\n", errmsg);
        free(errmsg);
    }
    char* jsonstr = sym_to_json_string(sym);
    sym_destroy(sym);

    symbol_t* sym2 = sym_create();
    if (!sym_from_json_string(sym2, jsonstr, &errmsg)) {
        fprintf(stderr, "%s\n", errmsg);
        free(errmsg);
    }
    free(jsonstr);

    jsonstr = sym_to_json_string(sym2);
    fprintf(stderr, "%s\n", jsonstr);
    free(jsonstr);

    size_t len;
    char* buf = sym_serialize(sym2, &len);
    fprintf(stderr, "memory size: %d\n", len);
    sym_destroy(sym2);

    fprintf(stderr, "sizeof(uint8_t): %d\n", sizeof(uint8_t));
    sym2 = sym_deserialize(buf);
    fprintf(stderr, "%s\n", sym_to_json_string(sym2));

    return EXIT_SUCCESS;
}