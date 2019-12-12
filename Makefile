all:
	cd lib; make
	cd src; make

# TODO: Configurar documentación para pract2, no para SimpleGEL
doc:
	cd SimpleGEL/doc; make

.PHONY: all doc
