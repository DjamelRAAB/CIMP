#include <stdio.h>
#include <stdlib.h>
#include <check.h>
#include "../src/cli/parsing.h"
#include "../src/cli/cli.h"

START_TEST( parse_test ){
    cmd * c;
    c = constListCmds("open a");

    ck_assert_int_eq(c->nb_args, 1);
    ck_assert_str_eq("a", c->args[0]); 
}END_TEST;

Suite *parseSuite(){
    Suite *s;
    TCase *tcase;

    tcase = tcase_create("Parse");
    tcase_add_test(tcase, parse_test);

    s = suite_create("parseSuite");
    suite_add_tcase(s,tcase);

    return s;
}

int main ( void ) {
    int failures ;
    Suite *s ;
    SRunner * sr ;
    s = parseSuite();
    sr = srunner_create(s);
    srunner_run_all( sr , CK_VERBOSE );
    failures = srunner_ntests_failed(sr);
    srunner_free ( sr ) ;
    return (failures == 0) ? EXIT_SUCCESS : EXIT_FAILURE ;
}