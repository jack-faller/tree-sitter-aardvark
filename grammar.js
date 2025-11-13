/**
 * @file Parser for the Aardvark language grammar, used also by AaScript.
 * @author Jack Faller <jack.t.faller@gmail.com>
 * @license GPL-3.0
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check
function join(...regexps) {
  let out = "";
  for (let i of regexps)
    out += i.source;
  return new RegExp(out);
}
const symbol = /(\\[⟨⟩\[\](){};:\s"\\]|[^⟨⟩\[\](){};:\s"\\])+/;
const language = join(/\(language /, symbol, /( \d+\.\d+)?\)/);
module.exports = grammar({
  name: "aardvark",
  extras: $ => [/\s/, $.comment],
  externals: $ => [$._long_string, $._long_string_escaped],

  rules: {
    source_file: $ =>
      seq(optional(alias(/.+/, $.preamble)), $.language, repeat($._form)),
    language: $ => language,
    symbol: $ => symbol,
    comment: $ => choice(/;;[^\n]*/, seq(";", $._form)),
    _form: $ => choice($.block, $.call, $.data, $.lambda, $.pair,
                       $.string, $.long_string, $.number, $.symbol),
    long_string: $ => choice(seq(/⟨/, $._long_string),
                             seq(/\\</, $._long_string_escaped)),
    _form_not_dot: $ => choice($._form, alias("\\.", $.symbol)),
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
