/*
Copyright 2016 Colin Girling

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

******************************************************************************

Using the macros TEST, TEST_FUNCTION, TEST_MEMBER_FUNCTION
and TEST_CONST_MEMBER_FUNCTION require the names are unique.
*/

#include "Test.hpp"

// Example functions for demonstrating how to write unit tests
// for functions using TEST_FUNCTION.

bool FuncOneArg(int i)
{
    return i != 0;
}

bool FuncTwoArgs(int i1, int i2)
{
    return i1 + i2 != 0;
}

// Example class for demonstrating how to write unit tests
// for member functions and const member functions using
// TEST_MEMBER_FUNCTION and TEST_CONST_MEMBER_FUNCTION.
//
// There are two versions for testing member functions,
// so that the output can display overloaded const and non-const versions.

class Example
{
public:
    Example(int value = 0)
        : m_value(value)
    {
    }

    Example(Example const& other)
        : m_value(other.m_value)
    {
    }

    Example& operator =(Example const& other)
    {
        m_value = other.m_value;
        return *this;
    }

    bool operator ==(Example const& other) const
    {
        return m_value == other.m_value;
    }

    int GetValue() const
    {
        return m_value;
    }

    void SetValue(int value)
    {
        m_value = value;
    }

    void Inc()
    {
        ++m_value;
    }

private:
    int m_value;
};


// When a test fails, the output is on multiple lines,
// so this macro can set the number of spaces to indent the failure messages.
TEST_FAILURE_INDENT(4);

// Implement unit tests for functions and member functions.

TEST_FUNCTION(FuncOneArg, int)
{
    CHECK_FALSE(FuncOneArg(0));
    CHECK_TRUE(FuncOneArg(1));
    CHECK_TRUE(FuncOneArg(-1));
}

// Because the arguments within TEST_FUNCTION can only use characters that
// would be valid for a variable name, the ARGS part of TEST_FUNCTION uses
// underscore to represent comma separated function arguments.
TEST_FUNCTION(FuncOneArg, int_int)
{
    TEST_OVERRIDE_ARGS("int,int");
}

// Because we cannot use == in function name for macro, is_equal is used
// and TEST_OVERRIDE_FUNCTION_NAME is used to update output in logging.
TEST_CONST_MEMBER_FUNCTION(Example, operator_is_equal, Example_const_ref)
{
    TEST_OVERRIDE_FUNCTION_NAME("operator ==");
    TEST_OVERRIDE_ARGS("Example const&");

    /*
     * Could also use TEST_OVERRIDE_FUNCTION_NAME_ARGS, but for this example
     * to show how to set name and args separately, the above was used.
     * Could also do this: TEST_OVERRIDE_FUNCTION_NAME("operator ==", "Example const&");
     */

    Example example1, example2;
    CHECK_TRUE(example1 == example2);

    example1.SetValue(1);
    CHECK_FALSE(example1 == example2);

    example2.SetValue(1);
    CHECK_TRUE(example1 == example2);
}

// Test the constructor for Example class.
// When args is set to NA the framework understand the function has no arguments.
// NA is an abbreviation for Not Applicable.
TEST_CONST_MEMBER_FUNCTION(Example, GetValue, NA)
{
    Example example;

    CHECK_ZERO(example.GetValue());
    example.SetValue(1);
    CHECK_EQUAL(example.GetValue(), 1);
}

TEST_MEMBER_FUNCTION(Example, Inc, NA)
{
    Example example;
    CHECK_ZERO(example.GetValue());
    example.Inc();
    CHECK_EQUAL(example.GetValue(), 1);
}

// When testing a combination of function calls,
// TEST macro can be used for this purpose.
TEST(ComboTesting)
{
    Example example(1);
    CHECK_EQUAL(example.GetValue(), 1);
    example.Inc();
    CHECK_EQUAL(example.GetValue(), 2);
    example.SetValue(-1);
    CHECK_EQUAL(example.GetValue(), -1);
    example.Inc();
    CHECK_EQUAL(example.GetValue(), 0);
}

// Enable or disable main, as these tests compile and these tests could
// be added to a different project for debugging, if desired.
#define INCLUDE_MAIN_FUNCTION

#ifdef INCLUDE_MAIN_FUNCTION

int main(int /*argc*/, char * /*argv*/[])
{
    // Currently an empty stub that will be used to demonstrate
    // a command line unit test executable returning error codes.

    bool test_has_failed = TEST_HAS_FAILED;

    return test_has_failed ? 1 : 0;
}

#endif // INCLUDE_MAIN_FUNCTION
