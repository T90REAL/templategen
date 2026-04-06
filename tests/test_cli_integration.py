import os
from pathlib import Path
import subprocess
import sys


def test_python_module_build_command_creates_tex(tmp_path):
    repo_root = tmp_path / "repo"
    (repo_root / "graph").mkdir(parents=True)
    (repo_root / "graph" / "dinic.cpp").write_text("int main() { return 0; }\n", encoding="utf-8")
    output = tmp_path / "template.tex"

    env = dict(os.environ)
    env["PYTHONPATH"] = str(Path.cwd() / "src")

    completed = subprocess.run(
        [
            sys.executable,
            "-m",
            "templategen",
            "build",
            str(repo_root),
            "--output",
            str(output),
        ],
        cwd=Path.cwd(),
        env=env,
        capture_output=True,
        text=True,
        check=False,
    )

    assert completed.returncode == 0
    assert output.exists()
    assert "Wrote TeX to" in completed.stdout
