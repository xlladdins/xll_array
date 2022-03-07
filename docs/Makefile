DBG = ../x64/Debug
REL = ../x64/Release
DOCS = ../xll/docs
XML = $(wildcard $(DBG)/*.xml)
HTML = $(subst .xml,.html, $(notdir $(XML)))

%.html : $(DBG)/%.xml
	xsltproc --path $(DOCS) $< -o $@

all: docs

docs: $(HTML)
	cp $(DOCS)/*.css $(DBG)/index.html .

clean:
	rm *.html *.css
	rm $(DBG)/*.xml
