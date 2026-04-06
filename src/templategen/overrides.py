from __future__ import annotations

from copy import deepcopy
import warnings

from templategen.config import GeneratorConfig
from templategen.model import DirectoryNode
from templategen.scanner import natural_sort_key


def apply_overrides(root: DirectoryNode, config: GeneratorConfig) -> DirectoryNode:
    cloned = deepcopy(root)
    matched_rename: set[str] = set()
    matched_order: set[str] = set()
    _apply_rename(cloned, config, matched_rename)
    _apply_order(cloned, config, matched_order)

    for key in sorted(config.rename_map):
        if key not in matched_rename:
            warnings.warn(f"Unused rename rule: {key}", stacklevel=2)
    for key in sorted(config.order_map):
        if key not in matched_order:
            warnings.warn(f"Unused order rule: {key}", stacklevel=2)

    return cloned
def _apply_rename(
    directory: DirectoryNode,
    config: GeneratorConfig,
    matched_rename: set[str],
) -> None:
    directory_key = directory.relative_path.as_posix()
    if directory_key in config.rename_map:
        directory.display_name = config.rename_map[directory_key]
        matched_rename.add(directory_key)

    for file_node in directory.files:
        file_key = file_node.relative_path.as_posix()
        if file_key in config.rename_map:
            file_node.display_name = config.rename_map[file_key]
            matched_rename.add(file_key)

    for child in directory.directories:
        _apply_rename(child, config, matched_rename)


def _apply_order(
    directory: DirectoryNode,
    config: GeneratorConfig,
    matched_order: set[str],
) -> None:
    is_top_level = directory.relative_path.as_posix() == "."
    if is_top_level:
        has_order_rule = "root" in config.order_map
        key = "root"
    else:
        key = directory.relative_path.as_posix()
        has_order_rule = key in config.order_map and key != "root"

    if has_order_rule:
        matched_order.add(key)
        desired = config.order_map[key]
        rank = {name: index for index, name in enumerate(desired)}

        directory.directories.sort(
            key=lambda node: (rank.get(node.display_name, len(rank)), natural_sort_key(node.display_name))
        )
        directory.files.sort(
            key=lambda node: (
                rank.get(node.display_name, len(rank)),
                natural_sort_key(node.display_name),
                node.relative_path.name,
            )
        )

    for child in directory.directories:
        _apply_order(child, config, matched_order)
