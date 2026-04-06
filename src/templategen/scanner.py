from __future__ import annotations

import re
import warnings
from fnmatch import fnmatch
from pathlib import Path, PurePosixPath

from templategen.config import GeneratorConfig
from templategen.model import DirectoryNode, FileNode


_SORT_RE = re.compile(r"(\d+)")


def natural_sort_key(value: str) -> tuple[tuple[int, int | str, str], ...]:
    parts = _SORT_RE.split(value)
    key: list[tuple[int, int | str, str]] = []
    for part in parts:
        if part.isdigit():
            key.append((1, int(part), part))
            continue
        key.append((0, part.lower(), part))
    return tuple(key)


def _is_excluded(relative_path: PurePosixPath, patterns: tuple[str, ...]) -> bool:
    path_text = relative_path.as_posix()
    return any(
        fnmatch(path_text, pattern) or any(fnmatch(part, pattern) for part in relative_path.parts)
        for pattern in patterns
    )


def scan_repository(repo_root: Path, config: GeneratorConfig) -> DirectoryNode:
    if not repo_root.exists():
        raise FileNotFoundError(f"Repository does not exist: {repo_root}")
    if not repo_root.is_dir():
        raise NotADirectoryError(f"Repository root is not a directory: {repo_root}")

    normalized_config = GeneratorConfig(
        metadata=config.metadata,
        include_extensions=frozenset(extension.lower() for extension in config.include_extensions),
        exclude_patterns=config.exclude_patterns,
        rename_map=config.rename_map,
        order_map=config.order_map,
    )
    root = _scan_directory(repo_root, repo_root, normalized_config)
    if not root.directories and not root.files:
        raise ValueError("No includable files found")
    return root


def _scan_directory(current: Path, repo_root: Path, config: GeneratorConfig) -> DirectoryNode:
    relative_path = (
        PurePosixPath(".")
        if current == repo_root
        else PurePosixPath(current.relative_to(repo_root).as_posix())
    )
    node = DirectoryNode(
        relative_path=relative_path,
        display_name="" if relative_path == PurePosixPath(".") else current.name,
    )

    directories: list[DirectoryNode] = []
    files: list[FileNode] = []

    try:
        children = sorted(current.iterdir(), key=lambda path: natural_sort_key(path.name))
    except OSError as exc:
        warnings.warn(f"Skipping unreadable directory {current}: {exc}", stacklevel=2)
        return node

    for child in children:
        child_relative = PurePosixPath(child.relative_to(repo_root).as_posix())
        if _is_excluded(child_relative, config.exclude_patterns):
            continue

        try:
            if child.is_symlink() and child.is_dir():
                continue
            is_dir = child.is_dir()
        except OSError as exc:
            warnings.warn(f"Skipping unreadable path {child}: {exc}", stacklevel=2)
            continue

        if is_dir:
            child_node = _scan_directory(child, repo_root, config)
            if child_node.directories or child_node.files:
                directories.append(child_node)
            continue

        if child.suffix.lower() not in config.include_extensions:
            continue

        files.append(
            FileNode(
                relative_path=child_relative,
                display_name=child.stem,
                extension=child.suffix.lower(),
            )
        )

    directories.sort(key=lambda directory: natural_sort_key(directory.display_name))
    files.sort(key=lambda file_node: (natural_sort_key(file_node.display_name), file_node.relative_path.name))
    node.directories = directories
    node.files = files
    return node
