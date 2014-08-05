# Project must correspond to PC/*.cpp file with main function
PROJECTS = BtLatency
export PROJECTS

all:
	-@make -C PC
	-@$(foreach binary,$(PROJECTS:%=PC/build/%),ln -s $(binary) .;)

clean:
	-@make clean -C PC
	-@$(foreach binary,$(PROJECTS),rm $(binary);)

