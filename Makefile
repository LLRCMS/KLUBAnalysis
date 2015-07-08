SRC=src

.PHONY: all clean cleandict exe test

all:
	$(MAKE) -C $(SRC)
cleandict:
	$(MAKE) -C $(SRC) cleandict
clean:
	$(MAKE) -C $(SRC) clean
exe:
	$(MAKE) -C $(SRC) exe
test:
	$(MAKE) -C $(SRC) test