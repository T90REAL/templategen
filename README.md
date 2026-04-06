# templategen

ICPC/XCPC 选手通常会维护一个**代码模板仓库**——将自己常用的算法和数据结构实现按类别整理成文件，比赛时翻阅查用。templategen 扫描这个仓库，按目录层级自动生成带目录页的 LaTeX 手册，并可直接编译为 PDF 打印带上赛场。

支持中文目录名，所有源文件内容直接内嵌到 `.tex` 中。

## 安装

需要 Python 3.11+。

```bash
pip install -e .
```

## 用法

```bash
# 生成 .tex
python -m templategen build /path/to/repo --output output/template.tex

# 生成 .tex 并直接编译为 PDF（需本地安装 XeLaTeX）
python -m templategen build /path/to/repo --output output/template.tex --pdf
```

## 端到端示例

假设你的模板仓库结构如下：

```
my-repo/
├── templategen.yml
├── 图论/
│   ├── Dijkstra.cpp
│   └── Dinic.cpp
└── 数据结构/
    ├── DSU.cpp
    └── 线段树/
        └── LazySegTree.cpp
```

`templategen.yml`：

```yaml
title: "队伍名称 Template"
author: "Alice, Bob, Carol"
cjk_font: "Noto Serif CJK SC"   # 留空则使用系统默认 CJK 字体
exclude:
  - .git
  - "*.py"
```

运行：

```bash
python -m templategen build my-repo --output output/template.tex --pdf
```

输出 `output/template.pdf`，包含封面、目录页，以及按目录层级排列的代码块。目录名作为章节标题，文件名（去掉后缀）作为代码块标题。

## 配置文件

配置文件放在仓库根目录，命名为 `templategen.yml`，所有字段均可选。

| 字段 | 默认值 | 说明 |
|------|--------|------|
| `title` | `ICPC Templates` | 封面标题 |
| `author` | 空 | 封面作者 |
| `cjk_font` | 空（系统默认） | CJK 字体名称 |
| `include_extensions` | `.cpp .cc .c .h .hpp .py .java` | 扫描的文件后缀 |
| `exclude` | `.git __pycache__ build dist` | 忽略规则（指定后完全替换默认值） |
| `rename` | 空 | 目录/文件重命名，`原名: 显示名` |
| `order` | 空 | 各目录内文件的排列顺序 |

## 编译为 PDF

### macOS

安装 MacTeX（含 XeLaTeX）：

```bash
brew install --cask mactex
```

中文推荐使用思源宋体：

```bash
brew install font-noto-serif-cjk
```

然后在 `templategen.yml` 中设置 `cjk_font: "Noto Serif CJK SC"`。

### Windows

安装 [TeX Live](https://www.tug.org/texlive/)（推荐完整版）或 [MiKTeX](https://miktex.org/)。Windows 系统字体（如 SimSun、SimHei）可直接使用：

```yaml
cjk_font: "SimSun"
```

### Overleaf

templategen 将所有源文件内容直接内嵌到 `.tex` 中，无需上传源代码——只需上传单个 `.tex` 文件。

1. 本地生成 `.tex`（不需要 `--pdf`）：`python -m templategen build my-repo --output template.tex`
2. 在 Overleaf 新建项目，上传 `template.tex`
3. 菜单 → **Compiler** → 改为 **XeLaTeX**，编译即可

Overleaf 内置 `Noto Serif CJK SC` 等 CJK 字体，可直接在配置中指定。
