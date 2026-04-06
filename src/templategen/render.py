from __future__ import annotations

from pathlib import Path

from templategen.config import GeneratorConfig
from templategen.model import DirectoryNode, FileNode


def _escape_title(text: str) -> str:
    """Escape a title string, converting \\n or actual newlines to LaTeX line breaks."""
    lines = text.replace("\\n", "\n").split("\n")
    return r" \\ ".join(escape_latex(line.strip()) for line in lines if line.strip())


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
    title_oneline = escape_latex(config.metadata.title.replace("\\n", " ").replace("\n", " ").strip())
    result = result.replace("%%TITLE_ONELINE%%", title_oneline)
    result = result.replace("%%TITLE%%", _escape_title(config.metadata.title))
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
        parts.append(_render_file(file_node, next_prefix, depth))

    return "\n".join(part for part in parts if part)


def _render_file(file_node: FileNode, folded_prefix: tuple[str, ...], parent_depth: int) -> str:
    display_parts = [*folded_prefix, file_node.display_name]
    display_name = " / ".join(part for part in display_parts if part)
    lang = language_for_extension(file_node.extension)
    toc_level = _toc_level(parent_depth + 1)
    toc_entry = f"\\phantomsection\\addcontentsline{{toc}}{{{toc_level}}}{{{escape_latex(display_name)}}}"
    title_line = f"\\templatefiletitle{{{escape_latex(display_name)}}}"
    if file_node.content is not None:
        body = file_node.content
        if not body.endswith("\n"):
            body += "\n"
        listing = f"\\begin{{lstlisting}}[language={lang}]\n{body}\\end{{lstlisting}}"
    else:
        listing = f"\\lstinputlisting[language={lang}]{{{file_node.relative_path.as_posix()}}}"
    return "\n".join([toc_entry, title_line, listing])


def _heading_command(depth: int) -> str:
    mapping = {
        1: "\\section",
        2: "\\subsection",
        3: "\\subsubsection",
        4: "\\paragraph",
        5: "\\subparagraph",
    }
    return mapping.get(depth, "\\paragraph")


def _toc_level(depth: int) -> str:
    return {
        1: "section",
        2: "subsection",
        3: "subsubsection",
        4: "paragraph",
        5: "subparagraph",
    }.get(depth, "subparagraph")
