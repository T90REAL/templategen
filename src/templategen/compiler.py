from __future__ import annotations

from pathlib import Path
import subprocess


def compile_pdf(tex_path: Path) -> Path:
    command = ["xelatex", "-interaction=nonstopmode", tex_path.name]
    completed = subprocess.run(
        command,
        cwd=tex_path.parent,
        capture_output=True,
        text=True,
        check=False,
    )
    if completed.returncode != 0:
        log_path = tex_path.with_suffix(".log")
        raise RuntimeError(f"XeLaTeX failed. Inspect {log_path}")
    return tex_path.with_suffix(".pdf")
