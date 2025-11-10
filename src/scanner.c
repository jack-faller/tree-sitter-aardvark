#include "tree_sitter/parser.h"
#include "tree_sitter/alloc.h"
#include "tree_sitter/array.h"

#define LENGTH(A) (sizeof(A)/(sizeof(A[0])))

enum TokenType {LONG_STRING, LONG_STRING_ESCAPED, PREAMBLE};

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
enum State {LANGUAGE, FIRST_NUMBER, SECOND_NUMBER};
// these chars ⟨ ⟩
#define langle 0x27E8
#define rangle 0x27E9
#define PROGRESS(p, vals, next) p = (next) == vals[p] ? p + 1 : 0
bool tree_sitter_aardvark_external_scanner_scan(
  void *payload,
  TSLexer *lexer,
  const bool *valid_symbols
) {
    if (valid_symbols[PREAMBLE]) {
        lexer->result_symbol = PREAMBLE;
        enum State state = LANGUAGE;
        unsigned int progress = 0;
        unsigned int progress_script = 0;
        static const uint32_t aardvark_string[] = {'(', 'l', 'a', 'n', 'g', 'u', 'a', 'g', 'e', ' ', 'a', 'a', 'r', 'd', 'v', 'a', 'r', 'k'};
        static const uint32_t aascript_string[] = {'(', 'l', 'a', 'n', 'g', 'u', 'a', 'g', 'e', ' ', 'a', 'a', 's', 'c', 'r', 'i', 'p', 't'};
        bool is_first_digit = true;
        for (;;) {
            if (lexer->lookahead == '(') lexer->mark_end(lexer);
            switch (state) {
            case LANGUAGE:
                if (lexer->eof(lexer)) return false;
                PROGRESS(progress, aardvark_string, lexer->lookahead);
                PROGRESS(progress_script, aascript_string, lexer->lookahead);
                lexer->advance(lexer, false);
                if (progress == LENGTH(aardvark_string)
                    || progress_script == LENGTH(aascript_string)) {
                    progress_script = progress = 0;
                    if (lexer->lookahead == ' ') {
                        lexer->advance(lexer, false);
                        is_first_digit = true;
                        state = FIRST_NUMBER;
                    } else if (lexer->lookahead == ')') {
                        return true;
                    }
                }
                break;
            case FIRST_NUMBER:
                if (lexer->lookahead == '.') {
                    lexer->advance(lexer, false);
                    state = is_first_digit ? LANGUAGE : SECOND_NUMBER;
                    is_first_digit = true;
                } else if ('0' <= lexer->lookahead && lexer->lookahead <= '9') {
                    lexer->advance(lexer, false);
                    is_first_digit = false;
                } else {
                    state = LANGUAGE;
                }
                break;
            case SECOND_NUMBER:
                if (lexer->lookahead == ')') {
                    return !is_first_digit;
                } else if ('0' <= lexer->lookahead && lexer->lookahead <= '9') {
                    lexer->advance(lexer, false);
                    is_first_digit = false;
                } else {
                    state = LANGUAGE;
                }
                break;
            }
        }
    }
#define RETURN(i) {out = i; goto end;}
    bool out;
    ArrayN32 label;
    uint32_t *buffer = NULL;
    array_init(&label);
    if (valid_symbols[LONG_STRING]) {
        lexer->result_symbol = LONG_STRING;
        array_push(&label, langle);
        while (lexer->lookahead != rangle) {
            array_push(&label, lexer->lookahead);
            lexer->advance(lexer, false);
            if (lexer->eof(lexer)) RETURN(false);
        }
        array_push(&label, lexer->lookahead);
        goto read_until_label;
    }
    if (valid_symbols[LONG_STRING_ESCAPED]) {
        lexer->result_symbol = LONG_STRING_ESCAPED;
        array_push(&label, '\\');
        array_push(&label, '<');
        int progress = 0;
        static const uint32_t vals[] = {'\\', '>'};
        while (progress < LENGTH(vals)) {
            printf("%c\n", (char)lexer->lookahead);
            array_push(&label, lexer->lookahead);
            progress = lexer->lookahead == vals[progress]
                       ? progress + 1 : 0;
            lexer->advance(lexer, false);
            if (lexer->eof(lexer)) RETURN(false);
        }
        goto read_until_label;
    }
read_until_label:
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
#undef RETURN
 end:
    if (buffer) ts_free(buffer);
    array_delete(&label);
    return out;
}
