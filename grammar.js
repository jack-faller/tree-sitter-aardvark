/**
 * @file Parser for the Aardvark language grammar, used also by AaScript.
 * @author Jack Faller <jack.t.faller@gmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

const language = /.*?\(language (aardvark|aascript)( \d+\.\d+)?\)/;

const symbol_escape = new RustRegex('\\\\[⟨⟩\\[\\](){};:\s"\\\\]');
const symbol_char_not_dot = choice(
  symbol_escape, new RustRegex('[^⟨⟩\\[\\](){};:\\s"\\\\.]')
);
const symbol_char = choice(symbol_char_not_dot, ".");
const symbol_start = choice(seq(".", symbol_char), symbol_char_not_dot);

module.exports = grammar({
  name: "aardvark",
  extras: $ => [/\s/, $.comment],
  externals: $ => [$.long_string, $.long_string_escape],

  rules: {
    source_file: $ => seq(language, repeat($._form)),
    comment: $ => choice(/;;[^\n]*/, seq(";", $._form)),
    _form_shared: $ => choice(
      $.block, $.call, $.data, $.lambda, $.pair,
      $.string, $.number
      // ,
      // seq("⟨", $.long_string),
      // seq("\\<", $.long_string_escape)
    ),
    _form: $ => choice($._form_shared, $.symbol),
    _form_not_dot: $ => choice($._form_shared, $._symbol_not_dot),
    _symbol_shared: $ => token(seq(symbol_start, repeat(symbol_char))),
    symbol: $ => choice(".", $._symbol_shared),
    _symbol_not_dot: $ => alias(choice("\\.", $._symbol_shared), $.symbol),
    _lambda_content: $ => seq(
      alias(repeat($._form_not_dot), $.parameters),
      ".",
      choice($._lambda_content, alias(repeat($._form_not_dot), $.body))),
    lambda: $ => seq("{", $._lambda_content, "}"),
    block: $ => seq("{", repeat($._form_not_dot), "}"),
    data: $ => seq("[", repeat($._form), "]"),
    call: $ => seq("(", repeat($._form), ")"),
    pair: $ => seq(":", $._form, $._form),
    string: $ => seq('"', repeat(choice($.string_escape, /[^\\"]/)), '"'),
    string_escape: $ => (/\\[0abntr<>\\]/),
    number: $ => token(prec(1, /[+-]?\d+(\.\d+(e[+-]?\d+)?)?/)),
  }
});
