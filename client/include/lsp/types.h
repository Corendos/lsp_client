#ifndef LSP_TYPES_H
#define LSP_TYPES_H

#include <json/value.h>

#include <reborn/strings.h>
#include <reborn/types.h>

typedef u8 ClientRequestType;
enum {
    ClientRequestType_Unknown,
    ClientRequestType_Initialize,
    ClientRequestType_Shutdown,
    ClientRequestType_WorkspaceSymbolsRequest,
    ClientRequestType_ExecuteCommand,
    ClientRequestType_WillCreateFiles,
    ClientRequestType_WillRenameFiles,
    ClientRequestType_WillDeleteFiles,
    ClientRequestType_WillSaveWaitUntilTextDocument,
    ClientRequestType_Completion,
    ClientRequestType_CompletionItemResolve,
    ClientRequestType_Hover,
    ClientRequestType_SignatureHelp,
    ClientRequestType_GotoDeclaration,
    ClientRequestType_GotoDefinition,
    ClientRequestType_GotoTypeDefinition,
    ClientRequestType_GotoImplementation,
    ClientRequestType_FindReferences,
    ClientRequestType_DocumentHighlights,
    ClientRequestType_DocumentSymbols,
    ClientRequestType_CodeAction,
    ClientRequestType_CodeActionResolve,
    ClientRequestType_CodeLens,
    ClientRequestType_CodeLensResolve,
    ClientRequestType_CodeLensRefresh,
    ClientRequestType_DocumentLink,
    ClientRequestType_DocumentLinkResolve,
    ClientRequestType_DocumentColor,
    ClientRequestType_ColorPresentation,
    ClientRequestType_DocumentFormatting,
    ClientRequestType_DocumentRangeFormatting,
    ClientRequestType_DocumentOnTypeFormatting,
    ClientRequestType_Rename,
    ClientRequestType_PrepareRename,
    ClientRequestType_FoldingRange,
    ClientRequestType_SelectionRange,
    ClientRequestType_PrepareCallHierarchy,
    ClientRequestType_CallHierarchyIncomingCalls,
    ClientRequestType_CallHierarchyOutgoingCalls,
    ClientRequestType_SemanticTokens,
    ClientRequestType_LinkedEditing,
    ClientRequestType_Monikers,
    ClientRequestType_COUNT,
};

typedef u8 ServerNotificationType;
enum {
    ServerNotificationType_Unknown,
    ServerNotificationType_LogTrace,
    ServerNotificationType_ShowMessage,
    ServerNotificationType_LogMessage,
    ServerNotificationType_Telemetry,
    ServerNotificationType_PublishDiagnostics,
    ServerNotificationType_COUNT
};

typedef u8 ServerRequestType;
enum {
    ServerRequestType_Unknown,
    ServerRequestType_ShowMessage,
    ServerRequestType_ShowDocument,
    ServerRequestType_CreatingWorkDoneProgress,
    ServerRequestType_CancelingWorkDoneProgress,
    ServerRequestType_RegisterCapability,
    ServerRequestType_UnregisterCapability,
    ServerRequestType_WorkspaceFolders,
    ServerRequestType_Configuration,
    ServerRequestType_AppliesWorkspaceEdit,
    ServerRequestType_CodeLensRefresh,
    ServerRequestType_COUNT,
};

struct NullableI64 {
    i64 value;
    bool has_value;
};

struct OptionalU64 {
    u64 value;
    bool has_value;
};

struct NullableConstStringU8 {
    ConstStringU8 value;
    bool has_value;
};

struct OptionalConstStringU8 {
    ConstStringU8 value;
    bool has_value;
};

struct OptionalNullableConstStringU8 {
    NullableConstStringU8 value;
    bool has_value;
};

struct TraceValue {
    u8 value;
};

enum {
    TraceValue_Off,
    TraceValue_Messages,
    TraceValue_Verbose,
};

struct OptionalTraceValue {
    TraceValue value;
    bool has_value;
};

static ConstStringU8 trace_value_str[] = {SCu8("off"), SCu8("messages"), SCu8("verbose")};

struct EOLType {
    u8 value;
};

enum {
    EOLType_CR,
    EOLType_CRLF,
    EOLType_LF,
};

static ConstStringU8 eol_type_str[] = {SCu8("\n"), SCu8("\r\n"), SCu8("\r")};

struct Position {
    u64 line;
    u64 character;
};

struct Range {
    Position start;
    Position end;
};

struct OptionalRange {
    Range value;
    bool has_value;
};

struct Location {
    ConstStringU8 uri;
    Range range;
};

struct LocationLink {
    OptionalRange origin_selection_range;
    ConstStringU8 target_uri;
    Range target_range;
    Range target_selection_range;
};

struct DiagnosticSeverity {
    u8 value;
};

enum {
    DiagnosticSeverity_Error       = 1,
    DiagnosticSeverity_Warning     = 2,
    DiagnosticSeverity_Information = 3,
    DiagnosticSeverity_Hint        = 4,
};

struct OptionalDiagnosticSeverity {
    DiagnosticSeverity value;
    bool has_value;
};

struct DiagnosticTag {
    u8 value;
};

enum { DiagnosticTag_Unnecessary = 1, DiagnosticTag_Deprecated = 2 };

struct DiagnosticTagArray {
    DiagnosticTag* values;
    u64 count;
};

struct OptionalDiagnosticTagArray {
    DiagnosticTagArray value;
    bool has_value;
};

struct DiagnosticRelatedInformation {
    Location location;
    ConstStringU8 message;
};

struct OptionalDiagnosticRelatedInformationArray {
    DiagnosticRelatedInformation* values;
    u64 value_count;
    bool has_value;
};

struct CodeDescription {
    ConstStringU8 href;
};

struct OptionalCodeDescription {
    CodeDescription value;
    bool has_value;
};

struct DiagnosticCodeType {
    u8 value;
};

enum { DiagnosticCodeType_Unknown, DiagnosticCodeType_Integer, DiagnosticCodeType_String };

struct Diagnostic {
    Range range;
    OptionalDiagnosticSeverity severity;
    DiagnosticCodeType code_type;
    union {
        i64 number;
        ConstStringU8 str;
    } code;
    OptionalCodeDescription code_description;
    OptionalConstStringU8 source;
    ConstStringU8 message;
    OptionalDiagnosticTagArray tags;
    OptionalDiagnosticRelatedInformationArray related_information;
    /* NOTE(Corentin): data omitted */
};

struct Command {
    ConstStringU8 title;
    ConstStringU8 command;
    /* NOTE(Corentin): arguments omitted */
};

struct TextEdit {
    Range range;
    ConstStringU8 new_text;
};

struct OptionalBoolean {
    bool value;
    bool has_value;
};

struct ChangeAnnotation {
    ConstStringU8 label;
    OptionalBoolean needs_information;
    OptionalConstStringU8 description;
};

struct AnnotatedTextEdit {
    TextEdit text_edit;
    // TODO(Corentin):  add type ?
    ConstStringU8 annotation_id;
};

struct TextDocumentEdit {
    // TODO(Corentin)
};

struct CreateFileOptions {
    OptionalBoolean overwrite;
    OptionalBoolean ignore_if_exists;
};

struct OptionalCreateFileOptions {
    CreateFileOptions value;
    bool has_value;
};

struct CreateFile {
    /* kind: 'create' */
    ConstStringU8 uri;
    OptionalCreateFileOptions options;
    OptionalConstStringU8 annotation_id;
};

struct RenameFileOptions {
    OptionalBoolean overwrite;
    OptionalBoolean ignore_if_exists;
};

struct OptionalRenameFileOptions {
    RenameFileOptions value;
    bool has_value;
};

struct RenameFile {
    /* kind: 'rename' */
    ConstStringU8 old_uri;
    ConstStringU8 new_uri;
    OptionalRenameFileOptions options;
    OptionalConstStringU8 annotation_id;
};

struct DeleteFileOptions {
    OptionalBoolean recursive;
    OptionalBoolean ignore_if_exists;
};

struct OptionalDeleteFileOptions {
    DeleteFileOptions value;
    bool has_value;
};

struct DeleteFile {
    /* kind: 'delete' */
    ConstStringU8 uri;
    OptionalDeleteFileOptions options;
    OptionalConstStringU8 annotation_id;
};

struct TextDocumentIdentifier {
    ConstStringU8 uri;
};

struct TextDocumentItem {
    ConstStringU8 uri;
    ConstStringU8 language_id;
    i64 version;
    ConstStringU8 text;
};

struct DocumentFilter {
    OptionalConstStringU8 language;
    OptionalConstStringU8 scheme;
    OptionalConstStringU8 pattern;
};

struct DocumentSelector {
    DocumentFilter* filters;
    u64 filter_count;
};

/// Request Message types

struct RequestMessageIdType {
    u8 value;
};

enum { RequestMessageIdType_Unknown, RequestMessageIdType_Integer, RequestMessageIdType_String };

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
    VALUE* params;
};

/// Response Message types

struct ResponseMessageIdType {
    u8 value;
};

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

struct ErrorCode {
    i64 value;
};

enum : i64 {
    ErrorCode_ParseError                     = -32700,
    ErrorCode_InvalidRequest                 = -32600,
    ErrorCode_MethodNotFound                 = -32601,
    ErrorCode_InvalidParams                  = -32602,
    ErrorCode_InternalError                  = -32603,
    ErrorCode_jsonrpcReservedErrorRangeStart = -32099,
    ErrorCode_serverErrorStart               = -32099, // = ErrorCode_jsonrpcReservedErrorRangeStart
    ErrorCode_ServerNotInitialized           = -32002,
    ErrorCode_UnknownErrorCode               = -32001,
    ErrorCode_jsonrpcReservedErrorRangeEnd   = -32000,
    ErrorCode_serverErrorEnd                 = -32000, // = ErrorCode_jsonrpcReservedErrorRangeEnd
    ErrorCode_lspReservedErrorRangeStart     = -32899,
    ErrorCode_ContentModified                = -32801,
    ErrorCode_RequestCancelled               = -32800,
    ErrorCode_lspReservedErrorRangeEnd       = -32800,
};

struct ResponseError {
    ErrorCode code;
    ConstStringU8 message;
    VALUE* data;
};

struct OptionalResponseError {
    ResponseError value;
    bool has_value;
};

struct ResponseMessage {
    ResponseMessageId id;
    VALUE* result;
    OptionalResponseError error;
};

/// Notification Message types

struct NotificationMessage {
    ConstStringU8 method;
    VALUE* params;
};

/// Params types

struct MessageType {
    u8 value;
};

enum { MessageType_Error = 1, MessageType_Warning = 2, MessageType_Info = 3, MessageType_Log = 4 };

struct LogMessageParams {
    MessageType type;
    ConstStringU8 message;
};

ConstStringU8 to_string(MessageType type);

struct ClientInfo {
    ConstStringU8 name;
    OptionalConstStringU8 version;
};

struct OptionalClientInfo {
    ClientInfo value;
    bool has_value;
};

struct ResourceOperationKind {
    u8 value;
};

enum {
    ResourceOperationKind_Create,
    ResourceOperationKind_Rename,
    ResourceOperationKind_Delete,
};

struct ResourceOperationKindArray {
    ResourceOperationKind* values;
    u64 count;
};

struct OptionalResourceOperationKindArray {
    ResourceOperationKindArray value;
    bool has_value;
};

static ConstStringU8 resource_operation_kind_str[] = {
    SCu8("create"),
    SCu8("rename"),
    SCu8("delete"),
};

struct FailureHandlingKind {
    u8 value;
};

enum {
    FailureHandlingKind_Abort,
    FailureHandlingKind_Transactional,
    FailureHandlingKind_Undo,
    FailureHandlingKind_TextOnlyTransactional,
};

struct OptionalFailureHandlingKind {
    FailureHandlingKind value;
    bool has_value;
};

static ConstStringU8 failure_handling_kind_str[] = {
    SCu8("abort"),
    SCu8("transactional"),
    SCu8("undo"),
    SCu8("textOnlyTransactional"),
};

struct ChangeAnnotationSupport {
    OptionalBoolean groups_on_label;
};

struct OptionalChangeAnnotationSupport {
    ChangeAnnotationSupport value;
    bool has_value;
};

struct WorkspaceEditClientCapabilities {
    OptionalBoolean document_changes;
    OptionalResourceOperationKindArray resource_operations;
    OptionalFailureHandlingKind failure_handling;
    OptionalBoolean normalize_line_endings;
    OptionalChangeAnnotationSupport change_annotation_support;
};

struct OptionalWorkspaceEditClientCapabilities {
    WorkspaceEditClientCapabilities value;
    bool has_value;
};

struct DidChangeConfigurationClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalDidChangeConfigurationClientCapabilities {
    DidChangeConfigurationClientCapabilities value;
    bool has_value;
};

struct DidChangeWatchedFilesClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalDidChangeWatchedFilesClientCapabilities {
    DidChangeWatchedFilesClientCapabilities value;
    bool has_value;
};

struct SymbolKind {
    u8 value;
};

enum {
    SymbolKind_File          = 1,
    SymbolKind_Module        = 2,
    SymbolKind_Namespace     = 3,
    SymbolKind_Package       = 4,
    SymbolKind_Class         = 5,
    SymbolKind_Method        = 6,
    SymbolKind_Property      = 7,
    SymbolKind_Field         = 8,
    SymbolKind_Constructor   = 9,
    SymbolKind_Enum          = 10,
    SymbolKind_Interface     = 11,
    SymbolKind_Function      = 12,
    SymbolKind_Variable      = 13,
    SymbolKind_Constant      = 14,
    SymbolKind_String        = 15,
    SymbolKind_Number        = 16,
    SymbolKind_Boolean       = 17,
    SymbolKind_Array         = 18,
    SymbolKind_Object        = 19,
    SymbolKind_Key           = 20,
    SymbolKind_Null          = 21,
    SymbolKind_EnumMember    = 22,
    SymbolKind_Struct        = 23,
    SymbolKind_Event         = 24,
    SymbolKind_Operator      = 25,
    SymbolKind_TypeParameter = 26,
};

struct SymbolKindArray {
    SymbolKind* values;
    u64 count;
};

struct OptionalSymbolKindArray {
    SymbolKindArray value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in WorkspaceSymbolClientCapabilities
struct SymbolKindCapabilities {
    OptionalSymbolKindArray value_set;
};

struct OptionalSymbolKindCapabilities {
    SymbolKindCapabilities value;
    bool has_value;
};

struct SymbolTag {
    u8 value;
};

enum { SymbolTag_Deprecated = 1 };

struct SymbolTagArray {
    SymbolTag* values;
    u64 count;
};

// NOTE(Corentin): anonymous struct in WorkspaceSymbolClientCapabilities
struct SymbolTagSupport {
    SymbolTagArray value_set;
};

struct OptionalSymbolTagSupport {
    SymbolTagSupport value;
    bool has_value;
};

struct WorkspaceSymbolClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalSymbolKindCapabilities symbol_kind;
    OptionalSymbolTagSupport tag_support;
};

struct OptionalWorkspaceSymbolClientCapabilities {
    WorkspaceSymbolClientCapabilities value;
    bool has_value;
};

struct ExecuteCommandClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalExecuteCommandClientCapabilities {
    ExecuteCommandClientCapabilities value;
    bool has_value;
};

struct SemanticTokensWorkspaceClientCapabilities {
    OptionalBoolean refresh_support;
};

struct OptionalSemanticTokensWorkspaceClientCapabilities {
    SemanticTokensWorkspaceClientCapabilities value;
    bool has_value;
};

struct CodeLensWorkspaceClientCapabilities {
    OptionalBoolean refresh_support;
};

struct OptionalCodeLensWorkspaceClientCapabilities {
    CodeLensWorkspaceClientCapabilities value;
    bool has_value;
};

struct FileOperations {
    OptionalBoolean dynamic_registration;
    OptionalBoolean did_create;
    OptionalBoolean will_create;
    OptionalBoolean did_rename;
    OptionalBoolean will_rename;
    OptionalBoolean did_delete;
    OptionalBoolean will_delete;
};

struct OptionalFileOperations {
    FileOperations value;
    bool has_value;
};

struct WorkspaceCapabilities {
    OptionalBoolean apply_edit;
    OptionalWorkspaceEditClientCapabilities workspace_edit;
    OptionalDidChangeConfigurationClientCapabilities did_change_configuration;
    OptionalDidChangeWatchedFilesClientCapabilities did_change_watched_files;
    OptionalWorkspaceSymbolClientCapabilities symbol;
    OptionalExecuteCommandClientCapabilities execute_command;
    OptionalBoolean workspace_folders;
    OptionalBoolean configuration;
    OptionalSemanticTokensWorkspaceClientCapabilities semantic_tokens;
    OptionalCodeLensWorkspaceClientCapabilities code_lens;
    OptionalFileOperations file_operations;
};

struct OptionalWorkspaceCapabilities {
    WorkspaceCapabilities value;
    bool has_value;
};

struct TextDocumentSyncClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean will_save;
    OptionalBoolean will_save_wait_until;
    OptionalBoolean did_save;
};

struct OptionalTextDocumentSyncClientCapabilities {
    TextDocumentSyncClientCapabilities value;
    bool has_value;
};

struct MarkupKind {
    u8 value;
};

enum {
    MarkupKind_PlainText,
    MarkupKind_Markdown,
};

static ConstStringU8 markup_kind_str[] = {SCu8("plaintext"), SCu8("markdown")};

struct MarkupKindArray {
    MarkupKind* values;
    u64 count;
};

struct OptionalMarkupKindArray {
    MarkupKindArray value;
    bool has_value;
};

struct CompletionItemTag {
    u8 value;
};

enum {
    CompletionItemTag_Deprecated = 1,
};

struct CompletionItemTagArray {
    CompletionItemTag* values;
    u64 count;
};

struct CompletionTagSupport {
    CompletionItemTagArray value_set;
};

struct OptionalCompletionTagSupport {
    CompletionTagSupport value;
    bool has_value;
};

struct ResolveSupport {
    ConstStringU8Array properties;
};

struct OptionalResolveSupport {
    ResolveSupport value;
    bool has_value;
};

struct InsertTextMode {
    u8 value;
};

enum {
    InsertTextMode_AsIs             = 1,
    InsertTextMode_AdjustIdentation = 2,
};

struct OptionalInsertTextMode {
    InsertTextMode value;
    bool has_value;
};

struct InsertTextModeArray {
    InsertTextMode* values;
    u64 count;
};

struct OptionalInsertTextModeArray {
    InsertTextModeArray value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CompletionItem
struct InsertTextModeSupport {
    OptionalInsertTextModeArray value_set;
};

struct OptionalInsertTextModeSupport {
    InsertTextModeSupport value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CompletionClientCapabilities
struct CompletionItem {
    OptionalBoolean snippet_support;
    OptionalBoolean commit_characters_support;
    OptionalMarkupKindArray documentation_format;
    OptionalBoolean deprecated_support;
    OptionalBoolean preselect_support;
    OptionalCompletionTagSupport tag_support;
    OptionalBoolean insert_replace_support;
    OptionalResolveSupport resolve_support;
    OptionalInsertTextModeSupport insert_text_mode_support;
    OptionalBoolean label_detail_support;
};

struct OptionalCompletionItem {
    CompletionItem value;
    bool has_value;
};

struct CompletionItemKind {
    u8 value;
};

enum {
    CompletionItemKind_Text          = 1,
    CompletionItemKind_Method        = 2,
    CompletionItemKind_Function      = 3,
    CompletionItemKind_Constructor   = 4,
    CompletionItemKind_Field         = 5,
    CompletionItemKind_Variable      = 6,
    CompletionItemKind_Class         = 7,
    CompletionItemKind_Interface     = 8,
    CompletionItemKind_Module        = 9,
    CompletionItemKind_Property      = 10,
    CompletionItemKind_Unit          = 11,
    CompletionItemKind_Value         = 12,
    CompletionItemKind_Enum          = 13,
    CompletionItemKind_Keyword       = 14,
    CompletionItemKind_Snippet       = 15,
    CompletionItemKind_Color         = 16,
    CompletionItemKind_File          = 17,
    CompletionItemKind_Reference     = 18,
    CompletionItemKind_Folder        = 19,
    CompletionItemKind_EnumMember    = 20,
    CompletionItemKind_Constant      = 21,
    CompletionItemKind_Struct        = 22,
    CompletionItemKind_Event         = 23,
    CompletionItemKind_Operator      = 24,
    CompletionItemKind_TypeParameter = 25,
};

struct CompletionItemKindArray {
    CompletionItemKind* values;
    u64 count;
};

struct OptionalCompletionItemKindArray {
    CompletionItemKindArray value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CompletionClientCapabilities
struct CompletionItemKindCapabilities {
    OptionalCompletionItemKindArray value_set;
};

struct OptionalCompletionItemKindCapabilities {
    CompletionItemKindCapabilities value;
    bool has_value;
};

struct OptionalConstStringU8Array {
    ConstStringU8Array value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CompletionClientCapabilities
struct CompletionList {
    OptionalConstStringU8Array item_defaults;
};

struct OptionalCompletionList {
    CompletionList value;
    bool has_value;
};

struct CompletionClientCapabilites {
    OptionalBoolean dynamic_registration;
    OptionalCompletionItem completion_item;
    OptionalCompletionItemKindCapabilities completion_item_kind;
    OptionalBoolean context_support;
    OptionalInsertTextMode insert_text_mode;
    OptionalCompletionList completion_list;
};

struct OptionalCompletionClientCapabilites {
    CompletionClientCapabilites value;
    bool has_value;
};

struct HoverClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalMarkupKindArray content_format;
};

struct OptionalHoverClientCapabilities {
    HoverClientCapabilities value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CompletionClientCapabilities
struct ParameterInformation {
    OptionalBoolean label_offset_support;
};

struct OptionalParameterInformation {
    ParameterInformation value;
    bool has_value;
};

struct SignatureInformation {
    OptionalMarkupKindArray documentation_format;
    OptionalParameterInformation parameter_information;
    OptionalBoolean active_parameter_support;
};

struct OptionalSignatureInformation {
    SignatureInformation value;
    bool has_value;
};

struct SignatureHelpClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalSignatureInformation signature_information;
    OptionalBoolean context_support;
};

struct OptionalSignatureHelpClientCapabilities {
    SignatureHelpClientCapabilities value;
    bool has_value;
};

struct DeclarationClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean link_support;
};

struct OptionalDeclarationClientCapabilities {
    DeclarationClientCapabilities value;
    bool has_value;
};

struct DefinitionClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean link_support;
};

struct OptionalDefinitionClientCapabilities {
    DefinitionClientCapabilities value;
    bool has_value;
};

struct TypeDefinitionClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean link_support;
};

struct OptionalTypeDefinitionClientCapabilities {
    TypeDefinitionClientCapabilities value;
    bool has_value;
};

struct ImplementationClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean link_support;
};

struct OptionalImplementationClientCapabilities {
    ImplementationClientCapabilities value;
    bool has_value;
};

struct ReferenceClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalReferenceClientCapabilities {
    ReferenceClientCapabilities value;
    bool has_value;
};

struct DocumentHighlightClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalDocumentHighlightClientCapabilities {
    DocumentHighlightClientCapabilities value;
    bool has_value;
};

struct DocumentSymbolClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalSymbolKindCapabilities symbol_kind;
    OptionalBoolean hierarchical_document_symbol_support;
    OptionalSymbolTagSupport tag_support;
    OptionalBoolean label_support;
};

struct OptionalDocumentSymbolClientCapabilities {
    DocumentSymbolClientCapabilities value;
    bool has_value;
};

struct CodeActionKind {
    u8 value;
};

enum {
    CodeActionKind_Empty,
    CodeActionKind_QuickFix,
    CodeActionKind_Refactor,
    CodeActionKind_RefactorExtract,
    CodeActionKind_RefactorInline,
    CodeActionKind_RefactorRewrite,
    CodeActionKind_Source,
    CodeActionKind_SourceOrganizeImports,
    CodeActionKind_SourceFixAll,
};

static ConstStringU8 code_action_kind_str[] = {
    SCu8(""),
    SCu8("quickfix"),
    SCu8("refactor"),
    SCu8("refactor.extract"),
    SCu8("refactor.inline"),
    SCu8("refactor.rewrite"),
    SCu8("source"),
    SCu8("source.organizeImports"),
    SCu8("source.fixAll"),
};

struct CodeActionKindArray {
    CodeActionKind* values;
    u64 count;
};

struct OptionalCodeActionKindArray {
    CodeActionKindArray value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CodeActionLiteralSupport
struct CodeActionKindCapabilities {
    OptionalCodeActionKindArray value_set;
};

struct OptionalCodeActionKindCapabilities {
    CodeActionKindCapabilities value;
    bool has_value;
};

// NOTE(Corentin): anonymous struct in CodeActionClientCapabilities
struct CodeActionLiteralSupport {
    OptionalCodeActionKindCapabilities code_action_kind;
};

struct OptionalCodeActionLiteralSupport {
    CodeActionLiteralSupport value;
    bool has_value;
};

struct CodeActionClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalCodeActionLiteralSupport code_action_literal_support;
    OptionalBoolean is_preferred_support;
    OptionalBoolean disabled_support;
    OptionalBoolean data_support;
    OptionalResolveSupport resolve_support;
    OptionalBoolean honors_change_annotations;
};

struct OptionalCodeActionClientCapabilities {
    CodeActionClientCapabilities value;
    bool has_value;
};

struct CodeLensClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalCodeLensClientCapabilities {
    CodeLensClientCapabilities value;
    bool has_value;
};

struct DocumentLinkClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean tooltip_support;
};

struct OptionalDocumentLinkClientCapabilities {
    DocumentLinkClientCapabilities value;
    bool has_value;
};

struct DocumentColorClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalDocumentColorClientCapabilities {
    DocumentColorClientCapabilities value;
    bool has_value;
};

struct DocumentFormattingClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalDocumentFormattingClientCapabilities {
    DocumentFormattingClientCapabilities value;
    bool has_value;
};

struct DocumentRangeFormattingClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalDocumentRangeFormattingClientCapabilities {
    DocumentRangeFormattingClientCapabilities value;
    bool has_value;
};

struct DocumentOnTypeFormattingClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalOnTypeDocumentFormattingClientCapabilities {
    DocumentOnTypeFormattingClientCapabilities value;
    bool has_value;
};

struct PrepareSupportDefaultBehavior {
    u8 value;
};

enum { PrepareSupportDefaultBehavior_Identifier = 1 };

struct OptionalPrepareSupportDefaultBehavior {
    PrepareSupportDefaultBehavior value;
    bool has_value;
};

struct RenameClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalBoolean prepare_support;
    OptionalPrepareSupportDefaultBehavior prepare_support_default_behavior;
    OptionalBoolean honors_change_annotations;
};

struct OptionalRenameClientCapabilities {
    RenameClientCapabilities value;
    bool has_value;
};

struct DiagnosticTagSupport {
    DiagnosticTagArray value_set;
};

struct OptionalDiagnosticTagSupport {
    DiagnosticTagSupport value;
    bool has_value;
};

struct PublishDiagnosticsClientCapabilities {
    OptionalBoolean related_information;
    OptionalDiagnosticTagSupport tag_support;
    OptionalBoolean version_support;
    OptionalBoolean code_description_support;
    OptionalBoolean data_support;
};

struct OptionalPublishDiagnosticsClientCapabilities {
    PublishDiagnosticsClientCapabilities value;
    bool has_value;
};

struct FoldingRangeClientCapabilities {
    OptionalBoolean dynamic_registration;
    OptionalU64 range_limit;
    OptionalBoolean line_folding_only;
};

struct OptionalFoldingRangeClientCapabilities {
    FoldingRangeClientCapabilities value;
    bool has_value;
};

struct SelectionRangeClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalSelectionRangeClientCapabilities {
    SelectionRangeClientCapabilities value;
    bool has_value;
};

struct LinkedEditingRangeClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalLinkedEditingRangeClientCapabilities {
    LinkedEditingRangeClientCapabilities value;
    bool has_value;
};

struct CallHierarchyClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalCallHierarchyClientCapabilities {
    CallHierarchyClientCapabilities value;
    bool has_value;
};

struct RequestRangeClientCapabilitiesType {
    u8 value;
};

enum {
    RequestRangeClientCapabilitiesType_Boolean,
    RequestRangeClientCapabilitiesType_Dict,
};

struct RequestRangeDictClientCapabilities {};

struct RequestRangeClientCapabilities {
    RequestRangeClientCapabilitiesType type;
    union {
        bool boolean;
        RequestRangeDictClientCapabilities dict;
    };
};

struct OptionalRequestRangeClientCapabilities {
    RequestRangeClientCapabilities value;
    bool has_value;
};

struct RequestFullClientCapabilitiesType {
    u8 value;
};

enum {
    RequestFullClientCapabilitiesType_Boolean,
    RequestFullClientCapabilitiesType_Dict,
};

struct RequestFullDictClientCapabilities {
    OptionalBoolean delta;
};

struct RequestFullClientCapabilities {
    RequestFullClientCapabilitiesType type;
    union {
        bool boolean;
        RequestFullDictClientCapabilities dict;
    };
};

struct OptionalRequestFullClientCapabilities {
    RequestFullClientCapabilities value;
    bool has_value;
};

struct RequestClientCapabilities {
    OptionalRequestRangeClientCapabilities range;
    OptionalRequestFullClientCapabilities full;
};

struct TokenFormat {
    u8 value;
};

enum {
    TokenFormat_Relative,
};

static ConstStringU8 token_format_str[] = {SCu8("relative")};

struct TokenFormatArray {
    TokenFormat* values;
    u64 count;
};

struct SemanticTokensClientCapabilities {
    OptionalBoolean dynamic_registration;
    RequestClientCapabilities requests;
    ConstStringU8Array token_types;
    ConstStringU8Array token_modifiers;
    TokenFormatArray formats;
    OptionalBoolean overlapping_token_support;
    OptionalBoolean multiline_token_support;
    OptionalBoolean server_cancel_support;
    OptionalBoolean augment_syntax_support;
};

struct OptionalSemanticTokensClientCapabilities {
    SemanticTokensClientCapabilities value;
    bool has_value;
};

struct MonikerClientCapabilities {
    OptionalBoolean dynamic_registration;
};

struct OptionalMonikerClientCapabilities {
    MonikerClientCapabilities value;
    bool has_value;
};

struct TextDocumentClientCapabilities {
    OptionalTextDocumentSyncClientCapabilities synchronization;
    OptionalCompletionClientCapabilites completion;
    OptionalHoverClientCapabilities hover;
    OptionalSignatureHelpClientCapabilities signature_help;
    OptionalDeclarationClientCapabilities declaration;
    OptionalDefinitionClientCapabilities definition;
    OptionalTypeDefinitionClientCapabilities type_definition;
    OptionalImplementationClientCapabilities implementation;
    OptionalReferenceClientCapabilities references;
    OptionalDocumentHighlightClientCapabilities document_highlight;
    OptionalDocumentSymbolClientCapabilities document_symbol;
    OptionalCodeActionClientCapabilities code_action;
    OptionalCodeLensClientCapabilities code_lens;
    OptionalDocumentLinkClientCapabilities document_link;
    OptionalDocumentColorClientCapabilities color_provider;
    OptionalDocumentFormattingClientCapabilities formatting;
    OptionalDocumentRangeFormattingClientCapabilities range_formatting;
    OptionalOnTypeDocumentFormattingClientCapabilities on_type_formatting;
    OptionalRenameClientCapabilities rename;
    OptionalPublishDiagnosticsClientCapabilities publish_diagnostics;
    OptionalFoldingRangeClientCapabilities folding_range;
    OptionalSelectionRangeClientCapabilities selection_range;
    OptionalLinkedEditingRangeClientCapabilities linked_editing_range;
    OptionalCallHierarchyClientCapabilities call_hierarchy;
    OptionalSemanticTokensClientCapabilities semantic_tokens;
    OptionalMonikerClientCapabilities moniker;
};

struct OptionalTextDocumentClientCapabilities {
    TextDocumentClientCapabilities value;
    bool has_value;
};

struct MessageActionItem {
    OptionalBoolean additional_properties_support;
};

struct OptionalMessageActionItem {
    MessageActionItem value;
    bool has_value;
};

struct ShowMessageRequestClientCapabilities {
    OptionalMessageActionItem message_action_item;
};

struct OptionalShowMessageRequestClientCapabilities {
    ShowMessageRequestClientCapabilities value;
    bool has_value;
};

struct ShowDocumentClientCapabilities {
    bool support;
};

struct OptionalShowDocumentClientCapabilities {
    ShowDocumentClientCapabilities value;
    bool has_value;
};

struct WindowCapabilities {
    OptionalBoolean work_done_progress;
    OptionalShowMessageRequestClientCapabilities show_message;
    OptionalShowDocumentClientCapabilities show_document;
};

struct OptionalWindowCapabilities {
    WindowCapabilities value;
    bool has_value;
};

struct RegularExpressionClientCapabilities {
    ConstStringU8 engine;
    OptionalConstStringU8 version;
};

struct OptionalRegularExpressionClientCapabilities {
    RegularExpressionClientCapabilities value;
    bool has_value;
};

struct MarkdownClientCapabilities {
    ConstStringU8 parser;
    OptionalConstStringU8 version;
    OptionalConstStringU8Array allowed_tags;
};

struct OptionalMarkdownClientCapabilities {
    MarkdownClientCapabilities value;
    bool has_value;
};

struct GeneralCapabilities {
    OptionalRegularExpressionClientCapabilities regular_expressions;
    OptionalMarkdownClientCapabilities markdown;
};

struct OptionalGeneralCapabilities {
    GeneralCapabilities value;
    bool has_value;
};

struct ClientCapabilities {
    OptionalWorkspaceCapabilities workspace;
    OptionalTextDocumentClientCapabilities text_document;
    OptionalWindowCapabilities window;
    OptionalGeneralCapabilities general;
};

struct WorkspaceFolder {
    ConstStringU8 uri;
    ConstStringU8 name;
};

struct WorkspaceFolderArray {
    WorkspaceFolder* folders;
    u64 folder_count;
};

struct NullableWorkspaceFolderArray {
    WorkspaceFolderArray value;
    bool has_value;
};

struct OptionalNullableWorkspaceFolderArray {
    NullableWorkspaceFolderArray value;
    bool has_value;
};

struct InitializeParams {
    NullableI64 process_id;
    OptionalClientInfo client_info;
    OptionalConstStringU8 locale;
    OptionalNullableConstStringU8 root_path;
    NullableConstStringU8 root_uri;
    /* NOTE(Corentin): initializationOptions omitted */
    ClientCapabilities capabilities;
    OptionalTraceValue trace;
    OptionalNullableWorkspaceFolderArray workspace_folders;
};

VALUE to_json(NullableI64* value);
VALUE to_json(ClientInfo* value);
VALUE to_json(ConstStringU8* value);
VALUE to_json_array(ConstStringU8* values, u64 count);
VALUE to_json(ConstStringU8Array* value);
VALUE to_json(NullableConstStringU8* value);
VALUE to_json(ClientCapabilities* params);
VALUE to_json(TraceValue* value);
VALUE to_json(WorkspaceFolder* folder);
VALUE to_json_array(WorkspaceFolder* folders, u64 count);
VALUE to_json(WorkspaceFolderArray* value);
VALUE to_json(NullableWorkspaceFolderArray* value);
VALUE to_json(InitializeParams* params);

VALUE to_json(ResourceOperationKind* value);
VALUE to_json_array(ResourceOperationKind* value, u64 count);
VALUE to_json_array(ResourceOperationKindArray* value);
VALUE to_json(FailureHandlingKind* value);
VALUE to_json(ChangeAnnotationSupport* value);
VALUE to_json(WorkspaceEditClientCapabilities* value);
VALUE to_json(DidChangeConfigurationClientCapabilities* value);
VALUE to_json(DidChangeWatchedFilesClientCapabilities* value);
VALUE to_json(SymbolKind* value);
VALUE to_json_array(SymbolKindArray* value, u64 count);
VALUE to_json(SymbolKindCapabilities* value);
VALUE to_json(SymbolTag* value);
VALUE to_json_array(SymbolTagArray* value, u64 count);
VALUE to_json(SymbolTagSupport* value);
VALUE to_json(WorkspaceSymbolClientCapabilities* value);
VALUE to_json(ExecuteCommandClientCapabilities* value);
VALUE to_json(SemanticTokensWorkspaceClientCapabilities* value);
VALUE to_json(CodeLensWorkspaceClientCapabilities* value);
VALUE to_json(FileOperations* value);
VALUE to_json(WorkspaceCapabilities* value);
VALUE to_json(TextDocumentSyncClientCapabilities* value);
VALUE to_json(MarkupKind* value);
VALUE to_json_array(MarkupKindArray* value, u64 count);
VALUE to_json(CompletionItemTag* value);
VALUE to_json_array(CompletionItemTagArray* value, u64 count);
VALUE to_json(CompletionTagSupport* value);
VALUE to_json(ResolveSupport* value);
VALUE to_json(InsertTextMode* value);
VALUE to_json_array(InsertTextModeArray* value, u64 count);
VALUE to_json(InsertTextModeSupport* value);
VALUE to_json(CompletionItem* value);
VALUE to_json(CompletionItemKind* value);
VALUE to_json_array(CompletionItemKindArray* value, u64 count);
VALUE to_json(CompletionItemKindCapabilities* value);
VALUE to_json(CompletionList* value);
VALUE to_json(CompletionClientCapabilites* value);
VALUE to_json(HoverClientCapabilities* value);
VALUE to_json(ParameterInformation* value);
VALUE to_json(SignatureInformation* value);
VALUE to_json(SignatureHelpClientCapabilities* value);
VALUE to_json(DeclarationClientCapabilities* value);
VALUE to_json(DefinitionClientCapabilities* value);
VALUE to_json(TypeDefinitionClientCapabilities* value);
VALUE to_json(ImplementationClientCapabilities* value);
VALUE to_json(ReferenceClientCapabilities* value);
VALUE to_json(DocumentHighlightClientCapabilities* value);
VALUE to_json(DocumentSymbolClientCapabilities* value);
VALUE to_json(CodeActionKind* value);
VALUE to_json_array(CodeActionKindArray* value, u64 count);
VALUE to_json(CodeActionKindCapabilities* value);
VALUE to_json(CodeActionLiteralSupport* value);
VALUE to_json(CodeActionClientCapabilities* value);
VALUE to_json(CodeLensClientCapabilities* value);
VALUE to_json(DocumentLinkClientCapabilities* value);
VALUE to_json(DocumentColorClientCapabilities* value);
VALUE to_json(DocumentFormattingClientCapabilities* value);
VALUE to_json(DocumentRangeFormattingClientCapabilities* value);
VALUE to_json(DocumentOnTypeFormattingClientCapabilities* value);
VALUE to_json(PrepareSupportDefaultBehavior* value);
VALUE to_json(RenameClientCapabilities* value);
VALUE to_json(PublishDiagnosticsClientCapabilities* value);
VALUE to_json(FoldingRangeClientCapabilities* value);
VALUE to_json(SelectionRangeClientCapabilities* value);
VALUE to_json(LinkedEditingRangeClientCapabilities* value);
VALUE to_json(CallHierarchyClientCapabilities* value);
VALUE to_json(RequestRangeClientCapabilitiesType* value);
VALUE to_json(RequestRangeDictClientCapabilities* value);
VALUE to_json(RequestRangeClientCapabilities* value);
VALUE to_json(RequestFullClientCapabilitiesType* value);
VALUE to_json(RequestFullDictClientCapabilities* value);
VALUE to_json(RequestFullClientCapabilities* value);
VALUE to_json(RequestClientCapabilities* value);
VALUE to_json(TokenFormat* value);
VALUE to_json_array(TokenFormatArray* value, u64 count);
VALUE to_json(SemanticTokensClientCapabilities* value);
VALUE to_json(MonikerClientCapabilities* value);
VALUE to_json(TextDocumentClientCapabilities* value);
VALUE to_json(MessageActionItem* value);
VALUE to_json(ShowMessageRequestClientCapabilities* value);
VALUE to_json(ShowDocumentClientCapabilities* value);
VALUE to_json(WindowCapabilities* value);
VALUE to_json(RegularExpressionClientCapabilities* value);
VALUE to_json(MarkdownClientCapabilities* value);
VALUE to_json(GeneralCapabilities* value);

#endif // LSP_TYPES_H
