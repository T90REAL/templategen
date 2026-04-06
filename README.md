## templategen

Generate a LaTeX algorithm template from a normal code repository.

### Install

```bash
python3 -m pip install -e .[dev]
```

### Generate `.tex`

```bash
python3 -m templategen build /path/to/code-repo --output output/template.tex
```

### Generate `.pdf`

```bash
python3 -m templategen build /path/to/code-repo --output output/template.tex --pdf
```

### Optional config

If `/path/to/code-repo/templategen.yml` exists, the CLI loads it automatically.
Missing config fields fall back to defaults, so partial config files are valid.

### Local wrapper

```bash
make template REPO=/path/to/code-repo
make pdf REPO=/path/to/code-repo
```
