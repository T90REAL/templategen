from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path

import yaml


DEFAULT_INCLUDE_EXTENSIONS = frozenset({".cpp", ".cc", ".c", ".h", ".hpp", ".py", ".java"})
DEFAULT_EXCLUDE_PATTERNS = (".git", "__pycache__", "build", "dist")


@dataclass(frozen=True)
class DocumentMetadata:
    title: str = "ICPC Templates"
    author: str = ""


@dataclass(frozen=True)
class GeneratorConfig:
    metadata: DocumentMetadata
    include_extensions: frozenset[str]
    exclude_patterns: tuple[str, ...]
    rename_map: dict[str, str]
    order_map: dict[str, tuple[str, ...]]


def load_config(repo_root: Path, config_path: Path | None) -> GeneratorConfig:
    resolved_path = config_path or (repo_root / "templategen.yml")
    if not resolved_path.exists():
        return GeneratorConfig(
            metadata=DocumentMetadata(),
            include_extensions=DEFAULT_INCLUDE_EXTENSIONS,
            exclude_patterns=DEFAULT_EXCLUDE_PATTERNS,
            rename_map={},
            order_map={},
        )

    try:
        raw = yaml.safe_load(resolved_path.read_text(encoding="utf-8")) or {}
    except yaml.YAMLError as exc:
        raise ValueError(f"Invalid config: {resolved_path}") from exc

    metadata = DocumentMetadata(
        title=raw.get("title", "ICPC Templates"),
        author=raw.get("author", ""),
    )
    include_extensions = frozenset(raw.get("include_extensions", DEFAULT_INCLUDE_EXTENSIONS))
    exclude_patterns = tuple(raw.get("exclude", DEFAULT_EXCLUDE_PATTERNS))
    rename_map = dict(raw.get("rename", {}))
    order_map = {
        key: tuple(values)
        for key, values in dict(raw.get("order", {})).items()
    }
    return GeneratorConfig(
        metadata=metadata,
        include_extensions=include_extensions,
        exclude_patterns=exclude_patterns,
        rename_map=rename_map,
        order_map=order_map,
    )
