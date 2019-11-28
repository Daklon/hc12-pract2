all:
	cd SimpleGEL/lib; make
	cd src; make

# TODO: Configurar documentación para pract2, no para SimpleGEL
doc:
	doxygen ./SimpleGEL/doc/Doxyfile

.PHONY: all doc
