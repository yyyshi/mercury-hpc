#include "mercury_time.h"

#include "mercury_test_config.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    hg_time_t t1, t2, diff1, diff2;
    hg_time_t sleep_time = {1, 0};
    double epsilon = 1e-9;
    double t1_double, t2_double;
    int ret = EXIT_SUCCESS;

    (void) argc;
    (void) argv;

    printf("Current time: %s\n", hg_time_stamp());

    hg_time_get_current(&t1);

    hg_time_sleep(sleep_time);

    hg_time_get_current(&t2);

    /* Should have slept at least sleep_time */
    if (!hg_time_less(t1, t2)) {
        fprintf(stderr, "Error: t1 > t2\n");
        ret = EXIT_FAILURE;
        goto done;
    }

    t1_double = hg_time_to_double(t1);
    t2_double = hg_time_to_double(t2);

    if (t1_double > t2_double) {
        fprintf(stderr, "Error: t1 > t2\n");
        ret = EXIT_FAILURE;
        goto done;
    }

    t1 = hg_time_from_double(t1_double);
    t2 = hg_time_from_double(t2_double);

    if (!hg_time_less(t1, t2)) {
        fprintf(stderr, "Error: t1 > t2\n");
        ret = EXIT_FAILURE;
        goto done;
    }

    /* t2 - (t1 + sleep_time) */
    diff1 = hg_time_subtract(t2, hg_time_add(t1, sleep_time));
    /* (t2 - t1) - sleep_time */
    diff2 = hg_time_subtract(hg_time_subtract(t2, t1), sleep_time);

    /* Should be equal */
    if (fabs(hg_time_to_double(diff1) - hg_time_to_double(diff2)) > epsilon) {
        fprintf(stderr, "Error: diff1 != diff2\n");
        ret = EXIT_FAILURE;
        goto done;
    }

done:
    return ret;
}
