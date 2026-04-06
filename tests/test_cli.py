from pathlib import Path

from templategen.cli import parse_args


def test_parse_args_build_defaults():
    options = parse_args(["build", "sample-repo"])
    assert options.command == "build"
    assert options.repo_root == Path("sample-repo")
    assert options.output is None
    assert options.config_path is None
    assert options.pdf is False


def test_parse_args_build_with_optional_flags():
    options = parse_args(
        [
            "build",
            "sample-repo",
            "--output",
            "out/template.tex",
            "--config",
            "sample-repo/templategen.yml",
            "--pdf",
        ]
    )
    assert options.output == Path("out/template.tex")
    assert options.config_path == Path("sample-repo/templategen.yml")
    assert options.pdf is True
