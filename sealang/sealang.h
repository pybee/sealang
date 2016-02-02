#ifndef SEALANG_H
#define SEALANG_H

#include "Python.h"

#include "clang-c/Index.h"
#include "clang-c/CXString.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    // Operators listed in order of precedence.
    BO_PtrMemD, BO_PtrMemI,       // [C++ 5.5] Pointer-to-member operators.
    BO_Mul, BO_Div, BO_Rem,       // [C99 6.5.5] Multiplicative operators.
    BO_Add, BO_Sub,               // [C99 6.5.6] Additive operators.
    BO_Shl, BO_Shr,               // [C99 6.5.7] Bitwise shift operators.
    BO_LT, BO_GT, BO_LE, BO_GE,   // [C99 6.5.8] Relational operators.
    BO_EQ, BO_NE,                 // [C99 6.5.9] Equality operators.
    BO_And,                       // [C99 6.5.10] Bitwise AND operator.
    BO_Xor,                       // [C99 6.5.11] Bitwise XOR operator.
    BO_Or,                        // [C99 6.5.12] Bitwise OR operator.
    BO_LAnd,                      // [C99 6.5.13] Logical AND operator.
    BO_LOr,                       // [C99 6.5.14] Logical OR operator.
    BO_Assign, BO_MulAssign,      // [C99 6.5.16] Assignment operators.
    BO_DivAssign, BO_RemAssign,
    BO_AddAssign, BO_SubAssign,
    BO_ShlAssign, BO_ShrAssign,
    BO_AndAssign, BO_XorAssign,
    BO_OrAssign,
    BO_Comma,                      // [C99 6.5.17] Comma operator.
    BO_Unknown
} CXBinaryOpcode;

typedef enum {
    UO_PostInc, UO_PostDec,  // [C99 6.5.2.4] Postfix increment and decrement
    UO_PreInc, UO_PreDec,    // [C99 6.5.3.1] Prefix increment and decrement
    UO_AddrOf, UO_Deref,     // [C99 6.5.3.2] Address and indirection
    UO_Plus, UO_Minus,       // [C99 6.5.3.3] Unary arithmetic
    UO_Not, UO_LNot,         // [C99 6.5.3.3] Unary arithmetic
    UO_Real, UO_Imag,        // "__real expr"/"__imag expr" Extension.
    UO_Extension,            // __extension__ marker.
    UO_Unknown
} CXUnaryOpcode;

/**
 * \brief Returns string representation of unary and binary operators
 */
CXString clang_Cursor_getOperatorString(CXCursor cursor);

/**
 * \brief Returns Opcode of binary operator
 */
CXBinaryOpcode clang_Cursor_getBinaryOpcode(CXCursor cursor);

/**
 * \brief Returns Opcode of unary operator
 */
CXUnaryOpcode clang_Cursor_getUnaryOpcode(CXCursor cursor);

/**
 * \brief Returns string representation of literal cursor (1.f, 1000L, etc)
 */
CXString clang_Cursor_getLiteralString(CXCursor cursor);

/**
 * \brief Returns for-loop init cursor [for(init;cond;inc)], or CXCursor_NoDeclFound if there is no decl,
 * or CXCursor_InvalidCode if C is not CXCursor_ForStmt
 */
// CXCursor clang_getForStmtInit(CXCursor C);

/**
 * \brief Returns for-loop condition cursor [for(init;cond;inc)], or CXCursor_NoDeclFound if there is no decl,
 * or CXCursor_InvalidCode if C is not CXCursor_ForStmt
 */
// CXCursor clang_getForStmtCond(CXCursor C);

/**
 * \brief Returns for-loop increment cursor [for(init;cond;inc)], or CXCursor_NoDeclFound if there is no decl,
 * or CXCursor_InvalidCode if C is not CXCursor_ForStmt
 */
// CXCursor clang_getForStmtInc(CXCursor C);

/**
 * \brief Returns for-loop body, or CXCursor_NoDeclFound if there is no decl,
 * or CXCursor_InvalidCode if C is not CXCursor_ForStmt
 */
// CXCursor clang_getForStmtBody(CXCursor C);


#ifdef __cplusplus
}
#endif

#endif
