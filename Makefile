CPPFLAGS += -Wall

ALL=fork-test pretend-evaluator

top: $(ALL)
fork-test:
pretend-evaluator:

clean:
	rm -f $(ALL)
