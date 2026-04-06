from __future__ import annotations

from dataclasses import dataclass
from collections.abc import Mapping
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


def _invalid_config(path: Path) -> ValueError:
    return ValueError(f"Invalid config: {path}")


def _ensure_mapping(value: object, path: Path) -> Mapping[str, object]:
    if not isinstance(value, Mapping):
        raise _invalid_config(path)
    return value


def _ensure_string_sequence(value: object, path: Path) -> tuple[str, ...]:
    if isinstance(value, (str, bytes)) or not isinstance(value, (list, tuple)):
        raise _invalid_config(path)
    if any(not isinstance(item, str) for item in value):
        raise _invalid_config(path)
    return tuple(value)


def _ensure_string_mapping(value: object, path: Path) -> dict[str, str]:
    mapping = _ensure_mapping(value, path)
    result: dict[str, str] = {}
    for key, item in mapping.items():
        if not isinstance(key, str) or not isinstance(item, str):
            raise _invalid_config(path)
        result[key] = item
    return result


def load_config(repo_root: Path, config_path: Path | None) -> GeneratorConfig:
    resolved_path = config_path or (repo_root / "templategen.yml")
    if config_path is not None and not resolved_path.exists():
        raise ValueError(f"Config file does not exist: {resolved_path}")
    if config_path is None and not resolved_path.exists():
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
        raise _invalid_config(resolved_path) from exc

    raw = _ensure_mapping(raw, resolved_path)

    title = raw.get("title", "ICPC Templates")
    author = raw.get("author", "")
    if not isinstance(title, str) or not isinstance(author, str):
        raise _invalid_config(resolved_path)

    metadata = DocumentMetadata(
        title=title,
        author=author,
    )

    include_extensions_raw = raw.get("include_extensions", DEFAULT_INCLUDE_EXTENSIONS)
    include_extensions = frozenset(_ensure_string_sequence(include_extensions_raw, resolved_path))

    exclude_raw = raw.get("exclude", DEFAULT_EXCLUDE_PATTERNS)
    exclude_patterns = _ensure_string_sequence(exclude_raw, resolved_path)

    rename_map_raw = raw.get("rename", {})
    rename_map = _ensure_string_mapping(rename_map_raw, resolved_path)

    order_raw = raw.get("order", {})
    order_mapping = _ensure_mapping(order_raw, resolved_path)
    order_map = {
        key: _ensure_string_sequence(values, resolved_path)
        for key, values in order_mapping.items()
    }
    return GeneratorConfig(
        metadata=metadata,
        include_extensions=include_extensions,
        exclude_patterns=exclude_patterns,
        rename_map=rename_map,
        order_map=order_map,
    )
