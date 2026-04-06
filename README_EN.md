# templategen

中文 | [English](README_EN.md)

![Python 3.11+](https://img.shields.io/badge/Python-3.11%2B-blue)
![Dependency PyYAML](https://img.shields.io/badge/Dependency-PyYAML-informational)
![XeLaTeX Optional](https://img.shields.io/badge/PDF-XeLaTeX%20optional-success)
![License MIT](https://img.shields.io/badge/License-MIT-green)

`templategen` turns a local competitive programming template repo into a single-file LaTeX handbook.

It recursively scans the directory tree, maps directories to chapters, maps source files to code blocks, and produces a ready-to-compile `.tex` file.

## Installation

Requires Python 3.11+.

```bash
git clone https://github.com/your-username/xcpc-template.git
cd xcpc-template
pip install -e .
```

## Quick Start

The repo ships with a ready-to-run example directory: `template_dir/` (the author's personal algorithm templates from 2024).

```text
template_dir/
├── templategen.yml
├── 图论/
├── 字符串/
├── 数学/
├── 数据结构/
│   └── 线段树/
└── 杂项/
```

The key fields in the example config:

```yaml
title: "XCPC Algorithm Template \n (Team DBL for 2024 ICPC Regional Contest)"
author: "Xiao Li"

include_extensions:
  - .cpp
  - .py

exclude:
  - .git
  - __pycache__
  - build
  - dist
  - config
```

Generate a `.tex` file:

```bash
python -m templategen build template_dir --output output/template.tex
```

Or use the Makefile shortcut:

```bash
make template REPO=template_dir OUT=output/template.tex
```

If XeLaTeX is installed locally, you can compile to PDF directly:

```bash
python -m templategen build template_dir --output output/template.tex --pdf
```

## Usage

### CLI

```bash
# Use templategen.yml in the repo root
python -m templategen build /path/to/repo

# Specify output path explicitly
python -m templategen build /path/to/repo --output output/template.tex

# Specify config file explicitly
python -m templategen build /path/to/repo --config /path/to/templategen.yml

# Compile to PDF directly
python -m templategen build /path/to/repo --output output/template.tex --pdf
```

Current CLI behavior:

- `repo_root` is required
- `--output` is optional; defaults to `output/template.tex`
- `--config` is optional; defaults to `templategen.yml` in the target repo root
- `--pdf` is optional; only recommended if `xelatex` is installed locally

### Makefile Shortcuts

```bash
make template REPO=/path/to/repo OUT=output/template.tex
make pdf REPO=/path/to/repo OUT=output/template.tex
make test
```

## Configuration

The config file must be named `templategen.yml` and is expected in the target repo root by default. You can point to a different location with `--config`.

All fields are optional. The tool works without any config file.

| Field | Default | Description |
|-------|---------|-------------|
| `title` | `ICPC Templates` | Cover title |
| `author` | (empty) | Cover author |
| `cjk_font` | (empty) | CJK font name for XeLaTeX; uses template default when empty |
| `include_extensions` | `.cpp .cc .c .h .hpp .py .java` | File extensions to include |
| `exclude` | `.git __pycache__ build dist` | Exclusion rules; overrides defaults when explicitly set |
| `rename` | (empty) | Display name mapping for directories or files |
| `order` | (empty) | Display order within each directory |

### `rename`

Keys in `rename` use relative paths.

- Rename a directory: `"图论": "Graph"`
- Rename a subdirectory: `"数据结构/线段树": "Segment Tree"`
- Rename a file: `"图论/Dinic.cpp": "Dinic Maxflow"`

### `order`

Keys in `order` also use relative paths, but the top-level directory must be written as `root`.

```yaml
order:
  root:
    - 图论
    - 字符串
    - 数学
    - 数据结构
  数据结构:
    - 线段树
```

A few notes:

- `order.root` controls the order of top-level directories and files
- Subdirectory ordering uses the directory's relative path as the key
- `order` matches against the current display name, so if a node has been `rename`d, use the renamed name in `order`
- Entries not matched by `order` are appended in natural sort order
- Unmatched `rename` / `order` rules produce a warning to help catch typos in the config

## PDF Render

### macOS

Install the full MacTeX distribution, which includes XeLaTeX and common fonts:

```bash
brew install --cask mactex
```

For CJK support, install Noto Serif CJK and specify it in the config:

```bash
brew install font-noto-serif-cjk
```

```yaml
cjk_font: "Noto Serif CJK SC"
```

Then compile with `--pdf`:

```bash
python -m templategen build /path/to/repo --output output/template.tex --pdf
```

### Windows (untested)

Install [TeX Live](https://www.tug.org/texlive/) (full scheme) or [MiKTeX](https://miktex.org/).

- **TeX Live**: Download `install-tl-windows.exe` and choose the full installation (~7 GB). Restart the terminal after installation and `xelatex` will be available.
- **MiKTeX**: Missing packages are downloaded automatically on first compile. Confirm XeLaTeX is set as the default compiler in MiKTeX Console.

Windows ships with CJK fonts like SimSun and SimHei that can be used directly:

```yaml
cjk_font: "SimSun"   # Song typeface
# cjk_font: "SimHei" # Hei typeface
```

Then compile with `--pdf`:

```bash
python -m templategen build /path/to/repo --output output/template.tex --pdf
```

### Overleaf

templategen embeds all source code directly into the `.tex` file — **you only need to upload the single `.tex`**.

1. Generate the `.tex` locally (no `--pdf` needed):
   ```bash
   python -m templategen build /path/to/repo --output template.tex
   ```
2. Create a new project on Overleaf and upload `template.tex`
3. Click the menu in the top-left → **Compiler** → switch to **XeLaTeX**
4. Click **Recompile**

Overleaf has `Noto Serif CJK SC` and similar fonts built in. Specify the font in your config when your content includes CJK characters:

```yaml
cjk_font: "Noto Serif CJK SC"
```

## Contributing

Contributions are welcome — add algorithm templates under `template_dir/` in the appropriate category, or submit improvements to templategen itself. For bugs or feature requests, open an Issue or PR.
