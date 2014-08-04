# Project must correspond to PC/*.cpp file with main function
PROJECTS = BtLatency Entchen
export PROJECTS

all:
	-@make -C PC
	-@$(foreach binary,$(PROJECTS:%=PC/build/%),ln -s $(binary) .;)

clean:
	-@make clean -C PC
	-@$(foreach binary,$(PROJECTS),rm $(binary);)

