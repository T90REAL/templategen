PYTHON ?= python3
REPO ?= ./algotem
OUT ?= output/template.tex

.PHONY: template pdf test

template:
	PYTHONPATH=src $(PYTHON) -m templategen build $(REPO) --output $(OUT)

pdf:
	PYTHONPATH=src $(PYTHON) -m templategen build $(REPO) --output $(OUT) --pdf

test:
	pytest -v
