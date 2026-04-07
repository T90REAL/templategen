# templategen

[English](README_EN.md) | 中文

[![Python 3.11+](https://img.shields.io/badge/Python-3.11%2B-blue)](https://www.python.org/downloads/)
[![Dependency PyYAML](https://img.shields.io/badge/Dependency-PyYAML-informational)](https://pypi.org/project/PyYAML/)
[![XeLaTeX Optional](https://img.shields.io/badge/PDF-XeLaTeX%20optional-success)](https://tug.org/xetex/)
[![License MIT](https://img.shields.io/badge/License-MIT-green)](LICENSE)

`templategen` 用来把一个本地算法模板仓自动整理成 LaTeX 手册。

它会递归扫描真实目录树，把目录映射成章节，把代码文件映射成代码块，并生成可直接继续编译的 `.tex` 文件。

## Installation

需要 Python 3.11+。

```bash
git clone https://github.com/T90REAL/templategen.git
cd templategen
pip install -e .
```


## Quick Start

仓库里已经包含了一个可直接运行的真实示例目录：`template_dir/` (2024年自用的算法模板)。

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

这个示例目录下的配置文件核心内容如下：

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

直接生成 `.tex`：

```bash
python -m templategen build template_dir --output output/template.tex
```

或者使用仓库里的快捷命令：

```bash
make template REPO=template_dir OUT=output/template.tex
```

如果本地已经装了Latex编译工具也可以直接生成 PDF：

```bash
python -m templategen build template_dir --output output/template.tex --pdf
```

## Usage

### CLI

```bash
# 使用仓库根目录下的 templategen.yml
python -m templategen build /path/to/repo

# 显式指定输出路径
python -m templategen build /path/to/repo --output output/template.tex

# 显式指定配置文件
python -m templategen build /path/to/repo --config /path/to/templategen.yml

# 直接生成 PDF
python -m templategen build /path/to/repo --output output/template.tex --pdf
```

当前 CLI 行为：

- `repo_root` 是必填参数
- `--output` 可省略，默认输出到 `output/template.tex`
- `--config` 可省略，默认尝试读取目标仓库根目录下的 `templategen.yml`
- `--pdf` 为可选项，只有本机装了 `xelatex` 时才建议开启

### Makefile Shortcuts

仓库内也提供了简单包装：

```bash
make template REPO=/path/to/repo OUT=output/template.tex
make pdf REPO=/path/to/repo OUT=output/template.tex
make test
```

## Configuration

配置文件名固定为 `templategen.yml`，默认放在目标仓库根目录；如果你不想放在根目录，也可以通过 `--config` 显式传入。

所有字段都是可选的。没有配置文件时，工具也能直接运行。

| 字段 | 默认值 | 说明 |
|------|--------|------|
| `title` | `ICPC Templates` | 封面标题 |
| `author` | 空 | 封面作者 |
| `cjk_font` | 空 | XeLaTeX 使用的 CJK 字体名；为空时使用模板默认行为 |
| `include_extensions` | `.cpp .cc .c .h .hpp .py .java` | 允许收录的文件后缀 |
| `exclude` | `.git __pycache__ build dist` | 忽略规则；一旦显式指定，会覆盖默认值 |
| `rename` | 空 | 目录或文件的显示名映射 |
| `order` | 空 | 指定各目录内的展示顺序 |

### `rename`

`rename` 的 key 使用相对路径。

- 目录重命名示例：`"图论": "Graph"`
- 子目录重命名示例：`"数据结构/线段树": "Segment Tree"`
- 文件重命名示例：`"图论/Dinic.cpp": "Dinic Maxflow"`

### `order`

`order` 的 key 也使用相对路径，但顶层目录必须写成 `root`。

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

几点需要注意：

- `order.root` 控制顶层目录和顶层文件的顺序
- 子目录排序使用该目录的相对路径作为 key
- `order` 匹配的是当前显示名，所以如果同一个节点被 `rename` 过，`order` 要写重命名后的名字
- 没被 `order` 命中的条目会按自然排序继续排在后面
- 未命中的 `rename` / `order` 规则会产生 warning，方便排查配置是否写错

## PDF Render

### macOS

推荐安装完整版 MacTeX，包含 XeLaTeX 和常用字体：

```bash
brew install --cask mactex
```

中文支持推荐安装思源宋体，并在配置文件中指定：

```bash
brew install font-noto-serif-cjk
```

```yaml
cjk_font: "Noto Serif CJK SC"
```

安装完成后直接加 `--pdf` 编译：

```bash
python -m templategen build /path/to/repo --output output/template.tex --pdf
```


### Windows (未测试)

推荐安装 [TeX Live](https://www.tug.org/texlive/)（完整版）或 [MiKTeX](https://miktex.org/)。

- **TeX Live**：下载 `install-tl-windows.exe`，选择完整方案安装（约 7 GB）。安装完成后重启终端，`xelatex` 命令即可使用。
- **MiKTeX**：安装后首次编译时会自动下载缺失宏包，无需手动配置。在 MiKTeX Console 确认默认编译器为 XeLaTeX。

Windows 系统内置宋体、黑体等中文字体，可直接在配置里使用：

```yaml
cjk_font: "SimSun"   # 宋体
# cjk_font: "SimHei" # 黑体
```

安装完成后同样通过 `--pdf` 编译：

```bash
python -m templategen build /path/to/repo --output output/template.tex --pdf
```

### Overleaf

templategen 将所有源码直接内嵌到 `.tex` 中，无需上传代码文件，**只需上传单个 `.tex`**。

1. 本地生成 `.tex`（无需 `--pdf`）：
   ```bash
   python -m templategen build /path/to/repo --output template.tex
   ```
2. 在 Overleaf 新建项目，上传生成的 `template.tex`
3. 点击左上角菜单 → **Compiler** → 切换为 **XeLaTeX**
4. 点击 **Recompile** 编译

Overleaf 内置 `Noto Serif CJK SC` 等字体，有中文内容时在配置里指定即可：

```yaml
cjk_font: "Noto Serif CJK SC"
```

## Contributing

欢迎同学们补充算法模板（在 `template_dir/` 下按分类新建文件即可）或者对 templategen 本身提交改进。如果遇到 bug 或有功能建议，欢迎提 Issue 或 PR。

