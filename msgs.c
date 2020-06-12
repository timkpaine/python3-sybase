/******************************************************************

 Copyright 2001 by Object Craft P/L, Melbourne, Australia.

 LICENCE - see LICENCE file distributed with this software for details.

******************************************************************/

#include "sybasect.h"

static int clientmsg_serial;

PyObject *clientmsg_alloc()
{
    CS_CLIENTMSGObj *self;

    self = PyObject_NEW(CS_CLIENTMSGObj, &CS_CLIENTMSGType);
    if (self == NULL)
	return NULL;

    SY_LEAK_REG(self);
    memset(&self->msg, 0, sizeof(self->msg));
    self->serial = clientmsg_serial++;

    return (PyObject*)self;
}

static void CS_CLIENTMSG_dealloc(CS_CLIENTMSGObj *self)
{
    SY_LEAK_UNREG(self);
    PyObject_DEL(self);
}

#define CLIENT_OFF(x) offsetof(CS_CLIENTMSG, x)

/* Adapted from Sybase cstypes.h */
#if defined (SYB_LP64) || defined (_AIX)
#define T_MSGNUM T_UINT
#else
#define T_MSGNUM T_LONG
#endif

static PyMemberDef CS_CLIENTMSG_memberlist[] = {
    { "severity",  T_INT,    CLIENT_OFF(severity), READONLY },
    { "msgnumber", T_MSGNUM, CLIENT_OFF(msgnumber), READONLY },
    { "msgstring", T_STRING_INPLACE, CLIENT_OFF(msgstring), READONLY }, /* faked */
    { "osnumber",  T_INT,    CLIENT_OFF(osnumber), READONLY },
    { "osstring",  T_STRING_INPLACE, CLIENT_OFF(osstring), READONLY }, /* faked */
#ifndef HAVE_FREETDS
    { "status",    T_INT,    CLIENT_OFF(status), READONLY },
    { "sqlstate",  T_STRING_INPLACE, CLIENT_OFF(sqlstate), READONLY }, /* faked */
#endif
    { NULL }			/* Sentinel */
};

static PyObject *CS_CLIENTMSG_getattr(CS_CLIENTMSGObj *self, char *name)
{
    if (strcmp(name, "msgstring") == 0)
	return PyUnicode_FromStringAndSize(self->msg.msgstring,
					  self->msg.msgstringlen);
    if (strcmp(name, "osstring") == 0)
	return PyUnicode_FromStringAndSize(self->msg.osstring,
					  self->msg.osstringlen);
#ifndef HAVE_FREETDS
    if (strcmp(name, "sqlstate") == 0)
	return PyUnicode_FromStringAndSize((char *)self->msg.sqlstate,
					  self->msg.sqlstatelen);
#endif
    return PyObject_GenericGetAttr((PyObject *)&self->msg, PyUnicode_FromString(name));
}

static int CS_CLIENTMSG_setattr(CS_CLIENTMSGObj *self, char *name, PyObject *v)
{
    if (v == NULL) {
	PyErr_SetString(PyExc_AttributeError, "Cannot delete attribute");
	return -1;
    }
    return PyObject_GenericSetAttr((PyObject *)&self->msg, PyUnicode_FromString(name), v);
}

static char CS_CLIENTMSGType__doc__[] = 
"";

PyTypeObject CS_CLIENTMSGType = {
        PyVarObject_HEAD_INIT(NULL, 0)
    "ClientMsgType",		/*tp_name*/
    sizeof(CS_CLIENTMSGObj),	/*tp_basicsize*/
    0,				/*tp_itemsize*/
    /* methods */
    (destructor)CS_CLIENTMSG_dealloc,/*tp_dealloc*/
    (printfunc)0,		/*tp_print*/
    (getattrfunc)CS_CLIENTMSG_getattr, /*tp_getattr*/
    (setattrfunc)CS_CLIENTMSG_setattr, /*tp_setattr*/
    0,				/*tp_as_async*/
    (reprfunc)0,		/*tp_repr*/
    0,				/*tp_as_number*/
    0,				/*tp_as_sequence*/
    0,				/*tp_as_mapping*/
    (hashfunc)0,		/*tp_hash*/
    (ternaryfunc)0,		/*tp_call*/
    (reprfunc)0,		/*tp_str*/
    0,				/*tp_getattro*/
    0,				/*tp_setattro*/
    0,				/*tp_as_buffer*/
    0,				/*tp_flags*/
    CS_CLIENTMSGType__doc__,	/* Documentation string */
    0,				/*tp_traverse*/
    0,				/*tp_clear*/
    (richcmpfunc)0,			/*tp_compare*/
    0,				/*tp_weaklistoffset*/
    0,				/*tp_iter*/
    0,				/*tp_iternext*/
    0,
    CS_CLIENTMSG_memberlist
};

static int servermsg_serial;

PyObject *servermsg_alloc()
{
    CS_SERVERMSGObj *self;

    self = PyObject_NEW(CS_SERVERMSGObj, &CS_SERVERMSGType);
    if (self == NULL)
	return NULL;

    SY_LEAK_REG(self);
    memset(&self->msg, 0, sizeof(self->msg));
    self->serial = servermsg_serial++;

    return (PyObject*)self;
}

static void CS_SERVERMSG_dealloc(CS_SERVERMSGObj *self)
{
    SY_LEAK_UNREG(self);
    PyObject_DEL(self);
}

#define SERV_OFF(x) offsetof(CS_SERVERMSG, x)

static PyMemberDef CS_SERVERMSG_memberlist[] = {
    { "msgnumber", T_MSGNUM, SERV_OFF(msgnumber), READONLY },
    { "state",     T_INT,    SERV_OFF(state), READONLY },
    { "severity",  T_INT,    SERV_OFF(severity), READONLY },
    { "text",      T_STRING_INPLACE, SERV_OFF(text), READONLY }, /* faked */
    { "svrname",   T_STRING_INPLACE, SERV_OFF(svrname), READONLY }, /* faked */
    { "proc",      T_STRING_INPLACE, SERV_OFF(proc), READONLY }, /* faked */
    { "line",      T_INT,    SERV_OFF(line), READONLY },
    { "status",    T_INT,    SERV_OFF(status), READONLY },
#ifndef HAVE_FREETDS
    { "sqlstate",  T_STRING_INPLACE, SERV_OFF(sqlstate), READONLY }, /* faked */
#endif
    { NULL }			/* Sentinel */
};

static PyObject *CS_SERVERMSG_getattr(CS_SERVERMSGObj *self, char *name)
{
#ifndef HAVE_FREETDS
    if (strcmp(name, "text") == 0)
	return PyUnicode_FromStringAndSize(self->msg.text,
					  self->msg.textlen);
#endif
    if (strcmp(name, "svrname") == 0)
	return PyUnicode_FromStringAndSize(self->msg.svrname,
					  self->msg.svrnlen);
    if (strcmp(name, "proc") == 0)
	return PyUnicode_FromStringAndSize(self->msg.proc,
					  self->msg.proclen);
#ifndef HAVE_FREETDS
    if (strcmp(name, "sqlstate") == 0)
	return PyUnicode_FromStringAndSize((char *)self->msg.sqlstate,
					  self->msg.sqlstatelen);
#endif
    return PyObject_GenericGetAttr((PyObject *)&self->msg, name);
}

static int CS_SERVERMSG_setattr(CS_SERVERMSGObj *self, char *name, PyObject *v)
{
    if (v == NULL) {
	PyErr_SetString(PyExc_AttributeError, "Cannot delete attribute");
	return -1;
    }
    return PyObject_GenericSetAttr((PyObject *)&self->msg, name, v);
}

static char CS_SERVERMSGType__doc__[] = 
"";

PyTypeObject CS_SERVERMSGType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "ServerMsgType",		/*tp_name*/
    sizeof(CS_SERVERMSGObj),	/*tp_basicsize*/
    0,				/*tp_itemsize*/
    /* methods */
    (destructor)CS_SERVERMSG_dealloc,/*tp_dealloc*/
    (printfunc)0,		/*tp_print*/
    (getattrfunc)CS_SERVERMSG_getattr, /*tp_getattr*/
    (setattrfunc)CS_SERVERMSG_setattr, /*tp_setattr*/
    0,				/*tp_as_async*/
    (reprfunc)0,		/*tp_repr*/
    0,				/*tp_as_number*/
    0,				/*tp_as_sequence*/
    0,				/*tp_as_mapping*/
    (hashfunc)0,		/*tp_hash*/
    (ternaryfunc)0,		/*tp_call*/
    (reprfunc)0,		/*tp_str*/
    0,				/*tp_getattro*/
    0,				/*tp_setattro*/
    0,				/*tp_as_buffer*/
    0,				/*tp_flags*/
    CS_SERVERMSGType__doc__,	/* Documentation string */
    0,				/*tp_traverse*/
    0,				/*tp_clear*/
    (richcmpfunc)0,			/*tp_compare*/
    0,				/*tp_weaklistoffset*/
    0,				/*tp_iter*/
    0,				/*tp_iternext*/
    0,
    CS_SERVERMSG_memberlist
};
