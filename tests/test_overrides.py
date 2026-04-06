from pathlib import PurePosixPath
import warnings

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


def test_apply_overrides_preserves_existing_order_without_matching_order_rule():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[
            DirectoryNode(relative_path=PurePosixPath("zeta"), display_name="zeta"),
            DirectoryNode(relative_path=PurePosixPath("alpha"), display_name="alpha"),
        ],
        files=[
            FileNode(
                relative_path=PurePosixPath("zeta.cpp"),
                display_name="zeta",
                extension=".cpp",
            ),
            FileNode(
                relative_path=PurePosixPath("alpha.cpp"),
                display_name="alpha",
                extension=".cpp",
            ),
        ],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={},
        order_map={"graph": ("unused",)},
    )

    overridden = apply_overrides(tree, config)

    assert [directory.display_name for directory in overridden.directories] == ["zeta", "alpha"]
    assert [file.display_name for file in overridden.files] == ["zeta", "alpha"]


def test_apply_overrides_treats_empty_order_rule_as_matched():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[
            DirectoryNode(relative_path=PurePosixPath("zeta"), display_name="zeta"),
            DirectoryNode(relative_path=PurePosixPath("alpha"), display_name="alpha"),
        ],
        files=[],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={},
        order_map={"root": ()},
    )

    with warnings.catch_warnings(record=True) as caught:
        warnings.simplefilter("always")
        apply_overrides(tree, config)

    assert [str(item.message) for item in caught] == []


def test_apply_overrides_reserves_root_key_for_top_level_only():
    tree = DirectoryNode(
        relative_path=PurePosixPath("."),
        display_name="",
        directories=[
            DirectoryNode(
                relative_path=PurePosixPath("root"),
                display_name="root",
                files=[
                    FileNode(
                        relative_path=PurePosixPath("root/zeta.cpp"),
                        display_name="zeta",
                        extension=".cpp",
                    ),
                    FileNode(
                        relative_path=PurePosixPath("root/alpha.cpp"),
                        display_name="alpha",
                        extension=".cpp",
                    ),
                ],
            ),
            DirectoryNode(relative_path=PurePosixPath("alpha"), display_name="alpha"),
        ],
        files=[],
    )
    config = GeneratorConfig(
        metadata=DocumentMetadata(),
        include_extensions=frozenset({".cpp"}),
        exclude_patterns=(),
        rename_map={},
        order_map={"root": ("alpha", "root")},
    )

    overridden = apply_overrides(tree, config)

    assert [directory.display_name for directory in overridden.directories] == ["alpha", "root"]
    assert [file.display_name for file in overridden.directories[1].files] == ["zeta", "alpha"]
