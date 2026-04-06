import pytest

from templategen.config import load_config


def test_load_config_uses_defaults_when_file_is_missing(tmp_path):
    repo_root = tmp_path / "repo"
    repo_root.mkdir()

    config = load_config(repo_root, None)

    assert config.metadata.title == "ICPC Templates"
    assert ".cpp" in config.include_extensions
    assert ".git" in config.exclude_patterns
    assert config.rename_map == {}
    assert config.order_map == {}


def test_load_config_merges_partial_yaml(tmp_path):
    repo_root = tmp_path / "repo"
    repo_root.mkdir()
    (repo_root / "templategen.yml").write_text(
        "title: Team Notebook\ninclude_extensions:\n  - .cpp\n  - .rs\n",
        encoding="utf-8",
    )

    config = load_config(repo_root, None)

    assert config.metadata.title == "Team Notebook"
    assert config.metadata.author == ""
    assert config.include_extensions == frozenset({".cpp", ".rs"})
    assert ".git" in config.exclude_patterns


def test_load_config_raises_for_invalid_yaml(tmp_path):
    repo_root = tmp_path / "repo"
    repo_root.mkdir()
    config_path = repo_root / "templategen.yml"
    config_path.write_text("title: [broken\n", encoding="utf-8")

    with pytest.raises(ValueError, match="Invalid config"):
        load_config(repo_root, config_path)
