from __future__ import annotations

from pathlib import Path

from templategen.config import GeneratorConfig
from templategen.model import DirectoryNode, FileNode


def escape_latex(text: str) -> str:
    replacements = {
        "\\": r"\textbackslash{}",
        "&": r"\&",
        "%": r"\%",
        "$": r"\$",
        "#": r"\#",
        "_": r"\_",
        "{": r"\{",
        "}": r"\}",
        "~": r"\textasciitilde{}",
        "^": r"\textasciicircum{}",
    }
    return "".join(replacements.get(char, char) for char in text)


def language_for_extension(extension: str) -> str:
    return {
        ".cpp": "C++",
        ".cc": "C++",
        ".c": "C",
        ".h": "C++",
        ".hpp": "C++",
        ".py": "Python",
        ".java": "Java",
    }.get(extension, "text")


def render_document(root: DirectoryNode, config: GeneratorConfig) -> str:
    template_path = Path(__file__).resolve().parent / "templates" / "base.tex"
    template_text = template_path.read_text(encoding="utf-8")
    body = _render_directory(root, depth=0, folded_prefix=())
    cjk_font_line = (
        rf"\setCJKmainfont{{{config.metadata.cjk_font}}}"
        if config.metadata.cjk_font
        else ""
    )
    result = template_text
    result = result.replace("%%TITLE%%", escape_latex(config.metadata.title))
    result = result.replace("%%AUTHOR%%", escape_latex(config.metadata.author))
    result = result.replace("%%CJK_FONT%%", cjk_font_line)
    result = result.replace("%%BODY%%", body)
    return result


def _render_directory(
    directory: DirectoryNode,
    depth: int,
    folded_prefix: tuple[str, ...],
) -> str:
    parts: list[str] = []
    if depth > 0:
        if depth <= 5:
            parts.append(f"{_heading_command(depth)}{{{escape_latex(directory.display_name)}}}")
            next_prefix = folded_prefix
        else:
            next_prefix = (*folded_prefix, directory.display_name)
    else:
        next_prefix = folded_prefix

    for child in directory.directories:
        parts.append(_render_directory(child, depth + 1, next_prefix))
    for file_node in directory.files:
        parts.append(_render_file(file_node, next_prefix))

    return "\n".join(part for part in parts if part)


def _render_file(file_node: FileNode, folded_prefix: tuple[str, ...]) -> str:
    display_parts = [*folded_prefix, file_node.display_name]
    display_name = " / ".join(part for part in display_parts if part)
    return "\n".join(
        [
            f"\\templatefiletitle{{{escape_latex(display_name)}}}",
            (
                f"\\lstinputlisting[language={language_for_extension(file_node.extension)}]"
                f"{{{file_node.relative_path.as_posix()}}}"
            ),
        ]
    )


def _heading_command(depth: int) -> str:
    mapping = {
        1: "\\section",
        2: "\\subsection",
        3: "\\subsubsection",
        4: "\\paragraph",
        5: "\\subparagraph",
    }
    return mapping.get(depth, "\\paragraph")
