CC = gcc
MTM_OBJS = matamikya.o matamikya_print.o matamikya_main.o matamikya_tests.o
MTM_EXEC = matamikya

AS_AMOUNT_STR_OBJ = amount_set_str.o amount_set_str_main.o amount_set_str_tests.o
AS_STR_EXEC = amount_set_str

DEBUG_FLAGS = -g -DNDEBUG
CFLAGS = -std=c99 -Wall -Werror --pedantic-errors $(DEBUG_FLAGS)
CPPFLAGS = -L. -lmtm -lm -las

#Matamikya 
$(MTM_EXEC) : $(MTM_OBJS) 
	$(CC) $(CFLAGS) $(MTM_OBJS) -o $@ $(CPPFLAGS)

matamikya.o: matamikya.c matamikya.h amount_set.h matamikya_print.h
matamikya_print.o: matamikya_print.c matamikya_print.h
matamikya_main.o: tests/matamikya_main.c tests/matamikya_tests.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) tests/matamikya_main.c -o $@
matamikya_tests.o: tests/matamikya_tests.c tests/matamikya_tests.h matamikya.h tests/test_utilities.h
	$(CC) -c $(CFLAGS) tests/matamikya_tests.c -o $@


#Amount_set_str
$(AS_STR_EXEC) : $(AS_AMOUNT_STR_OBJ) 
	$(CC) $(CFLAGS) $(AS_AMOUNT_STR_OBJ) -o $@

amount_set_str.o: amount_set_str.c amount_set_str.h 
amount_set_str_main.o: amount_set_str_main.c amount_set_str_tests.h amount_set_str.h amount_set_str_test_utilities.h
amount_set_str_tests.o: amount_set_str_tests.c amount_set_str_tests.h amount_set_str.h amount_set_str_test_utilities.h

clean:
	rm -f $(MTM_OBJS) $(MTM_EXEC) $(AS_AMOUNT_STR_OBJ) $(AS_STR_EXEC)