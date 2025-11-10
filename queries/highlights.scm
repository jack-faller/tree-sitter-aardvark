["(" ")" "[" "]" "{" "}"] @punctuation.bracket
(language) @attribute
(lambda "." @punctuation.delimiter)
(number) @number
[(comment) (preamble)] @comment
((symbol) @keyword
 (#match? @keyword "^_$"))
((symbol) @constant.builtin
 (#match? @constant.builtin "^(true|false)$"))
((symbol) @type
 (#match? @type "^[A-Z]"))
(symbol) @variable
[(string) (long_string)] @string
(string_escape) @string.special
(call . (symbol) @function)
((symbol) @operator
 (#match? @operator "^(\\+|-|\\*|/|=|!=|≠|>|<|>=|<=|^|'|\\||&)$"))
(call . (symbol) @keyword
      (#match? @keyword "^(module|package|define|let|language|match|if)$"))
(call . (symbol) @keyword . (symbol) @module
      (#match? @keyword "^(module|package)$"))
(call . (symbol) @_at . _ . (symbol) @property (#match? @_at "^at$"))
(pair . (symbol) @property)
