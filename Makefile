# Project must correspond to PC/*.cpp file with main function
PROJECTS = BtLatency
export PROJECTS

all:
	-@make -C PC
	$(foreach binary,$(PROJECTS:%=PC/build/%),\
            -@ln -s $(binary) . 2>/dev/null || true;)

clean:
	-@make clean -C PC
	$(foreach binary,$(PROJECTS),\
            -@rm $(binary) 2>/dev/null || true;)

