/**
 * @file Parser for the Aardvark language grammar, used also by AaScript.
 * @author Jack Faller <jack.t.faller@gmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check
const language = /\(language (aardvark|aascript)( \d+\.\d+)?\)/;
module.exports = grammar({
  name: "aardvark",
  extras: $ => [/\s/, $.comment],
  externals: $ => [$._long_string, $._long_string_escaped, $.preamble],

  rules: {
    source_file: $ => seq($.preamble, alias(language, $.language), repeat($._form)),
    comment: $ => choice(/;;[^\n]*/, seq(";", $._form)),
    _form: $ => choice($.block, $.call, $.data, $.lambda, $.pair,
                       $.string, $.long_string, $.number, $.symbol),
    long_string: $ => choice(seq(/⟨/, $._long_string),
                             seq(/\\</, $._long_string_escaped)),
    _form_not_dot: $ => choice($._form, alias("\\.", $.symbol)),
    symbol: $ => (/(\\[⟨⟩\[\](){};:\s"\\]|[^⟨⟩\[\](){};:\s"\\])+/),
    _lambda_content: $ => seq(
      alias(repeat($._form_not_dot), $.parameters),
      ".",
      choice($._lambda_content, alias(repeat($._form_not_dot), $.body))),
    lambda: $ => seq("{", $._lambda_content, "}"),
    block: $ => seq("{", repeat($._form_not_dot), "}"),
    data: $ => seq("[", repeat($._form), "]"),
    call: $ => seq("(", repeat($._form), ")"),
    pair: $ => seq(":", $._form, $._form),
    string: $ => seq(/"/, repeat(choice($.string_escape, /[^\\"]/)), /"/),
    string_escape: $ => (/\\[0abntr<>\\]/),
    number: $ => token(prec(1, /[+-]?\d+(\.\d+(e[+-]?\d+)?)?/)),
  }
});
