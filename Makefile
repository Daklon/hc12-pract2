all:
	cd lib; make
	cd src; make

# TODO: Configurar documentación para pract2, no para SimpleGEL
doc:
	doxygen ./doc/Doxyfile

.PHONY: all doc
