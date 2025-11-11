["(" ")" "[" "]" "{" "}"] @punctuation.bracket
(language) @directive
(lambda "." @punctuation.delimiter)
(number) @number
[(comment) (preamble)] @comment
(symbol) @variable
((symbol) @constant.builtin (#match? @constant.builtin "^(true|false)$"))
[(string) (long_string)] @string
(string_escape) @string.special
(call . (comment)* . (symbol) @function)
((symbol) @operator (#match? @operator "^(\\+|-|\\*|/|=|!=|≠|>|<|>=|<=|^|'|\\||&)$"))
(call . (comment)* . (symbol) @keyword
      (#match? @keyword "^(module|package|define|import|export|use|document|let|type|macro|macro-variable|include|where|match|if|and|or|_)$"))
(call . (comment)* . (symbol) @keyword . (comment)* . (symbol) @module
      (#match? @keyword "^(module|package)$"))
(call . (comment)* . (symbol) @_at . (comment)* . _ . (comment)* . (symbol) @property (#match? @_at "^@$"))
(pair ":" @directive . (comment)* . (symbol) @directive)
(parameters [(symbol) @variable.definition
             (pair ":" @variable.definition . (comment)* . (symbol) @variable.definition)])
(call . (comment)* . (symbol) @keyword . (comment)* . (symbol) @variable.definition
      (#equal? @keyword "let"))
(call . (comment)* . (symbol) @keyword (symbol) @variable.definition
      (#equal? @keyword "macro-variable"))
(call . (comment)* . (symbol) @keyword
      . (comment)* . _
      (pair ":" @variable.definition . (comment) * . (symbol) @variable.definition)
      (#eq? @keyword "where"))
(call . (comment)* . (symbol) @keyword
      . (comment)* . _
      (pair ":" @variable.definition . (comment) * . (symbol) @variable.definition)
      (#eq? @keyword "where"))
(call . (comment)* . (symbol) @keyword
      . (comment)* . (symbol) @function.definition
      . (comment)* . [(lambda) (call . (comment)* . (symbol) @_macro (#eql? @_macro "macro"))]
      (#equal? @keyword "let"))
(call . (comment)* . (symbol) @keyword . (comment)* . (symbol) @module
      (#match? @keyword "^(module|package)$"))
((symbol) @keyword (#match? @keyword "^_$"))
((symbol) @type (#match? @type "^[A-Z]"))
