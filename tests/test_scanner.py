from pathlib import Path, PurePosixPath

import pytest

from templategen.config import load_config
from templategen.scanner import scan_repository


def test_scan_repository_recurses_and_skips_non_code_files(tmp_path):
    repo_root = tmp_path / "repo"
    (repo_root / "graph" / "shortest").mkdir(parents=True)
    (repo_root / "graph" / "shortest" / "dijkstra.cpp").write_text(
        "int main() { return 0; }\n",
        encoding="utf-8",
    )
    (repo_root / "graph" / "notes.md").write_text("# ignore\n", encoding="utf-8")
    (repo_root / "__pycache__").mkdir()
    (repo_root / "__pycache__" / "junk.py").write_text("pass\n", encoding="utf-8")

    tree = scan_repository(repo_root, load_config(repo_root, None))

    assert [directory.display_name for directory in tree.directories] == ["graph"]
    graph = tree.directories[0]
    assert [directory.display_name for directory in graph.directories] == ["shortest"]
    shortest = graph.directories[0]
    assert [file.display_name for file in shortest.files] == ["dijkstra"]
    assert shortest.files[0].relative_path == PurePosixPath("graph/shortest/dijkstra.cpp")


def test_scan_repository_uses_natural_sort_for_files(tmp_path):
    repo_root = tmp_path / "repo"
    (repo_root / "dp").mkdir(parents=True)
    (repo_root / "dp" / "dp10.cpp").write_text("int x = 10;\n", encoding="utf-8")
    (repo_root / "dp" / "dp2.cpp").write_text("int x = 2;\n", encoding="utf-8")

    tree = scan_repository(repo_root, load_config(repo_root, None))

    dp_dir = tree.directories[0]
    assert [file.display_name for file in dp_dir.files] == ["dp2", "dp10"]


def test_scan_repository_uses_deterministic_tiebreak_for_natural_sort(tmp_path, monkeypatch):
    repo_root = tmp_path / "repo"
    tie_dir = repo_root / "ties"
    tie_dir.mkdir(parents=True)
    for name in ("file2.cpp", "file02.cpp", "file1.cpp", "file01.cpp"):
        (tie_dir / name).write_text("int x = 0;\n", encoding="utf-8")

    original_iterdir = Path.iterdir

    def reversed_tie_dir_iterdir(self):
        children = list(original_iterdir(self))
        if self == tie_dir:
            return iter(reversed(children))
        return iter(children)

    monkeypatch.setattr(Path, "iterdir", reversed_tie_dir_iterdir)

    tree = scan_repository(repo_root, load_config(repo_root, None))

    ties = tree.directories[0]
    assert [file.relative_path.name for file in ties.files] == [
        "file01.cpp",
        "file1.cpp",
        "file02.cpp",
        "file2.cpp",
    ]


def test_scan_repository_sorts_mixed_natural_shapes_without_crashing(tmp_path):
    repo_root = tmp_path / "repo"
    docs_dir = repo_root / "docs"
    docs_dir.mkdir(parents=True)
    for name in ("chapter2.cpp", "chapter.cpp", "chapter10.cpp"):
        (docs_dir / name).write_text("int x = 0;\n", encoding="utf-8")

    tree = scan_repository(repo_root, load_config(repo_root, None))

    docs = tree.directories[0]
    assert [file.display_name for file in docs.files] == [
        "chapter",
        "chapter2",
        "chapter10",
    ]


def test_scan_repository_skips_directory_symlinks(tmp_path):
    repo_root = tmp_path / "repo"
    external_root = tmp_path / "external"
    (repo_root / "local").mkdir(parents=True)
    external_root.mkdir(parents=True)
    (repo_root / "local" / "inside.cpp").write_text("int local_file = 1;\n", encoding="utf-8")
    (external_root / "outside.cpp").write_text("int external_file = 1;\n", encoding="utf-8")
    (repo_root / "linked").symlink_to(external_root, target_is_directory=True)

    tree = scan_repository(repo_root, load_config(repo_root, None))

    assert [directory.display_name for directory in tree.directories] == ["local"]
    local = tree.directories[0]
    assert [file.display_name for file in local.files] == ["inside"]


def test_scan_repository_warns_and_skips_unreadable_paths(tmp_path, monkeypatch):
    repo_root = tmp_path / "repo"
    unreadable_dir = repo_root / "unreadable-dir"
    unreadable_path = repo_root / "broken.cpp"
    readable_dir = repo_root / "ok"
    unreadable_dir.mkdir(parents=True)
    readable_dir.mkdir(parents=True)
    unreadable_path.write_text("int broken = 0;\n", encoding="utf-8")
    (readable_dir / "good.cpp").write_text("int good = 0;\n", encoding="utf-8")

    original_iterdir = Path.iterdir
    original_is_dir = Path.is_dir

    def patched_iterdir(self):
        if self == unreadable_dir:
            raise OSError("blocked directory")
        return original_iterdir(self)

    def patched_is_dir(self):
        if self == unreadable_path:
            raise OSError("blocked path")
        return original_is_dir(self)

    monkeypatch.setattr(Path, "iterdir", patched_iterdir)
    monkeypatch.setattr(Path, "is_dir", patched_is_dir)

    with pytest.warns(UserWarning, match="Skipping unreadable path"):
        with pytest.warns(UserWarning, match="Skipping unreadable directory"):
            tree = scan_repository(repo_root, load_config(repo_root, None))

    assert [directory.display_name for directory in tree.directories] == ["ok"]
    assert [file.display_name for file in tree.directories[0].files] == ["good"]
