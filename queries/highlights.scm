["(" ")" "[" "]" "{" "}"] @punctuation.bracket
(language) @attribute
(lambda "." @punctuation.delimiter)
(number) @number
[(comment) (preamble)] @comment
(symbol) @variable
((symbol) @constant.builtin (#match? @constant.builtin "^(true|false)$"))
[(string) (long_string)] @string
(string_escape) @string.special
(call . (symbol) @function)
((symbol) @operator (#match? @operator "^(\\+|-|\\*|/|=|!=|≠|>|<|>=|<=|^|'|\\||&)$"))
(call . (symbol) @keyword
      (#match? @keyword "^(module|package|define|import|export|use|let|type|match|if|and|or)$"))
(call . (symbol) @keyword . (symbol) @module
      (#match? @keyword "^(module|package)$"))
(call . (symbol) @_at . _ . (symbol) @property (#match? @_at "^at$"))
(pair . (comment) * . (symbol) @property)
(pair ":" @property)
((symbol) @keyword (#match? @keyword "^_$"))
((symbol) @type (#match? @type "^[A-Z]"))
