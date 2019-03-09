PROJECT_PATHS:=$(dir $(shell find ./*/ -name Makefile))

.PHONY: all
all:
	$(foreach PATH,$(PROJECT_PATHS),make -C $(PATH) -j4;)

.PHONY: clean
clean:
	$(foreach PATH,$(PROJECT_PATHS),make -C $(PATH) clean;)
