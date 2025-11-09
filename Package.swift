// swift-tools-version:5.3

import Foundation
import PackageDescription

var sources = ["src/parser.c"]
if FileManager.default.fileExists(atPath: "src/scanner.c") {
    sources.append("src/scanner.c")
}

let package = Package(
    name: "TreeSitterAardvark",
    products: [
        .library(name: "TreeSitterAardvark", targets: ["TreeSitterAardvark"]),
    ],
    dependencies: [
        .package(url: "https://github.com/tree-sitter/swift-tree-sitter", from: "0.8.0"),
    ],
    targets: [
        .target(
            name: "TreeSitterAardvark",
            dependencies: [],
            path: ".",
            sources: sources,
            resources: [
                .copy("queries")
            ],
            publicHeadersPath: "bindings/swift",
            cSettings: [.headerSearchPath("src")]
        ),
        .testTarget(
            name: "TreeSitterAardvarkTests",
            dependencies: [
                "SwiftTreeSitter",
                "TreeSitterAardvark",
            ],
            path: "bindings/swift/TreeSitterAardvarkTests"
        )
    ],
    cLanguageStandard: .c11
)
