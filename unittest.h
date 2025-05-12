#ifndef UNITTEST_H
#define UNITTEST_H

#define unit_assert(message, test) do { if (!(test)) return message; else return NULL; } while (0)
#define unit_test(test) do { char *message = test(); UNIT_TEST_COUNT++; if (message) return message; } while (0)

extern int UNIT_TEST_COUNT;

#endif // UNITTEST_H

#ifdef UNITTEST_IMPLEMENTATION

int UNIT_TEST_COUNT = 0;

#endif // UNITTEST_IMPLEMENTATION
