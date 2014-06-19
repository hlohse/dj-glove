include Makefile.include
.PHONY: all clean

all:
	@$(foreach dir,$(ARCH_DIRS),$(MAKE) -C $(dir);)

clean:
	@$(foreach dir,$(ARCH_DIRS),$(MAKE) clean -C $(dir);)

