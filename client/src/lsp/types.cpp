#include <lsp/types.h>

ConstStringU8 to_string(MessageType type) {
    ConstStringU8 result = SCu8("[null]");
    
    switch (type.value) {
        case MessageType_Error:
        result = SCu8("["
                      "\x1b\x5b"
                      "31m"
                      "Error"
                      "\x1b\x5b"
                      "0m"
                      "]");
        break;
        case MessageType_Warning:
        result = SCu8("["
                      "\x1b\x5b"
                      "33m"
                      "Warning"
                      "\x1b\x5b"
                      "0m"
                      "]");
        break;
        case MessageType_Info:
        result = SCu8("["
                      "\x1b\x5b"
                      "32m"
                      "Info"
                      "\x1b\x5b"
                      "0m"
                      "]");
        break;
        case MessageType_Log:
        result = SCu8("["
                      "\x1b\x5b"
                      "34m"
                      "Log"
                      "\x1b\x5b"
                      "0m"
                      "]");
        break;
    }
    
    return result;
}

VALUE to_json(NullableI64* value) {
    VALUE json = {0};
    
    if (value->has_value) {
        value_init_int64(&json, value->value);
    } else {
        value_init_null(&json);
    }
    
    return json;
}

VALUE to_json(ClientInfo* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* name_json = value_dict_add(&json, "name");
    *name_json       = to_json(&value->name);
    
    if (value->version.has_value) {
        VALUE* version_json = value_dict_add(&json, "version");
        *version_json       = to_json(&value->version.value);
    }
    
    return json;
}

VALUE to_json(ConstStringU8* value) {
    VALUE json = {0};
    
    value_init_string_(&json, value->data, value->size);
    
    return json;
}

VALUE to_json_array(ConstStringU8* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        ConstStringU8* current = values + i;
        VALUE* new_item        = value_array_append(&json);
        *new_item              = to_json(current);
    }
    
    return json;
}

VALUE to_json(ConstStringU8Array* value) { return to_json_array(value->values, value->count); }

VALUE to_json(NullableConstStringU8* value) {
    VALUE json = {0};
    
    if (value->has_value) {
        json = to_json(&value->value);
    } else {
        value_init_null(&json);
    }
    
    return json;
}

VALUE to_json(ClientCapabilities* params) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    return json;
}

VALUE to_json(TraceValue* value) {
    ConstStringU8* str = trace_value_str + (u8)value;
    
    return to_json(str);
}

VALUE to_json(WorkspaceFolder* folder) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* uri_json = value_dict_add(&json, "uri");
    *uri_json       = to_json(&folder->uri);
    
    VALUE* name_json = value_dict_add(&json, "name");
    *name_json       = to_json(&folder->name);
    
    return json;
}

VALUE to_json_array(WorkspaceFolder* folders, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        WorkspaceFolder* current = folders + i;
        VALUE* new_item          = value_array_append(&json);
        *new_item                = to_json(current);
    }
    
    return json;
}

VALUE to_json(WorkspaceFolderArray* value) { return to_json_array(value->folders, value->folder_count); }

VALUE to_json(NullableWorkspaceFolderArray* value) {
    VALUE json = {0};
    
    if (value->has_value) {
        json = to_json(&value->value);
    } else {
        value_init_null(&json);
    }
    
    return json;
}

VALUE to_json(InitializeParams* params) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "processId");
    *current       = to_json(&params->process_id);
    
    if (params->client_info.has_value) {
        current  = value_dict_add(&json, "clientInfo");
        *current = to_json(&params->client_info.value);
    }
    
    if (params->locale.has_value) {
        current  = value_dict_add(&json, "locale");
        *current = to_json(&params->locale.value);
    }
    
    if (params->root_path.has_value) {
        current  = value_dict_add(&json, "rootPath");
        *current = to_json(&params->root_path.value);
    }
    
    current  = value_dict_add(&json, "rootUri");
    *current = to_json(&params->root_uri);
    
    current  = value_dict_add(&json, "capabilities");
    *current = to_json(&params->capabilities);
    
    if (params->trace.has_value) {
        current  = value_dict_add(&json, "trace");
        *current = to_json(&params->trace.value);
    }
    
    if (params->workspace_folders.has_value) {
        current  = value_dict_add(&json, "workspaceFolders");
        *current = to_json(&params->workspace_folders.value);
    }
    
    return json;
}

VALUE to_json(ResourceOperationKind* value) {
    ConstStringU8* str = resource_operation_kind_str + (u8)value;
    return to_json(str);
}

VALUE to_json_array(ResourceOperationKind* value, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        ResourceOperationKind* current = value + i;
        VALUE* new_item                = value_array_append(&json);
        *new_item                      = to_json(current);
    }
    
    return json;
}

VALUE to_json(ResourceOperationKindArray* value) {
    VALUE json = {0};
    
    json = to_json_array(value->values, value->count);
    
    return json;
}

VALUE to_json(FailureHandlingKind* value) {
    ConstStringU8* str = failure_handling_kind_str + (u8)value;
    return to_json(str);
}

VALUE to_json(ChangeAnnotationSupport* value) {
    VALUE json = {0};
    
    if (value->groups_on_label.has_value) {
        value_init_bool(&json, value->groups_on_label.value);
    }
    
    return json;
}

VALUE to_json(WorkspaceEditClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->document_changes.has_value) {
        current = value_dict_add(&json, "documentChanges");
        value_init_bool(current, value->document_changes.value);
    }
    
    if (value->resource_operations.has_value) {
        current  = value_dict_add(&json, "resourceOperations");
        *current = to_json(&value->resource_operations.value);
    }
    
    if (value->failure_handling.has_value) {
        current  = value_dict_add(&json, "failureHandling");
        *current = to_json(&value->failure_handling.value);
    }
    
    if (value->normalize_line_endings.has_value) {
        current = value_dict_add(&json, "normalizeLineEndings");
        value_init_bool(current, value->normalize_line_endings.value);
    }
    
    if (value->change_annotation_support.has_value) {
        current  = value_dict_add(&json, "changeAnnotationSupport");
        *current = to_json(&value->change_annotation_support.value);
    }
    
    return json;
}

VALUE to_json(DidChangeConfigurationClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DidChangeWatchedFilesClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(SymbolKind* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json_array(SymbolKind* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        SymbolKind* current = values + i;
        VALUE* new_item     = value_array_append(&json);
        *new_item           = to_json(current);
    }
    
    return json;
}

VALUE to_json(SymbolKindArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(SymbolKindCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->value_set.has_value) {
        current  = value_dict_add(&json, "valueSet");
        *current = to_json(&value->value_set.value);
    }
    
    return json;
}

VALUE to_json(SymbolTag* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json_array(SymbolTag* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        SymbolTag* current = values + i;
        VALUE* new_item    = value_array_append(&json);
        *new_item          = to_json(current);
    }
    
    return json;
}

VALUE to_json(SymbolTagArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(SymbolTagSupport* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "valueSet");
    *current       = to_json(&value->value_set);
    
    return json;
}

VALUE to_json(WorkspaceSymbolClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(ExecuteCommandClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(SemanticTokensWorkspaceClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->refresh_support.has_value) {
        current = value_dict_add(&json, "refreshSupport");
        value_init_bool(current, value->refresh_support.value);
    }
    
    return json;
}

VALUE to_json(CodeLensWorkspaceClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->refresh_support.has_value) {
        current = value_dict_add(&json, "refreshSupport");
        value_init_bool(current, value->refresh_support.value);
    }
    
    return json;
}

VALUE to_json(FileOperations* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->did_create.has_value) {
        current = value_dict_add(&json, "didCreate");
        value_init_bool(current, value->did_create.value);
    }
    
    if (value->will_create.has_value) {
        current = value_dict_add(&json, "willCreate");
        value_init_bool(current, value->will_create.value);
    }
    
    if (value->did_rename.has_value) {
        current = value_dict_add(&json, "didRename");
        value_init_bool(current, value->did_rename.value);
    }
    
    if (value->will_rename.has_value) {
        current = value_dict_add(&json, "willRename");
        value_init_bool(current, value->will_rename.value);
    }
    
    if (value->did_delete.has_value) {
        current = value_dict_add(&json, "didDelete");
        value_init_bool(current, value->did_delete.value);
    }
    
    if (value->will_delete.has_value) {
        current = value_dict_add(&json, "willDelete");
        value_init_bool(current, value->will_delete.value);
    }
    
    return json;
}

VALUE to_json(WorkspaceCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->apply_edit.has_value) {
        current = value_dict_add(&json, "applyEdit");
        value_init_bool(current, value->apply_edit.value);
    }
    
    if (value->workspace_edit.has_value) {
        current  = value_dict_add(&json, "workspaceEdit");
        *current = to_json(&value->workspace_edit.value);
    }
    
    if (value->did_change_configuration.has_value) {
        current  = value_dict_add(&json, "didChangeConfiguration");
        *current = to_json(&value->did_change_configuration.value);
    }
    
    if (value->did_change_configuration.has_value) {
        current  = value_dict_add(&json, "didChangeWatchedFiles");
        *current = to_json(&value->did_change_watched_files.value);
    }
    
    if (value->symbol.has_value) {
        current  = value_dict_add(&json, "symbol");
        *current = to_json(&value->symbol.value);
    }
    
    if (value->execute_command.has_value) {
        current  = value_dict_add(&json, "executeCommand");
        *current = to_json(&value->execute_command.value);
    }
    
    if (value->workspace_folders.has_value) {
        current = value_dict_add(&json, "workspaceFolders");
        value_init_bool(current, value->workspace_folders.value);
    }
    
    if (value->configuration.has_value) {
        current = value_dict_add(&json, "configuration");
        value_init_bool(current, value->configuration.value);
    }
    
    if (value->semantic_tokens.has_value) {
        current  = value_dict_add(&json, "semanticTokens");
        *current = to_json(&value->semantic_tokens.value);
    }
    
    if (value->code_lens.has_value) {
        current  = value_dict_add(&json, "codeLens");
        *current = to_json(&value->code_lens.value);
    }
    
    if (value->file_operations.has_value) {
        current  = value_dict_add(&json, "fileOperations");
        *current = to_json(&value->file_operations.value);
    }
    
    return json;
}

VALUE to_json(TextDocumentSyncClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->will_save.has_value) {
        current = value_dict_add(&json, "willSave");
        value_init_bool(current, value->will_save.value);
    }
    
    if (value->will_save_wait_until.has_value) {
        current = value_dict_add(&json, "willSaveWaitUntil");
        value_init_bool(current, value->will_save_wait_until.value);
    }
    
    if (value->did_save.has_value) {
        current = value_dict_add(&json, "didSave");
        value_init_bool(current, value->did_save.value);
    }
    
    return json;
}

VALUE to_json(MarkupKind* value) {
    ConstStringU8* str = markup_kind_str + value->value;
    return to_json(str);
}

VALUE to_json_array(MarkupKind* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        MarkupKind* current = values + i;
        VALUE* new_item     = value_array_append(&json);
        *new_item           = to_json(current);
    }
    
    return json;
}

VALUE to_json(MarkupKindArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(CompletionItemTag* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json_array(CompletionItemTag* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        CompletionItemTag* current = values + i;
        VALUE* new_item            = value_array_append(&json);
        *new_item                  = to_json(current);
    }
    
    return json;
}

VALUE to_json(CompletionItemTagArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(CompletionTagSupport* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "valueSet");
    *current       = to_json(&value->value_set);
    
    return json;
}

VALUE to_json(ResolveSupport* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "properties");
    *current       = to_json(&value->properties);
    
    return json;
}

VALUE to_json(InsertTextMode* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json_array(InsertTextMode* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        InsertTextMode* current = values + i;
        VALUE* new_item         = value_array_append(&json);
        *new_item               = to_json(current);
    }
    
    return json;
}

VALUE to_json(InsertTextModeArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(InsertTextModeSupport* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    if (value->value_set.has_value) {
        VALUE* current = value_dict_add(&json, "valueSet");
        *current       = to_json(&value->value_set.value);
    }
    
    return json;
}

VALUE to_json(CompletionItem* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->snippet_support.has_value) {
        current = value_dict_add(&json, "snippetSupport");
        value_init_bool(current, value->snippet_support.value);
    }
    
    if (value->commit_characters_support.has_value) {
        current = value_dict_add(&json, "commitCharactersSupport");
        value_init_bool(current, value->commit_characters_support.value);
    }
    
    if (value->documentation_format.has_value) {
        current  = value_dict_add(&json, "documentationFormat");
        *current = to_json(&value->documentation_format.value);
    }
    
    if (value->deprecated_support.has_value) {
        current = value_dict_add(&json, "deprecatedSupport");
        value_init_bool(current, value->deprecated_support.value);
    }
    
    if (value->preselect_support.has_value) {
        current = value_dict_add(&json, "preselectSupport");
        value_init_bool(current, value->preselect_support.value);
    }
    
    if (value->tag_support.has_value) {
        current  = value_dict_add(&json, "tagSupport");
        *current = to_json(&value->tag_support.value);
    }
    
    if (value->insert_replace_support.has_value) {
        current = value_dict_add(&json, "insertReplaceSupport");
        value_init_bool(current, value->insert_replace_support.value);
    }
    
    if (value->resolve_support.has_value) {
        current  = value_dict_add(&json, "resolveSupport");
        *current = to_json(&value->resolve_support.value);
    }
    
    if (value->insert_text_mode_support.has_value) {
        current  = value_dict_add(&json, "insertTextModeSupport");
        *current = to_json(&value->insert_text_mode_support.value);
    }
    
    if (value->label_detail_support.has_value) {
        current = value_dict_add(&json, "labelDetailSupport");
        value_init_bool(current, value->label_detail_support.value);
    }
    
    return json;
}

VALUE to_json(CompletionItemKind* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json_array(CompletionItemKind* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        CompletionItemKind* current = values + i;
        VALUE* new_item             = value_array_append(&json);
        *new_item                   = to_json(current);
    }
    
    return json;
}

VALUE to_json(CompletionItemKindArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(CompletionItemKindCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    if (value->value_set.has_value) {
        VALUE* current = value_dict_add(&json, "valueSet");
        *current       = to_json(&value->value_set.value);
    }
    
    return json;
}

VALUE to_json(CompletionList* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    if (value->item_defaults.has_value) {
        VALUE* current = value_dict_add(&json, "itemDefaults");
        *current       = to_json(&value->item_defaults.value);
    }
    
    return json;
}

VALUE to_json(CompletionClientCapabilites* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->completion_item.has_value) {
        current  = value_dict_add(&json, "completionItem");
        *current = to_json(&value->completion_item.value);
    }
    
    if (value->completion_item_kind.has_value) {
        current  = value_dict_add(&json, "completionItemKind");
        *current = to_json(&value->completion_item_kind.value);
    }
    
    if (value->context_support.has_value) {
        current = value_dict_add(&json, "contextSupport");
        value_init_bool(current, value->context_support.value);
    }
    
    if (value->insert_text_mode.has_value) {
        current  = value_dict_add(&json, "insertTextMode");
        *current = to_json(&value->insert_text_mode.value);
    }
    
    if (value->completion_list.has_value) {
        current  = value_dict_add(&json, "completionList");
        *current = to_json(&value->completion_list.value);
    }
    
    return json;
}

VALUE to_json(HoverClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->content_format.has_value) {
        current  = value_dict_add(&json, "contentFormat");
        *current = to_json(&value->content_format.value);
    }
    
    return json;
}

VALUE to_json(ParameterInformation* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->label_offset_support.has_value) {
        current = value_dict_add(&json, "labelOffsetSupport");
        value_init_bool(current, value->label_offset_support.value);
    }
    
    return json;
}

VALUE to_json(SignatureInformation* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->documentation_format.has_value) {
        current  = value_dict_add(&json, "documentationFormat");
        *current = to_json(&value->documentation_format.value);
    }
    
    if (value->parameter_information.has_value) {
        current  = value_dict_add(&json, "parameterInformation");
        *current = to_json(&value->parameter_information.value);
    }
    
    if (value->active_parameter_support.has_value) {
        current = value_dict_add(&json, "activeParameterSupport");
        value_init_bool(current, value->active_parameter_support.value);
    }
    
    return json;
}

VALUE to_json(SignatureHelpClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->signature_information.has_value) {
        current  = value_dict_add(&json, "signatureInformation");
        *current = to_json(&value->signature_information.value);
    }
    
    if (value->context_support.has_value) {
        current = value_dict_add(&json, "contextSupport");
        value_init_bool(current, value->context_support.value);
    }
    
    return json;
}

VALUE to_json(DeclarationClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->link_support.has_value) {
        current = value_dict_add(&json, "linkSupport");
        value_init_bool(current, value->link_support.value);
    }
    
    return json;
}

VALUE to_json(DefinitionClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->link_support.has_value) {
        current = value_dict_add(&json, "linkSupport");
        value_init_bool(current, value->link_support.value);
    }
    
    return json;
}

VALUE to_json(TypeDefinitionClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->link_support.has_value) {
        current = value_dict_add(&json, "linkSupport");
        value_init_bool(current, value->link_support.value);
    }
    
    return json;
}

VALUE to_json(ImplementationClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->link_support.has_value) {
        current = value_dict_add(&json, "linkSupport");
        value_init_bool(current, value->link_support.value);
    }
    
    return json;
}

VALUE to_json(ReferenceClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DocumentHighlightClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DocumentSymbolClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->symbol_kind.has_value) {
        current  = value_dict_add(&json, "symbolKind");
        *current = to_json(&value->symbol_kind.value);
    }
    
    if (value->hierarchical_document_symbol_support.has_value) {
        current = value_dict_add(&json, "hierarchicalDocumentSymbolSupport");
        value_init_bool(current, value->hierarchical_document_symbol_support.value);
    }
    
    if (value->tag_support.has_value) {
        current  = value_dict_add(&json, "tag_support");
        *current = to_json(&value->tag_support.value);
    }
    
    if (value->label_support.has_value) {
        current = value_dict_add(&json, "labelSupport");
        value_init_bool(current, value->label_support.value);
    }
    
    return json;
}

VALUE to_json(CodeActionKind* value) {
    ConstStringU8* str = code_action_kind_str + value->value;
    return to_json(str);
}

VALUE to_json_array(CodeActionKind* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        CodeActionKind* current = values + i;
        VALUE* new_item         = value_array_append(&json);
        *new_item               = to_json(current);
    }
    
    return json;
}

VALUE to_json(CodeActionKindArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(CodeActionKindCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->value_set.has_value) {
        current  = value_dict_add(&json, "valueSet");
        *current = to_json(&value->value_set.value);
    }
    
    return json;
}

VALUE to_json(CodeActionLiteralSupport* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->code_action_kind.has_value) {
        current  = value_dict_add(&json, "codeActionKind");
        *current = to_json(&value->code_action_kind.value);
    }
    
    return json;
}

VALUE to_json(CodeActionClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->code_action_literal_support.has_value) {
        VALUE* current = value_dict_add(&json, "codeActionLiteralSupport");
        *current       = to_json(&value->code_action_literal_support.value);
    }
    
    if (value->is_preferred_support.has_value) {
        current = value_dict_add(&json, "isPreferredSupport");
        value_init_bool(current, value->is_preferred_support.value);
    }
    
    if (value->disabled_support.has_value) {
        current = value_dict_add(&json, "disabledSupport");
        value_init_bool(current, value->disabled_support.value);
    }
    
    if (value->data_support.has_value) {
        current = value_dict_add(&json, "dataSupport");
        value_init_bool(current, value->data_support.value);
    }
    
    if (value->resolve_support.has_value) {
        VALUE* current = value_dict_add(&json, "resolveSupport");
        *current       = to_json(&value->resolve_support.value);
    }
    
    if (value->honors_change_annotations.has_value) {
        current = value_dict_add(&json, "honorsChangeAnnotations");
        value_init_bool(current, value->honors_change_annotations.value);
    }
    
    return json;
}

VALUE to_json(CodeLensClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DocumentLinkClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->tooltip_support.has_value) {
        current = value_dict_add(&json, "tooltipSupport");
        value_init_bool(current, value->tooltip_support.value);
    }
    
    return json;
}

VALUE to_json(DocumentColorClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DocumentFormattingClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DocumentRangeFormattingClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(DocumentOnTypeFormattingClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(PrepareSupportDefaultBehavior* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json(RenameClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->prepare_support.has_value) {
        current = value_dict_add(&json, "prepareSupport");
        value_init_bool(current, value->prepare_support.value);
    }
    
    if (value->prepare_support_default_behavior.has_value) {
        VALUE* current = value_dict_add(&json, "prepareSupportDefaultBehavior");
        *current       = to_json(&value->prepare_support_default_behavior.value);
    }
    
    if (value->honors_change_annotations.has_value) {
        current = value_dict_add(&json, "honorsChangeAnnotations");
        value_init_bool(current, value->honors_change_annotations.value);
    }
    
    return json;
}

VALUE to_json(DiagnosticTag* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json_array(DiagnosticTag* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        DiagnosticTag* current = values + i;
        VALUE* new_item        = value_array_append(&json);
        *new_item              = to_json(current);
    }
    
    return json;
}

VALUE to_json(DiagnosticTagArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(DiagnosticTagSupport* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "valueSet");
    *current       = to_json(&value->value_set);
    
    return json;
}

VALUE to_json(PublishDiagnosticsClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->related_information.has_value) {
        current = value_dict_add(&json, "relatedInformation");
        value_init_bool(current, value->related_information.value);
    }
    
    if (value->tag_support.has_value) {
        VALUE* current = value_dict_add(&json, "tagSupport");
        *current       = to_json(&value->tag_support.value);
    }
    
    if (value->version_support.has_value) {
        current = value_dict_add(&json, "versionSupport");
        value_init_bool(current, value->version_support.value);
    }
    
    if (value->code_description_support.has_value) {
        current = value_dict_add(&json, "codeDescriptionSupport");
        value_init_bool(current, value->code_description_support.value);
    }
    
    if (value->data_support.has_value) {
        current = value_dict_add(&json, "dataSupport");
        value_init_bool(current, value->data_support.value);
    }
    
    return json;
}

VALUE to_json(FoldingRangeClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    if (value->range_limit.has_value) {
        VALUE* current = value_dict_add(&json, "rangeLimit");
        value_init_uint32(current, value->range_limit.value);
    }
    
    if (value->line_folding_only.has_value) {
        current = value_dict_add(&json, "lineFoldingOnly");
        value_init_bool(current, value->line_folding_only.value);
    }
    
    return json;
}

VALUE to_json(SelectionRangeClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(LinkedEditingRangeClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(CallHierarchyClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(RequestRangeDictClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    return json;
}

VALUE to_json(RequestRangeClientCapabilities* value) {
    VALUE json = {0};
    
    if (value->type.value == RequestRangeClientCapabilitiesType_Boolean) {
        value_init_bool(&json, value->boolean);
    } else if (value->type.value == RequestRangeClientCapabilitiesType_Dict) {
        json = to_json(&value->dict);
    }
    
    return json;
}

VALUE to_json(RequestFullClientCapabilitiesType* value) {
    VALUE json = {0};
    
    value_init_uint32(&json, value->value);
    
    return json;
}

VALUE to_json(RequestFullDictClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->delta.has_value) {
        current = value_dict_add(&json, "delta");
        value_init_bool(current, value->delta.value);
    }
    
    return json;
}

VALUE to_json(RequestFullClientCapabilities* value) {
    VALUE json = {0};
    
    if (value->type.value == RequestFullClientCapabilitiesType_Boolean) {
        value_init_bool(&json, value->boolean);
    } else if (value->type.value == RequestFullClientCapabilitiesType_Dict) {
        json = to_json(&value->dict);
    }
    
    return json;
}

VALUE to_json(RequestClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->range.has_value) {
        VALUE* current = value_dict_add(&json, "range");
        *current       = to_json(&value->range.value);
    }
    
    if (value->full.has_value) {
        VALUE* current = value_dict_add(&json, "full");
        *current       = to_json(&value->full.value);
    }
    
    return json;
}

VALUE to_json(TokenFormat* value) {
    ConstStringU8* str = token_format_str + value->value;
    return to_json(str);
}

VALUE to_json_array(TokenFormat* values, u64 count) {
    VALUE json = {0};
    
    value_init_array(&json);
    
    for (u64 i = 0; i < count; i++) {
        TokenFormat* current = values + i;
        VALUE* new_item      = value_array_append(&json);
        *new_item            = to_json(current);
    }
    
    return json;
}

VALUE to_json(TokenFormatArray* value) { return to_json_array(value->values, value->count); }

VALUE to_json(SemanticTokensClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    current  = value_dict_add(&json, "requests");
    *current = to_json(&value->requests);
    
    current  = value_dict_add(&json, "tokenTypes");
    *current = to_json(&value->token_types);
    
    current  = value_dict_add(&json, "tokenModifiers");
    *current = to_json(&value->token_modifiers);
    
    current  = value_dict_add(&json, "formats");
    *current = to_json(&value->formats);
    
    if (value->overlapping_token_support.has_value) {
        current = value_dict_add(&json, "overlappingTokenSupport");
        value_init_bool(current, value->overlapping_token_support.value);
    }
    
    if (value->multiline_token_support.has_value) {
        current = value_dict_add(&json, "multilineTokenSupport");
        value_init_bool(current, value->multiline_token_support.value);
    }
    
    if (value->server_cancel_support.has_value) {
        current = value_dict_add(&json, "serverCancelSupport");
        value_init_bool(current, value->server_cancel_support.value);
    }
    
    if (value->augment_syntax_support.has_value) {
        current = value_dict_add(&json, "augmentSyntaxSupport");
        value_init_bool(current, value->augment_syntax_support.value);
    }
    
    return json;
}

VALUE to_json(MonikerClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->dynamic_registration.has_value) {
        current = value_dict_add(&json, "dynamicRegistration");
        value_init_bool(current, value->dynamic_registration.value);
    }
    
    return json;
}

VALUE to_json(TextDocumentClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->synchronization.has_value) {
        current  = value_dict_add(&json, "synchronization");
        *current = to_json(&value->synchronization.value);
    }
    
    if (value->completion.has_value) {
        current  = value_dict_add(&json, "completion");
        *current = to_json(&value->completion.value);
    }
    
    if (value->hover.has_value) {
        current  = value_dict_add(&json, "hover");
        *current = to_json(&value->hover.value);
    }
    
    if (value->signature_help.has_value) {
        current  = value_dict_add(&json, "signatureHelp");
        *current = to_json(&value->signature_help.value);
    }
    
    if (value->declaration.has_value) {
        current  = value_dict_add(&json, "declaration");
        *current = to_json(&value->declaration.value);
    }
    
    if (value->definition.has_value) {
        current  = value_dict_add(&json, "definition");
        *current = to_json(&value->definition.value);
    }
    
    if (value->type_definition.has_value) {
        current  = value_dict_add(&json, "typeDefinition");
        *current = to_json(&value->type_definition.value);
    }
    
    if (value->implementation.has_value) {
        current  = value_dict_add(&json, "implementation");
        *current = to_json(&value->implementation.value);
    }
    
    if (value->references.has_value) {
        current  = value_dict_add(&json, "references");
        *current = to_json(&value->references.value);
    }
    
    if (value->document_highlight.has_value) {
        current  = value_dict_add(&json, "documentHighlight");
        *current = to_json(&value->document_highlight.value);
    }
    
    if (value->document_symbol.has_value) {
        current  = value_dict_add(&json, "documentSymbol");
        *current = to_json(&value->document_symbol.value);
    }
    
    if (value->code_action.has_value) {
        current  = value_dict_add(&json, "codeAction");
        *current = to_json(&value->code_action.value);
    }
    
    if (value->code_lens.has_value) {
        current  = value_dict_add(&json, "codeLens");
        *current = to_json(&value->code_lens.value);
    }
    
    if (value->document_link.has_value) {
        current  = value_dict_add(&json, "documentLink");
        *current = to_json(&value->document_link.value);
    }
    
    if (value->color_provider.has_value) {
        current  = value_dict_add(&json, "colorProvider");
        *current = to_json(&value->color_provider.value);
    }
    
    if (value->formatting.has_value) {
        current  = value_dict_add(&json, "formatting");
        *current = to_json(&value->formatting.value);
    }
    
    if (value->range_formatting.has_value) {
        current  = value_dict_add(&json, "rangeFormatting");
        *current = to_json(&value->range_formatting.value);
    }
    
    if (value->on_type_formatting.has_value) {
        current  = value_dict_add(&json, "onTypeFormatting");
        *current = to_json(&value->on_type_formatting.value);
    }
    
    if (value->rename.has_value) {
        current  = value_dict_add(&json, "rename");
        *current = to_json(&value->rename.value);
    }
    
    if (value->publish_diagnostics.has_value) {
        current  = value_dict_add(&json, "publishDiagnostics");
        *current = to_json(&value->publish_diagnostics.value);
    }
    
    if (value->folding_range.has_value) {
        current  = value_dict_add(&json, "foldingRange");
        *current = to_json(&value->folding_range.value);
    }
    
    if (value->selection_range.has_value) {
        current  = value_dict_add(&json, "selectionRange");
        *current = to_json(&value->selection_range.value);
    }
    
    if (value->linked_editing_range.has_value) {
        current  = value_dict_add(&json, "linkedEditingRange");
        *current = to_json(&value->linked_editing_range.value);
    }
    
    if (value->call_hierarchy.has_value) {
        current  = value_dict_add(&json, "callHierarchy");
        *current = to_json(&value->call_hierarchy.value);
    }
    
    if (value->semantic_tokens.has_value) {
        current  = value_dict_add(&json, "semanticTokens");
        *current = to_json(&value->semantic_tokens.value);
    }
    
    if (value->moniker.has_value) {
        current  = value_dict_add(&json, "moniker");
        *current = to_json(&value->moniker.value);
    }
    
    return json;
}

VALUE to_json(MessageActionItem* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->additional_properties_support.has_value) {
        current = value_dict_add(&json, "additionalPropertiesSupport");
        value_init_bool(current, value->additional_properties_support.value);
    }
    
    return json;
}

VALUE to_json(ShowMessageRequestClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->message_action_item.has_value) {
        current  = value_dict_add(&json, "messageActionItem");
        *current = to_json(&value->message_action_item.value);
    }
    
    return json;
}

VALUE to_json(ShowDocumentClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_bool(&json, value->support);
    
    return json;
}

VALUE to_json(WindowCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->work_done_progress.has_value) {
        current = value_dict_add(&json, "workDoneProgress");
        value_init_bool(current, value->work_done_progress.value);
    }
    
    if (value->show_message.has_value) {
        current  = value_dict_add(&json, "showMessage");
        *current = to_json(&value->show_message.value);
    }
    
    if (value->show_document.has_value) {
        current  = value_dict_add(&json, "showDocument");
        *current = to_json(&value->show_document.value);
    }
    
    return json;
}

VALUE to_json(RegularExpressionClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "engine");
    *current       = to_json(&value->engine);
    
    if (value->version.has_value) {
        current  = value_dict_add(&json, "version");
        *current = to_json(&value->version.value);
    }
    
    return json;
}

VALUE to_json(MarkdownClientCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = value_dict_add(&json, "parser");
    *current       = to_json(&value->parser);
    
    if (value->version.has_value) {
        current  = value_dict_add(&json, "version");
        *current = to_json(&value->version.value);
    }
    
    if (value->allowed_tags.has_value) {
        current  = value_dict_add(&json, "allowedTags");
        *current = to_json(&value->allowed_tags.value);
    }
    
    return json;
}

VALUE to_json(GeneralCapabilities* value) {
    VALUE json = {0};
    
    value_init_dict(&json);
    
    VALUE* current = 0;
    
    if (value->regular_expressions.has_value) {
        current  = value_dict_add(&json, "regularExpressions");
        *current = to_json(&value->regular_expressions.value);
    }
    
    if (value->markdown.has_value) {
        current  = value_dict_add(&json, "markdown");
        *current = to_json(&value->markdown.value);
    }
    
    return json;
}