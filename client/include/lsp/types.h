#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <json/value.h>

#include <std/types.h>
#include <std/strings.h>

/// Request Message types

typedef u32 RequestMessageIdType;
enum {
    RequestMessageIdType_Unknown,
    RequestMessageIdType_Integer,
    RequestMessageIdType_String
};

struct RequestMessageId {
    union {
        i64 number;
        ConstStringU8 str;
    };
    RequestMessageIdType type;
};

struct RequestMessage {
    RequestMessageId id;
    ConstStringU8 method;
    VALUE *params;
};

/// Response Message types

typedef u32 ResponseMessageIdType;
enum {
    ResponseMessageIdType_Unknown,
    ResponseMessageIdType_Null,
    ResponseMessageIdType_Integer,
    ResponseMessageIdType_String
};

struct ResponseMessageId {
    union {
        i64 number;
        ConstStringU8 str;
    };
    ResponseMessageIdType type;
};

typedef i64 ErrorCode;
enum : i64 {
    ErrorCode_ParseError = -32700,
    ErrorCode_InvalidRequest = -32600,
    ErrorCode_MethodNotFound = -32601,
    ErrorCode_InvalidParams = -32602,
    ErrorCode_InternalError = -32603,
    ErrorCode_jsonrpcReservedErrorRangeStart = -32099,
    ErrorCode_serverErrorStart = -32099, // = ErrorCode_jsonrpcReservedErrorRangeStart
    ErrorCode_ServerNotInitialized = -32002,
    ErrorCode_UnknownErrorCode = -32001,
    ErrorCode_jsonrpcReservedErrorRangeEnd = -32000,
    ErrorCode_serverErrorEnd = -32000, // = ErrorCode_jsonrpcReservedErrorRangeEnd
    ErrorCode_lspReservedErrorRangeStart = -32899,
    ErrorCode_ContentModified = -32801,
    ErrorCode_RequestCancelled = -32800,
    ErrorCode_lspReservedErrorRangeEnd = -32800,
};

struct ResponseError {
    ErrorCode code;
    ConstStringU8 message;
    VALUE* data;
};

struct ResponseMessage {
    ResponseMessageId id;
    VALUE *result;
    bool has_response_error;
    ResponseError error;
};

/// Notification Message types

struct NotificationMessage {
    ConstStringU8 method;
    VALUE *params;
};

/// Params types

typedef u8 MessageType;
enum {
    MessageType_Error = 1,
    MessageType_Warning = 2,
    MessageType_Info = 3,
    MessageType_Log = 4
};

struct LogMessageParams {
    MessageType type;
    ConstStringU8 message;
};


ConstStringU8 to_string(MessageType type);

#endif // LSP_TYPES_H
