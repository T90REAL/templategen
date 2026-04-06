from pathlib import Path, PurePosixPath

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
