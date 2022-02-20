#include <stdio.h>
#include <iostream>

#include "test.h"

class Hardt_Test: public Test
{
    public:

        void run()
        {
            UNITTEST(check_versions);
        }

        const char* name()
        {
            return "Hardt";
        }

    private:

        void check_versions()
        {
            ASSERT_IS_TRUE(isVersion(MAJORVERSION, MINORVERSION, BUILDNO));
            ASSERT_IS_TRUE(!isVersion(MAJORVERSION - 1, MINORVERSION, BUILDNO));
            ASSERT_IS_TRUE(!isVersion(MAJORVERSION, MINORVERSION - 1, BUILDNO));
            ASSERT_IS_TRUE(!isVersion(MAJORVERSION, MINORVERSION, BUILDNO - 1));
            ASSERT_IS_TRUE(!isVersion(MAJORVERSION + 1, MINORVERSION, BUILDNO));
            ASSERT_IS_TRUE(!isVersion(MAJORVERSION, MINORVERSION + 1, BUILDNO));
            ASSERT_IS_TRUE(!isVersion(MAJORVERSION, MINORVERSION, BUILDNO + 1));

            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION, MINORVERSION, BUILDNO));
            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION - 1, MINORVERSION, BUILDNO));
            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION, MINORVERSION - 1, BUILDNO));
            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION, MINORVERSION, BUILDNO - 1));
            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION - 1, MINORVERSION - 1, BUILDNO));
            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION - 1, MINORVERSION, BUILDNO - 1));
            ASSERT_IS_TRUE(isVersionOrNewer(MAJORVERSION - 1, MINORVERSION - 1, BUILDNO - 1));
            ASSERT_IS_TRUE(!isVersionOrNewer(MAJORVERSION + 1, MINORVERSION, BUILDNO));
            ASSERT_IS_TRUE(!isVersionOrNewer(MAJORVERSION, MINORVERSION + 1, BUILDNO));
            ASSERT_IS_TRUE(!isVersionOrNewer(MAJORVERSION, MINORVERSION, BUILDNO + 1));

            ASSERT_IS_TRUE(isVersionMajor(MAJORVERSION));
            ASSERT_IS_TRUE(!isVersionMajor(MAJORVERSION - 1));
            ASSERT_IS_TRUE(!isVersionMajor(MAJORVERSION + 1));

            ASSERT_IS_TRUE(isVersionMajorOrNewer(MAJORVERSION));
            ASSERT_IS_TRUE(isVersionMajorOrNewer(MAJORVERSION - 1));
            ASSERT_IS_TRUE(!isVersionMajorOrNewer(MAJORVERSION + 1));

            ASSERT_IS_TRUE(isVersionMajorOrOlder(MAJORVERSION));
            ASSERT_IS_TRUE(isVersionMajorOrOlder(MAJORVERSION + 1));
            ASSERT_IS_TRUE(!isVersionMajorOrOlder(MAJORVERSION - 1));
        }

} hardt_test;