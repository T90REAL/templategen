from __future__ import annotations

from dataclasses import dataclass, field
from pathlib import PurePosixPath


@dataclass(slots=True)
class FileNode:
    relative_path: PurePosixPath
    display_name: str
    extension: str
    content: str | None = None


@dataclass(slots=True)
class DirectoryNode:
    relative_path: PurePosixPath
    display_name: str
    directories: list["DirectoryNode"] = field(default_factory=list)
    files: list[FileNode] = field(default_factory=list)
