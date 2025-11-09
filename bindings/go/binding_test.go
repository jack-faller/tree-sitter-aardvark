package tree_sitter_aardvark_test

import (
	"testing"

	tree_sitter "github.com/tree-sitter/go-tree-sitter"
	tree_sitter_aardvark "github.com/jack-faller/tree-sitter-aardvark.git/bindings/go"
)

func TestCanLoadGrammar(t *testing.T) {
	language := tree_sitter.NewLanguage(tree_sitter_aardvark.Language())
	if language == nil {
		t.Errorf("Error loading Aardvark grammar")
	}
}
