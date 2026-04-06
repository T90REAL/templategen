from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from pathlib import PurePosixPath

from templategen.compiler import compile_pdf
from templategen.config import load_config
from templategen.model import DirectoryNode
from templategen.overrides import apply_overrides
from templategen.render import render_document
from templategen.scanner import scan_repository


@dataclass(frozen=True)
class BuildRequest:
    repo_root: Path
    output: Path
    config_path: Path | None
    pdf: bool


@dataclass(frozen=True)
class BuildResult:
    tex_path: Path
    pdf_path: Path | None


def build_project(request: BuildRequest) -> BuildResult:
    config = load_config(request.repo_root, request.config_path)
    scanned_tree = scan_repository(request.repo_root, config)
    document_tree = apply_overrides(scanned_tree, config)
    _materialize_listing_paths(document_tree, request.repo_root)
    latex = render_document(document_tree, config)

    request.output.parent.mkdir(parents=True, exist_ok=True)
    request.output.write_text(latex, encoding="utf-8")

    pdf_path = compile_pdf(request.output) if request.pdf else None
    return BuildResult(tex_path=request.output, pdf_path=pdf_path)


def _materialize_listing_paths(
    directory: DirectoryNode,
    repo_root: Path,
) -> None:
    for file_node in directory.files:
        source_path = (repo_root / file_node.relative_path).resolve()
        file_node.relative_path = PurePosixPath(source_path.as_posix())

    for child in directory.directories:
        _materialize_listing_paths(child, repo_root)
