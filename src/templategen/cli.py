from __future__ import annotations

import argparse
from dataclasses import dataclass
from pathlib import Path

from templategen.build import BuildRequest, build_project


@dataclass(frozen=True)
class CLIOptions:
    command: str
    repo_root: Path
    output: Path | None
    config_path: Path | None
    pdf: bool


def parse_args(argv: list[str] | None = None) -> CLIOptions:
    parser = argparse.ArgumentParser(prog="templategen")
    subparsers = parser.add_subparsers(dest="command", required=True)

    build = subparsers.add_parser("build")
    build.add_argument("repo_root", type=Path)
    build.add_argument("--output", type=Path)
    build.add_argument("--config", dest="config_path", type=Path)
    build.add_argument("--pdf", action="store_true")

    args = parser.parse_args(argv)
    return CLIOptions(
        command=args.command,
        repo_root=args.repo_root,
        output=args.output,
        config_path=args.config_path,
        pdf=args.pdf,
    )


def main(argv: list[str] | None = None) -> int:
    options = parse_args(argv)
    output = options.output or Path("output/template.tex")
    build_project(
        BuildRequest(
            repo_root=options.repo_root,
            output=output,
            config_path=options.config_path,
            pdf=options.pdf,
        )
    )
    return 0
