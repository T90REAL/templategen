from pathlib import Path

import pytest

from templategen.build import BuildRequest, build_project


def test_build_project_writes_tex_without_pdf(tmp_path):
    repo_root = tmp_path / "repo"
    (repo_root / "graph").mkdir(parents=True)
    (repo_root / "graph" / "dinic.cpp").write_text("int main() { return 0; }\n", encoding="utf-8")
    output = tmp_path / "out" / "template.tex"

    result = build_project(
        BuildRequest(
            repo_root=repo_root,
            output=output,
            config_path=None,
            pdf=False,
        )
    )

    assert output.exists()
    assert result.tex_path == output
    assert result.pdf_path is None
    assert r"\lstinputlisting[language=C++]" in output.read_text(encoding="utf-8")


def test_build_project_invokes_compiler_when_pdf_is_enabled(tmp_path, monkeypatch):
    repo_root = tmp_path / "repo"
    (repo_root / "graph").mkdir(parents=True)
    (repo_root / "graph" / "dinic.cpp").write_text("int main() { return 0; }\n", encoding="utf-8")
    output = tmp_path / "out" / "template.tex"
    called: dict[str, Path] = {}

    def fake_compile(tex_path: Path) -> Path:
        called["tex_path"] = tex_path
        pdf_path = tex_path.with_suffix(".pdf")
        pdf_path.write_text("pdf", encoding="utf-8")
        return pdf_path

    monkeypatch.setattr("templategen.build.compile_pdf", fake_compile)

    result = build_project(
        BuildRequest(
            repo_root=repo_root,
            output=output,
            config_path=None,
            pdf=True,
        )
    )

    assert called["tex_path"] == output
    assert result.pdf_path == output.with_suffix(".pdf")


def test_build_project_keeps_tex_when_pdf_compilation_fails(tmp_path, monkeypatch):
    repo_root = tmp_path / "repo"
    (repo_root / "graph").mkdir(parents=True)
    (repo_root / "graph" / "dinic.cpp").write_text("int main() { return 0; }\n", encoding="utf-8")
    output = tmp_path / "out" / "template.tex"

    def fake_compile(_: Path) -> Path:
        raise RuntimeError("XeLaTeX failed. Inspect template.log")

    monkeypatch.setattr("templategen.build.compile_pdf", fake_compile)

    with pytest.raises(RuntimeError, match="template.log"):
        build_project(
            BuildRequest(
                repo_root=repo_root,
                output=output,
                config_path=None,
                pdf=True,
            )
        )

    assert output.exists()


def test_build_project_uses_absolute_listing_paths_for_latex(tmp_path):
    repo_root = tmp_path / "repo"
    (repo_root / "图论").mkdir(parents=True)
    source_file = repo_root / "图论" / "dinic.cpp"
    source_file.write_text("int main() { return 0; }\n", encoding="utf-8")
    output = tmp_path / "out" / "template.tex"

    build_project(
        BuildRequest(
            repo_root=repo_root,
            output=output,
            config_path=None,
            pdf=False,
        )
    )

    expected_path = source_file.resolve().as_posix()
    assert f"{{{expected_path}}}" in output.read_text(encoding="utf-8")
