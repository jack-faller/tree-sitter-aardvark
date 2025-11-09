(use-modules (guix gexp)
             (guix packages)
             (guix build-system tree-sitter)
             ((guix licenses) #:prefix license:))

(define-public tree-sitter-aardvark
  (package
   (name "tree-sitter-aardvark")
   (version "0.1.0")
   (source (local-file ".." #:recursive? #t))
   (build-system tree-sitter-build-system)
   (home-page "https://github.com/jack-faller/tree-sitter-aardvark")
   (synopsis "A TreeSitter grammar for the Aardvark programming language")
   (description "A TreeSitter grammar for the Aardvark programming language and associated AaScript language.")
   (license license:lgpl3+)))
tree-sitter-aardvark
