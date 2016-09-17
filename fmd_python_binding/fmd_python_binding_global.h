#ifndef FMD_PYTHON_BINDING_GLOBAL_H
#define FMD_PYTHON_BINDING_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(FMD_PYTHON_BINDING_LIBRARY)
#  define FMD_PYTHON_BINDINGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define FMD_PYTHON_BINDINGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // FMD_PYTHON_BINDING_GLOBAL_H
