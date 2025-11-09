/**
 * @file Parser for the Aardvark language grammar, used also by AaScript.
 * @author Jack Faller <jack.t.faller@gmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

module.exports = grammar({
  name: "aardvark",

  rules: {
    // TODO: add the actual grammar rules
    source_file: $ => "hello"
  }
});
