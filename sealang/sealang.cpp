#include "sealang.h"

#include "clang/AST/Stmt.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ExprCXX.h"

/************************************************************************
 * Duplicated libclang functionality
 *
 * The following methods are duplicates of methods that are implemented
 * in libclang, but aren't exposed as symbols that can be used by third-
 * party libraries.
 ************************************************************************/

namespace clang {
    enum CXStringFlag {
      /// CXString contains a 'const char *' that it doesn't own.
      CXS_Unmanaged,

      /// CXString contains a 'const char *' that it allocated with malloc().
      CXS_Malloc,

      /// CXString contains a CXStringBuf that needs to be returned to the
      /// CXStringPool.
      CXS_StringBuf
    };

    const clang::Stmt *getCursorStmt(CXCursor cursor) {
        if (cursor.kind == CXCursor_ObjCSuperClassRef ||
            cursor.kind == CXCursor_ObjCProtocolRef ||
            cursor.kind == CXCursor_ObjCClassRef) {

            return nullptr;
        }
        return static_cast<const clang::Stmt *>(cursor.data[1]);
    }

    const clang::Expr *getCursorExpr(CXCursor cursor) {
      return clang::dyn_cast_or_null<clang::Expr>(getCursorStmt(cursor));
    }

    namespace cxstring {

        CXString createEmpty() {
            CXString Str;
            Str.data = "";
            Str.private_flags = CXS_Unmanaged;
            return Str;
        }

        CXString createDup(StringRef String) {
            CXString Result;
            char *Spelling = static_cast<char *>(malloc(String.size() + 1));
            memmove(Spelling, String.data(), String.size());
            Spelling[String.size()] = 0;
            Result.data = Spelling;
            Result.private_flags = (unsigned) CXS_Malloc;
            return Result;
        }

    }
}

/************************************************************************
 * New Sealang functionality
 *
 * The following methods expose useful features of the LLVM AST. They are
 * all potentially candidates for inclusion upstream in libclang.
 ************************************************************************/

CXString clang_Cursor_getOperatorString(CXCursor cursor)
{
    if (cursor.kind == CXCursor_BinaryOperator) {
        clang::BinaryOperator *op = (clang::BinaryOperator *) clang::getCursorExpr(cursor);
        return clang::cxstring::createDup(clang::BinaryOperator::getOpcodeStr(op->getOpcode()));
    }

    if (cursor.kind == CXCursor_CompoundAssignOperator) {
        clang::CompoundAssignOperator *op = (clang::CompoundAssignOperator*) clang::getCursorExpr(cursor);
        return clang::cxstring::createDup(clang::BinaryOperator::getOpcodeStr(op->getOpcode()));
    }

    if (cursor.kind == CXCursor_UnaryOperator) {
        clang::UnaryOperator *op = (clang::UnaryOperator*) clang::getCursorExpr(cursor);
        return clang::cxstring::createDup(clang::UnaryOperator::getOpcodeStr(op->getOpcode()));
    }

    return clang::cxstring::createEmpty();
}

CXBinaryOpcode clang_Cursor_getBinaryOpcode(CXCursor cursor)
{
    if (cursor.kind == CXCursor_BinaryOperator) {
        clang::BinaryOperator *op = (clang::BinaryOperator *) clang::getCursorExpr(cursor);
        return static_cast<CXBinaryOpcode>(op->getOpcode());
    }

    if (cursor.kind == CXCursor_CompoundAssignOperator) {
        clang::CompoundAssignOperator *op = (clang::CompoundAssignOperator *) clang::getCursorExpr(cursor);
        return static_cast<CXBinaryOpcode>(op->getOpcode());
    }

    return BO_Unknown;
}

CXUnaryOpcode clang_Cursor_getUnaryOpcode(CXCursor cursor)
{
    if (cursor.kind == CXCursor_UnaryOperator) {
        clang::UnaryOperator *op = (clang::UnaryOperator*) clang::getCursorExpr(cursor);
        return static_cast<CXUnaryOpcode>(op->getOpcode());
    }

    return UO_Unknown;
}

CXString clang_Cursor_getLiteralString(CXCursor cursor)
{
    if (cursor.kind == CXCursor_IntegerLiteral) {
        clang::IntegerLiteral *intLiteral = (clang::IntegerLiteral *) clang::getCursorExpr(cursor);
        return clang::cxstring::createDup(intLiteral->getValue().toString(10, true));
    }

    if (cursor.kind == CXCursor_FloatingLiteral) {
        clang::FloatingLiteral *floatLiteral = (clang::FloatingLiteral *) clang::getCursorExpr(cursor);
        llvm::SmallString<1024> str;
        floatLiteral->getValue().toString(str);
        return clang::cxstring::createDup(str.c_str());
    }

    if (cursor.kind == CXCursor_CharacterLiteral) {
        clang::CharacterLiteral *charLiteral = (clang::CharacterLiteral *) clang::getCursorExpr(cursor);
        char c[2];
        c[0] = (char) charLiteral->getValue();
        c[1] = '\0';
        return clang::cxstring::createDup(c);
    }

    if (cursor.kind == CXCursor_StringLiteral) {
        clang::StringLiteral *stringLiteral = (clang::StringLiteral *) clang::getCursorExpr(cursor);
        return clang::cxstring::createDup(stringLiteral->getBytes());
    }

    if (cursor.kind == CXCursor_CXXBoolLiteralExpr) {
        clang::CXXBoolLiteralExpr *boolLiteral = (clang::CXXBoolLiteralExpr *) clang::getCursorExpr(cursor);
        return clang::cxstring::createDup(boolLiteral->getValue() ? "true" : "false");
    }

    return clang::cxstring::createEmpty();
}

// CXCursor clang_getForStmtInit(CXCursor cursor)
// {
//    if(cursor.kind!=CXCursor_ForStmt) return MakeCXCursorInvalid(CXCursor_InvalidCode);

//    ForStmt* Node=(ForStmt*)(cursor.data[1]);
//    const CXTranslationUnit tu=(const CXTranslationUnit)(cursor.data[2]);

//    Stmt* init=Node->getInit();
//    if (init) return MakeCXCursor(init,0,tu);
//    else return MakeCXCursorInvalid(CXCursor_NoDeclFound);
// }

// CXCursor clang_getForStmtCond(CXCursor cursor)
// {
//    if(cursor.kind!=CXCursor_ForStmt) return MakeCXCursorInvalid(CXCursor_InvalidCode);

//    ForStmt* Node=(ForStmt*)(cursor.data[1]);
//    const CXTranslationUnit tu=(const CXTranslationUnit)(cursor.data[2]);

//    Stmt* cond=Node->getCond();
//    if (cond) return MakeCXCursor(cond,0,tu);
//    else return MakeCXCursorInvalid(CXCursor_NoDeclFound);
// }

// CXCursor clang_getForStmtInc(CXCursor cursor)
// {
//    if(cursor.kind!=CXCursor_ForStmt) return MakeCXCursorInvalid(CXCursor_InvalidCode);

//    ForStmt* Node=(ForStmt*)(cursor.data[1]);
//    const CXTranslationUnit tu=(const CXTranslationUnit)(cursor.data[2]);

//    Stmt* inc=Node->getInc();
//    if (inc) return MakeCXCursor(inc,0,tu);
//    else return MakeCXCursorInvalid(CXCursor_NoDeclFound);
// }

// CXCursor clang_getForStmtBody(CXCursor cursor)
// {
//    if(cursor.kind!=CXCursor_ForStmt) return MakeCXCursorInvalid(CXCursor_InvalidCode);

//    ForStmt* Node=(ForStmt*)(cursor.data[1]);
//    const CXTranslationUnit tu=(const CXTranslationUnit)(cursor.data[2]);

//    Stmt* body=Node->getBody();
//    if (body) return MakeCXCursor(body,0,tu);
//    else return MakeCXCursorInvalid(CXCursor_NoDeclFound);
// }

/************************************************************************
 * Python module definition
 *
 * This is a stub module definition; we aren't exposing any Python
 * methods - we're just making the module .so easy to find.
 ************************************************************************/

static PyMethodDef methods[] = {
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef sealangmodule = {
    PyModuleDef_HEAD_INIT,
    "sealang",
    NULL,
    -1,
    methods
};

PyMODINIT_FUNC PyInit_sealang()
{
    return PyModule_Create(&sealangmodule);
}
