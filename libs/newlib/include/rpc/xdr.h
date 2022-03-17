
/*
 * Copyright (c) 2009, Sun Microsystems, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of Sun Microsystems, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 *      from: @(#)xdr.h 1.19 87/04/22 SMI
 *      from: @(#)xdr.h        2.2 88/07/29 4.0 RPCSRC
 *      $FreeBSD: src/include/rpc/xdr.h,v 1.23 2003/03/07 13:19:40 nectar Exp $
 *      $NetBSD: xdr.h,v 1.19 2000/07/17 05:00:45 matt Exp $
 */

/*
 * xdr.h, External Data Representation Serialization Routines.
 *
 * Copyright (C) 1984, Sun Microsystems, Inc.
 */

#ifndef _RPC_XDR_H
#define _RPC_XDR_H
#include <_ansi.h>
#include <rpc/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * XDR provides a conventional way for converting between C data
 * types and an external bit-string representation.  Library supplied
 * routines provide for the conversion on built-in C data types.  These
 * routines and utility routines defined here are used to help implement
 * a type encode/decode routine for each user-defined type.
 *
 * Each data type provides a single procedure which takes two arguments:
 *
 *        bool_t
 *        xdrproc(XDR *xdrs, <type> *argresp)
 *
 * xdrs is an instance of a XDR handle, to which or from which the data
 * type is to be converted.  argresp is a pointer to the structure to be
 * converted.  The XDR handle contains an operation field which indicates
 * which of the operations (ENCODE, DECODE * or FREE) is to be performed.
 *
 * XDR_DECODE may allocate space if the pointer argresp is null.  This
 * data can be freed with the XDR_FREE operation.
 *
 * We write only one procedure per data type to make it easy
 * to keep the encode and decode procedures for a data type consistent.
 * In many cases the same code performs all operations on a user defined type,
 * because all the hard work is done in the component type routines.
 * decode as a series of calls on the nested data types.
 */

/*
 * Xdr operations.  XDR_ENCODE causes the type to be encoded into the
 * stream.  XDR_DECODE causes the type to be extracted from the stream.
 * XDR_FREE can be used to release the space allocated by an XDR_DECODE
 * request.
 */
enum xdr_op
{
    XDR_ENCODE = 0,
    XDR_DECODE = 1,
    XDR_FREE = 2
};

/*
 * This is the number of bytes per unit of external data.
 */
#define BYTES_PER_XDR_UNIT (4)
#if 1
/* faster version when BYTES_PER_XDR_UNIT is a power of two */
#define RNDUP(x) (((x) + BYTES_PER_XDR_UNIT - 1) & ~(BYTES_PER_XDR_UNIT - 1))
#else /* old version */
#define RNDUP(x) ((((x) + BYTES_PER_XDR_UNIT - 1) / BYTES_PER_XDR_UNIT) * BYTES_PER_XDR_UNIT)
#endif

/*
 * The XDR handle.
 * Contains operation which is being applied to the stream,
 * an operations vector for the particular implementation (e.g. see xdr_mem.c),
 * and two private fields for the use of the particular implementation.
 */
typedef struct __rpc_xdr
{
    enum xdr_op x_op; /* operation; fast additional param */
    _CONST struct xdr_ops
    {
        /* get a long from underlying stream */
        bool_t _EXFNPTR(x_getlong, (struct __rpc_xdr *, long *));

        /* put a long to " */
        bool_t _EXFNPTR(x_putlong, (struct __rpc_xdr *, _CONST long *));

        /* get some bytes from " */
        bool_t _EXFNPTR(x_getbytes, (struct __rpc_xdr *, char *, u_int));

        /* put some bytes to " */
        bool_t _EXFNPTR(x_putbytes, (struct __rpc_xdr *, _CONST char *, u_int));

        /* returns bytes off from beginning */
        u_int _EXFNPTR(x_getpostn, (struct __rpc_xdr *));

        /* lets you reposition the stream */
        bool_t _EXFNPTR(x_setpostn, (struct __rpc_xdr *, u_int));

        /* buf quick ptr to buffered data */
        int32_t *_EXFNPTR(x_inline, (struct __rpc_xdr *, u_int));

        /* free privates of this xdr_stream */
        void _EXFNPTR(x_destroy, (struct __rpc_xdr *));

        /* get an int32 from this xdr_stream */
        bool_t _EXFNPTR(x_getint32, (struct __rpc_xdr *, int32_t *));

        /* put an int32 to the underlying stream */
        bool_t _EXFNPTR(x_putint32, (struct __rpc_xdr *, _CONST int32_t *));

    } * x_ops;
    char *x_public;  /* users' data */
    void *x_private; /* pointer to private data */
    char *x_base;    /* private used for position info */
    u_int x_handy;   /* extra private word */
} XDR;

/*
 * A xdrproc_t exists for each data type which is to be encoded or decoded.
 *
 * The second argument to the xdrproc_t is a pointer to an opaque pointer.
 * The opaque pointer generally points to a structure of the data type
 * to be decoded.  If this pointer is 0, then the type routines should
 * allocate dynamic storage of the appropriate size and return it.
 * bool_t (*xdrproc_t)(XDR *, some_type *)
 */
typedef bool_t _EXFNPTR(xdrproc_t, (XDR *, ...));

/*
 * Operations defined on a XDR handle
 *
 * XDR   *xdrs;
 * long  *longp;
 * char  *addr;
 * u_int  len;
 * u_int  pos;
 */
#define XDR_GETINT32(xdrs, int32p) \
    (*(xdrs)->x_ops->x_getint32)(xdrs, int32p)
#define xdr_getint32(xdrs, int32p) \
    (*(xdrs)->x_ops->x_getint32)(xdrs, int32p)

#define XDR_PUTINT32(xdrs, int32p) \
    (*(xdrs)->x_ops->x_putint32)(xdrs, int32p)
#define xdr_putint32(xdrs, int32p) \
    (*(xdrs)->x_ops->x_putint32)(xdrs, int32p)

#define XDR_GETLONG(xdrs, longp) \
    (*(xdrs)->x_ops->x_getlong)(xdrs, longp)
#define xdr_getlong(xdrs, longp) \
    (*(xdrs)->x_ops->x_getlong)(xdrs, longp)

#define XDR_PUTLONG(xdrs, longp) \
    (*(xdrs)->x_ops->x_putlong)(xdrs, longp)
#define xdr_putlong(xdrs, longp) \
    (*(xdrs)->x_ops->x_putlong)(xdrs, longp)

#define XDR_GETBYTES(xdrs, addr, len) \
    (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)
#define xdr_getbytes(xdrs, addr, len) \
    (*(xdrs)->x_ops->x_getbytes)(xdrs, addr, len)

#define XDR_PUTBYTES(xdrs, addr, len) \
    (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)
#define xdr_putbytes(xdrs, addr, len) \
    (*(xdrs)->x_ops->x_putbytes)(xdrs, addr, len)

#define XDR_GETPOS(xdrs) \
    (*(xdrs)->x_ops->x_getpostn)(xdrs)
#define xdr_getpos(xdrs) \
    (*(xdrs)->x_ops->x_getpostn)(xdrs)

#define XDR_SETPOS(xdrs, pos) \
    (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)
#define xdr_setpos(xdrs, pos) \
    (*(xdrs)->x_ops->x_setpostn)(xdrs, pos)

#define XDR_INLINE(xdrs, len) \
    (*(xdrs)->x_ops->x_inline)(xdrs, len)
#define xdr_inline(xdrs, len) \
    (*(xdrs)->x_ops->x_inline)(xdrs, len)

#define XDR_DESTROY(xdrs)                      \
    do                                         \
    {                                          \
        if ((xdrs)->x_ops->x_destroy)          \
            (*(xdrs)->x_ops->x_destroy)(xdrs); \
    } while (0)
#define xdr_destroy(xdrs)                      \
    do                                         \
    {                                          \
        if ((xdrs)->x_ops->x_destroy)          \
            (*(xdrs)->x_ops->x_destroy)(xdrs); \
    } while (0)

/*
 * Solaris strips the '_t' from these types -- not sure why.
 * But, let's be compatible.
 */
#define xdr_rpcvers(xdrs, versp) xdr_u_int32(xdrs, versp)
#define xdr_rpcprog(xdrs, progp) xdr_u_int32(xdrs, progp)
#define xdr_rpcproc(xdrs, procp) xdr_u_int32(xdrs, procp)
#define xdr_rpcprot(xdrs, protp) xdr_u_int32(xdrs, protp)
#define xdr_rpcport(xdrs, portp) xdr_u_int32(xdrs, portp)

/*
 * Support struct for discriminated unions.
 * You create an array of xdrdiscrim structures, terminated with
 * an entry with a null procedure pointer.  The xdr_union routine gets
 * the discriminant value and then searches the array of structures
 * for a matching value.  If a match is found the associated xdr routine
 * is called to handle that part of the union.  If there is
 * no match, then a default routine may be called.
 * If there is no match and no default routine it is an error.
 */
#define NULL_xdrproc_t ((xdrproc_t)0)
struct xdr_discrim
{
    int value;
    xdrproc_t proc;
};

/*
 * In-line routines for fast encode/decode of primitive data types.
 * Caveat emptor: these use single memory cycles to get the
 * data from the underlying buffer, and will fail to operate
 * properly if the data is not aligned.  The standard way to use these
 * is to say:
 *        if ((buf = XDR_INLINE(xdrs, count)) == NULL)
 *            return (FALSE);
 *        <<< macro calls >>>
 * where ``count'' is the number of bytes of data occupied
 * by the primitive data types.
 *
 * N.B. and frozen for all time: each data type here uses 4 bytes
 * of external representation.
 */
#define IXDR_GET_INT32(buf) ((int32_t)ntohl((u_int32_t) * (buf)++))
#define IXDR_PUT_INT32(buf, v) (*(buf)++ = (int32_t)htonl((u_int32_t)v))
#define IXDR_GET_U_INT32(buf) ((uint32_t)IXDR_GET_INT32(buf))
#define IXDR_PUT_U_INT32(buf, v) IXDR_PUT_INT32((buf), ((int32_t)(v)))

/* Warning: inline long routines are broken for 64 bit platforms.
 * Because the other inline routines below are implemented in terms
 * of them, they are all also broken for 64 bit platforms.
 */
#define IXDR_GET_LONG(buf) ((long)ntohl((u_int32_t) * (buf)++))
#define IXDR_PUT_LONG(buf, v) (*(buf)++ = (int32_t)htonl((u_int32_t)v))
#define IXDR_GET_U_LONG(buf) ((u_long)IXDR_GET_LONG(buf))
#define IXDR_PUT_U_LONG(buf, v) IXDR_PUT_LONG((buf), (v))

#define IXDR_GET_BOOL(buf) ((bool_t)IXDR_GET_LONG(buf))
#define IXDR_GET_ENUM(buf, t) ((t)IXDR_GET_LONG(buf))
#define IXDR_GET_SHORT(buf) ((short)IXDR_GET_LONG(buf))
#define IXDR_GET_U_SHORT(buf) ((u_short)IXDR_GET_LONG(buf))

#define IXDR_PUT_BOOL(buf, v) IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_ENUM(buf, v) IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_SHORT(buf, v) IXDR_PUT_LONG((buf), (v))
#define IXDR_PUT_U_SHORT(buf, v) IXDR_PUT_LONG((buf), (v))

/*
 * These are the "generic" xdr routines.
 */
extern bool_t _EXFUN(xdr_void, (void));
extern bool_t _EXFUN(xdr_short, (XDR *, short *));
extern bool_t _EXFUN(xdr_u_short, (XDR *, u_short *));
extern bool_t _EXFUN(xdr_int, (XDR *, int *));
extern bool_t _EXFUN(xdr_u_int, (XDR *, u_int *));
extern bool_t _EXFUN(xdr_long, (XDR *, long *));
extern bool_t _EXFUN(xdr_u_long, (XDR *, u_long *));
extern bool_t _EXFUN(xdr_int8_t, (XDR *, int8_t *));
extern bool_t _EXFUN(xdr_uint8_t, (XDR *, uint8_t *));
extern bool_t _EXFUN(xdr_u_int8_t, (XDR *, u_int8_t *));
extern bool_t _EXFUN(xdr_int16_t, (XDR *, int16_t *));
extern bool_t _EXFUN(xdr_uint16_t, (XDR *, uint16_t *));
extern bool_t _EXFUN(xdr_u_int16_t, (XDR *, u_int16_t *));
extern bool_t _EXFUN(xdr_int32_t, (XDR *, int32_t *));
extern bool_t _EXFUN(xdr_uint32_t, (XDR *, uint32_t *));
extern bool_t _EXFUN(xdr_u_int32_t, (XDR *, u_int32_t *));
#if defined(___int64_t_defined)
extern bool_t _EXFUN(xdr_int64_t, (XDR *, int64_t *));
extern bool_t _EXFUN(xdr_uint64_t, (XDR *, uint64_t *));
extern bool_t _EXFUN(xdr_u_int64_t, (XDR *, u_int64_t *));
#endif /* ___int64_t_defined */
extern bool_t _EXFUN(xdr_bool, (XDR *, bool_t *));
extern bool_t _EXFUN(xdr_enum, (XDR *, enum_t *));
extern bool_t _EXFUN(xdr_array, (XDR *, char **, u_int *, u_int, u_int, xdrproc_t));
extern bool_t _EXFUN(xdr_bytes, (XDR *, char **, u_int *, u_int));
extern bool_t _EXFUN(xdr_opaque, (XDR *, char *, u_int));
extern bool_t _EXFUN(xdr_string, (XDR *, char **, u_int));
extern bool_t _EXFUN(xdr_union, (XDR *, enum_t *, char *,
                                 _CONST struct xdr_discrim *, xdrproc_t));
extern bool_t _EXFUN(xdr_char, (XDR *, char *));
extern bool_t _EXFUN(xdr_u_char, (XDR *, u_char *));
extern bool_t _EXFUN(xdr_vector, (XDR *, char *, u_int, u_int, xdrproc_t));
extern bool_t _EXFUN(xdr_float, (XDR *, float *));
extern bool_t _EXFUN(xdr_double, (XDR *, double *));
/* extern bool_t _EXFUN (xdr_quadruple, (XDR *, long double *)); */
extern bool_t _EXFUN(xdr_reference, (XDR *, char **, u_int, xdrproc_t));
extern bool_t _EXFUN(xdr_pointer, (XDR *, char **, u_int, xdrproc_t));
extern bool_t _EXFUN(xdr_wrapstring, (XDR *, char **));
#if defined(___int64_t_defined)
extern bool_t _EXFUN(xdr_hyper, (XDR *, quad_t *));
extern bool_t _EXFUN(xdr_u_hyper, (XDR *, u_quad_t *));
extern bool_t _EXFUN(xdr_longlong_t, (XDR *, quad_t *));
extern bool_t _EXFUN(xdr_u_longlong_t, (XDR *, u_quad_t *));
#endif /* ___int64_t_defined */
extern u_long _EXFUN(xdr_sizeof, (xdrproc_t, void *));

/*
 * Common opaque bytes objects used by many rpc protocols;
 * declared here due to commonality.
 */
#define MAX_NETOBJ_SZ 1024
struct netobj
{
    u_int n_len;
    char *n_bytes;
};
typedef struct netobj netobj;
extern bool_t _EXFUN(xdr_netobj, (XDR *, struct netobj *));

/*
 * These are the public routines for the various implementations of
 * xdr streams.
 */

/* XDR using memory buffers */
extern void _EXFUN(xdrmem_create, (XDR *, char *, u_int, enum xdr_op));

/* XDR using stdio library */
#if defined(_STDIO_H_)
extern void _EXFUN(xdrstdio_create, (XDR *, FILE *, enum xdr_op));
#endif

/* XDR pseudo records for tcp */
extern void _EXFUN(xdrrec_create, (XDR *, u_int, u_int, void *,
                                   int _EXPARM(, (void *, void *, int)),
                                   int _EXPARM(, (void *, void *, int))));

/* make end of xdr record */
extern bool_t _EXFUN(xdrrec_endofrecord, (XDR *, bool_t));

/* move to beginning of next record */
extern bool_t _EXFUN(xdrrec_skiprecord, (XDR *));

/* true if no more input */
extern bool_t _EXFUN(xdrrec_eof, (XDR *));
extern u_int _EXFUN(xdrrec_readbytes, (XDR *, caddr_t, u_int));

/* free memory buffers for xdr */
extern void _EXFUN(xdr_free, (xdrproc_t, void *));

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_XDR_H */
