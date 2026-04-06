from pathlib import PurePosixPath

import pytest

from templategen.config import DocumentMetadata, GeneratorConfig
from templategen.model import DirectoryNode, FileNode
from templategen.overrides import apply_overrides


def test_apply_overrides_renames_paths_and_orders_root_children():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[
            DirectoryNode(relative_path=PurePosixPath("graph"), display_name="graph"),
            DirectoryNode(relative_path=PurePosixPath("math"), display_name="math"),
        ],
        files=[],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={"graph": "图论"},
        order_map={"root": ("math", "图论")},
    )

    overridden = apply_overrides(tree, config)

    assert [directory.display_name for directory in overridden.directories] == ["math", "图论"]


def test_apply_overrides_renames_files_by_relative_path():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[],
        files=[
            FileNode(
                relative_path=PurePosixPath("graph/dinic.cpp"),
                display_name="dinic",
                extension=".cpp",
            )
        ],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={"graph/dinic.cpp": "Dinic"},
        order_map={},
    )

    overridden = apply_overrides(tree, config)

    assert overridden.files[0].display_name == "Dinic"


def test_apply_overrides_warns_for_unmatched_rules():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[],
        files=[],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={"missing/path.cpp": "Missing"},
        order_map={"missing": ("A",)},
    )

    with pytest.warns(UserWarning) as caught:
        apply_overrides(tree, config)

    messages = [str(item.message) for item in caught]
    assert messages == [
        "Unused rename rule: missing/path.cpp",
        "Unused order rule: missing",
    ]
