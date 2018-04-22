#pragma once

#include <stdint.h>
#include <assert.h>

#define SAFE_DELETE(p)\
		if(p != NULL)\
		{\
			delete p;\
			p = NULL;\
		}

#define SAFE_DELETE_ARR(p)\
		if(p != NULL)\
		{\
			delete[] p;\
			p = NULL;\
		}

#define ASSERT_DO(assert_statement, do_statement)\
{\
    if (!(assert_statement))\
    {\
        assert(!#assert_statement);\
        do_statement;\
    }\
}

#define ASSERT_RETURN(assert_statement, return_value)\
    ASSERT_DO(assert_statement, return return_value)

#define ASSERT_RETURN_VOID(assert_statement)\
    ASSERT_RETURN(assert_statement, )

#define ASSERT_CONTINUE(assert_statement)\
    ASSERT_DO(assert_statement, continue)
