/**
 * @file Parser for the Aardvark language grammar, used also by AaScript.
 * @author Jack Faller <jack.t.faller@gmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

const language = /.*?\(language (aardvark|aascript)( \d+\.\d+)?\)/;

module.exports = grammar({
  name: "aardvark",
  extras: $ => [/\s/, $.comment],
  externals: $ => [$.long_string, $.long_string_escape],

  rules: {
    source_file: $ => seq(language, repeat($._form)),
    comment: $ => choice(/;;[^\n]*/, seq(";", $._form)),
    _form: $ => choice($.block, $.call, $.data, $.lambda, $.pair,
                       $.string, $.number, $.symbol
                       // seq("⟨", $.long_string), seq("\\<", $.long_string_escape)
    ),
    _form_not_dot: $ => choice($._form, alias("\\.", $.symbol)),
    symbol: $ => (/(\\[\[\](){};:\s"\\]|[^\[\](){};:\s"\\])+/),
    _lambda_content: $ => seq(
      alias(repeat($._form_not_dot), $.parameters),
      token(prec(1, ".")),
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
