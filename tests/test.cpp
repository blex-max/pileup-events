#include <catch2/catch_test_macros.hpp>

#include "const.hpp"
#include "pileup.hpp"

TEST_CASE ("score single") {
    std::vector<int> res (N_FIELDS_PER_OBS * 2,
                          0); // init result arr, two position long
    count_params cp; // defaults fine
    AEVSettings aevst; // defaults fine (discard_overlaps=false)
    AlleleEventCounter aev (cp, res, aevst);


    {
        BaseInfo b{HTS_NT_A, 0, 30}; // a very normal a
        aev._score_single (b, 0);
        CAPTURE (res);
        REQUIRE (res[FIELD_A] == 1);
        REQUIRE (res[FIELD_MAPQ] == 30);
        REQUIRE (res[FIELD_NOBS] == 1);
        int sum = 0;
        for (int x : res)
            sum += x;
        REQUIRE (sum == 32); // rest 0
    }

    {
        BaseInfo b{HTS_NT_C, FLAG_REV, 30}; // rev C
        aev._score_single (b, 0);
        REQUIRE (res[FIELD_C + RSTRAND_OFFSET] == 1);
        REQUIRE (res[FIELD_MAPQ + RSTRAND_OFFSET] == 30);
        REQUIRE (res[FIELD_NOBS + RSTRAND_OFFSET] == 1);
        int sum = 0;
        for (int x : res)
            sum += x;
        REQUIRE (sum == 64); // rest unchanged
    }

    {
        BaseInfo b{HTS_NT_G, FLAG_QUAL_FAIL, 30}; // failing G
        aev._score_single (b, 1); // next pos
        CAPTURE (res);
        REQUIRE (res[FIELD_N + N_FIELDS_PER_OBS] == 1);
        REQUIRE (res[FIELD_MAPQ + N_FIELDS_PER_OBS] == 30);
        REQUIRE (res[FIELD_NOBS + N_FIELDS_PER_OBS] == 1);
        int sum = 0;
        for (int x : res)
            sum += x;
        REQUIRE (sum == 96); // rest 0
    }

    {
        BaseInfo b{HTS_NT_T, FLAG_REV, 30}; // rev C
        aev._score_single (b, 1);
        REQUIRE (res[FIELD_T + RSTRAND_OFFSET + N_FIELDS_PER_OBS] ==
                 1);
        REQUIRE (
            res[FIELD_MAPQ + RSTRAND_OFFSET + N_FIELDS_PER_OBS] ==
            30);
        REQUIRE (
            res[FIELD_NOBS + RSTRAND_OFFSET + N_FIELDS_PER_OBS] == 1);
        int sum = 0;
        for (int x : res)
            sum += x;
        REQUIRE (sum == 128); // rest unchanged
    }
}
