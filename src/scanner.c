#include "tree_sitter/parser.h"
#include "tree_sitter/alloc.h"
#include "tree_sitter/array.h"

enum TokenType {LONG_STRING, LONG_STRING_ESCAPE};

void * tree_sitter_aardvark_external_scanner_create() {
    return NULL;
}
void tree_sitter_aardvark_external_scanner_destroy(void *payload) {
}
unsigned tree_sitter_aardvark_external_scanner_serialize(
  void *payload,
  char *buffer
) {
    return 0;
}
void tree_sitter_aardvark_external_scanner_deserialize(
  void *payload,
  const char *buffer,
  unsigned length
) {
}
typedef Array(uint32_t) ArrayN32;
static bool equal(ArrayN32 *to, uint32_t *buffer, unsigned int head) {
    for (int i = 0; i < to->size; ++i)
        if (to->contents[i] != buffer[(head + i) % to->size])
            return false;
    return true;
}
// these chars ⟨ ⟩
#define langle 0x27E8
#define rangle 0x27E9
bool tree_sitter_aardvark_external_scanner_scan(
  void *payload,
  TSLexer *lexer,
  const bool *valid_symbols
) {
#define RETURN(i) {out = i; goto end;}
    bool out;
    ArrayN32 label;
    uint32_t *buffer = NULL;
    array_init(&label);
    if (valid_symbols[LONG_STRING_ESCAPE]) {
        lexer->result_symbol = LONG_STRING_ESCAPE;
    }
    if (valid_symbols[LONG_STRING]) {
        lexer->result_symbol = LONG_STRING;
        if (lexer->eof(lexer)) RETURN(false);
        switch (lexer->lookahead) {
            default:
            RETURN(false);
        case langle:
            array_push(&label, langle);
            do {
                lexer->advance(lexer, false);
                if (lexer->eof(lexer)) RETURN(false);
                array_push(&label, lexer->lookahead);
            } while (lexer->lookahead != rangle);
            array_push(&label, lexer->lookahead);
            break;
        case '\\':
            lexer->advance(lexer, false);
            if (lexer->eof(lexer) || lexer->lookahead != '<') RETURN(false);
            array_push(&label, '\\');
            array_push(&label, '<');
            int progress = 0;
            static const uint32_t vals[2] = {'\\', '>'};
            do {
                lexer->advance(lexer, false);
                if (lexer->eof(lexer)) RETURN(false);
                array_push(&label, lexer->lookahead);
                progress = lexer->lookahead == vals[progress]
                           ? progress + 1 : 0;
            } while (progress < 2);
            break;
        }
        // `label` now contains the full label text.
        buffer = ts_calloc(label.size, sizeof(uint32_t));
        unsigned int head = 0;
        for (int i = 0; i < label.size; ++i) {
            lexer->advance(lexer, false);
            if (lexer->eof(lexer)) RETURN(false);
            buffer[i] = lexer->lookahead;
        }
        while (!equal(&label, buffer, head)) {
            lexer->advance(lexer, false);
            if (lexer->eof(lexer)) RETURN(false);
            buffer[head] = lexer->lookahead;
            head = (head + 1) % label.size;
        }
        lexer->advance(lexer, false);
        RETURN(true);
    }
#undef RETURN
 end:
    if (buffer) ts_free(buffer);
    array_delete(&label);
    return out;
}
