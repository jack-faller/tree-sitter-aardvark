import XCTest
import SwiftTreeSitter
import TreeSitterAardvark

final class TreeSitterAardvarkTests: XCTestCase {
    func testCanLoadGrammar() throws {
        let parser = Parser()
        let language = Language(language: tree_sitter_aardvark())
        XCTAssertNoThrow(try parser.setLanguage(language),
                         "Error loading Aardvark grammar")
    }
}
