from pathlib import PurePosixPath

from templategen.config import DocumentMetadata, GeneratorConfig
from templategen.model import DirectoryNode, FileNode
from templategen.render import language_for_extension, render_document


def test_language_for_extension_maps_cpp_and_python():
    assert language_for_extension(".cpp") == "C++"
    assert language_for_extension(".py") == "Python"
    assert language_for_extension(".txt") == "text"


def test_render_document_writes_headings_titles_and_paths():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[
            DirectoryNode(
                relative_path=PurePosixPath("graph"),
                display_name="图论",
                files=[
                    FileNode(
                        relative_path=PurePosixPath("graph/dinic.cpp"),
                        display_name="Dinic",
                        extension=".cpp",
                    )
                ],
            )
        ],
        files=[],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(title="ICPC & Templates", author="Li_xiao"),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={},
        order_map={},
    )

    document = render_document(tree, config)

    assert r"\title{ICPC \& Templates}" in document
    assert r"\author{Li\_xiao}" in document
    assert r"\section{图论}" in document
    assert r"\templatefiletitle{Dinic}" in document
    assert r"\lstinputlisting[language=C++]{graph/dinic.cpp}" in document


def test_render_document_folds_paths_deeper_than_five_levels():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[
            DirectoryNode(
                relative_path=PurePosixPath("a"),
                display_name="a",
                directories=[
                    DirectoryNode(
                        relative_path=PurePosixPath("a/b"),
                        display_name="b",
                        directories=[
                            DirectoryNode(
                                relative_path=PurePosixPath("a/b/c"),
                                display_name="c",
                                directories=[
                                    DirectoryNode(
                                        relative_path=PurePosixPath("a/b/c/d"),
                                        display_name="d",
                                        directories=[
                                            DirectoryNode(
                                                relative_path=PurePosixPath("a/b/c/d/e"),
                                                display_name="e",
                                                directories=[
                                                    DirectoryNode(
                                                        relative_path=PurePosixPath("a/b/c/d/e/f"),
                                                        display_name="f",
                                                        files=[
                                                            FileNode(
                                                                relative_path=PurePosixPath("a/b/c/d/e/f/g.cpp"),
                                                                display_name="g",
                                                                extension=".cpp",
                                                            )
                                                        ],
                                                    )
                                                ],
                                            )
                                        ],
                                    )
                                ],
                            )
                        ],
                    )
                ],
            )
        ],
        files=[],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={},
        order_map={},
    )

    document = render_document(tree, config)

    assert r"\subparagraph{e}" in document
    assert r"\templatefiletitle{f / g}" in document
    assert r"\paragraph{f}" not in document
