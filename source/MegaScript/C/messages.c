extern char *GetErrorMessage(int error_code) {
    switch(error_code) {
        case FailedToCreateASMFunctionError:
            return "Failed to create the ASM-function.";
        case CommasCannotBeUsedHereError:
            return "Commas cannot be used here.";
        case InvalidOperatorSyntaxError:
            return "Invalid operator syntax.";
        case InvalidSyntaxError:
            return "Invalid syntax.";
        case AttemptedToGetAddressOfObjectError:
            return "Getting the address of an object in virtual mode is forbidden for security reasons.";
        case AttemptedToAllocateZeroError:
            return "Attempted to allocate zero bytes of memory.";
        case ListPointerDoesNotMatchError:
            return "The list pointers do not match.";
        case ListPointerNotFoundError:
            return "Could not find the list pointer.";
        case AttemptedToAssignNonObjectToObjectError:
            return "Attempted to assign a non object to an object variable.";
        case ObjectInStructError:
            return "Objects cannot be added to structs for security reasons.";
        case MegaScript32BitVersionError:
            return "This version of MegaScript is 32-bit.";
        case Unsupported128BitFloatError:
            return "This CPU does not support 128-bit floats.";
        case Unsupported64BitFloatError:
            return "This CPU does not support 64-bit floats.";
        case Unsupported32BitFloatError:
            return "This CPU does not support 32-bit floats.";
        case Unsupported16BitFloatError:
            return "This CPU does not support 16-bit floats.";
        case Unsupported8BitFloatError:
            return "This CPU does not support 8-bit floats.";
        case Unsupported128BitIntError:
            return "This CPU does not support 128-bit integers.";
        case Unsupported64BitIntError:
            return "This CPU does not support 64-bit integers.";
        case Unsupported32BitIntError:
            return "This CPU does not support 32-bit integers.";
        case Unsupported16BitIntError:
            return "This CPU does not support 16-bit integers.";
        case Unsupported8BitIntError:
            return "This CPU does not support 8-bit integers.";
        case FailedToFindClassError:
            return "Failed to find the class by name.";
        case FailedToPopStringError:
            return "Failed to pop a string off the stack.";
        case PrivilegeLevelError:
            return "Insufficient privilege level.";
        case CouldNotFindClassTypedefError:
            return "Could not find the class data type by name.";
        case AttemptedToSetObjectParentToNonObjectError:
            return "Attempted to set object parent to a non object.";
        case EnvironmentRootNotFoundError:
            return "The environment root was not found.";
        case ObjectCallbackError:
            return "Failed to execute the object callback.";
        case NotAnObjectCallbackError:
            return "This is not an object callback.";
        case MissingClosingParenthesisOnCallbackError:
            return "Missing closing parenthesis on element callback.";
        case AttemptToAssignNonStructToStructError:
            return "Attempted to assign a non-struct to a struct.";
        case ObjectDoesNotExistError:
            return "Object does not exist.";
        case InvalidObjectClassError:
            return "Invalid object class.";
        case NotABasicDataTypeError:
            return "This is not a basic data type.";
        case FailedToGetStructDataError:
            return "Failed to get the structure data.";
        case CouldNotFindStructListPointerError:
            return "Could not find the structure list pointer.";
        case FailedToGetStructSizeError:
            return "Failed to get the recursive struct size.";
        case StructElementNotFoundError:
            return "Structure element could not be found.";
        case TypedefNotFoundError:
            return "The typedef for the new variable was not found.";
        case StructureAutoIncrementDesyncedError:
            return "Structure auto increment desynced.";
        case InitializedArrayInStructError:
            return "Cannot initialize arrays inside structure definition.";
        case FailedToSetTheStructAuxilaryPointerError:
            return "Failed to set the struct auxilary pointer.";
        case FailedToSetTheStructDataTypesPointerError:
            return "Failed to set the struct data types pointer.";
        case FailedToSetTheStructElementsPointerError:
            return "Failed to set the struct element names pointer.";
        case FailedToSetTheStructMemberCountError:
            return "Failed to set the struct member count.";
        case FailedToAllocateStructListMemoryError:
            return "Failed to allocate memory for the struct list.";
        case ImproperUsageOfNewError:
            return "Improper usage of new.";
        case FailedToPopStructError:
            return "Failed to pop the struct off the stack.";
        case RecursiveStructListPointerNotFoundError:
            return "The recursive struct child pointer was not found.";
        case StructListCouldNotBeFoundError:
            return "The structure list could not be found.";
        case NotAStructError:
            return "This data is not a structure.";
        case MultiDimensionalArraysNotYetAddedError:
            return "Multi-dimensional arrays are not added in this update.";
        case ImproperUsageOfCommaError:
            return "Improper usage of comma.";
        case StoppedAtCommaWithBracketError:
            return "Stopped at comma with bracket.";
        case StoppedAtCommaError:
            return "Stopped at comma.";
        case NotACLikePointerError:
            return "This is not a C-like pointer.";
        case NotAnArrayError:
            return "This is not an array.";
        case MissingArrayIndexClosingBracketError:
            return "Missing array index closing bracket.";
        case FailedToAllocateMemoryError:
            return "Failed to allocate memory.";
        case FailedToCreateDebrisError:
            return "Failed to create debris.";
        case FailedToFindDebrisByIdError:
            return "Failed to find the debris by id.";
        case NotASymbolOrDebrisError:
            return "This data is not a symbol or a debris item.";
        case FailedToFindDebrisInDereferenceError:
            return "Failed to find the debris by id while dereferencing.";
        case FailedToReadPointerError:
            return "Failed to read the pointer.";
        case PointerDimensionZeroError:
            return "The pointer dimension is already 0.";
        case ArrayPointerAssigningError:
            return "Failed to assign the pointer to the array.";
        case ArrayMemoryAllocationFailedError:
            return "Failed to allocate memory for the array.";
        case AttemptedArrayDataSizeZeroError:
            return "Attempted to make an array of a data type whose size is 0.";
        case CPointerZeroingError:
            return "Failed to zero the new c-like pointer.";
        case MissingEndingArrayInitializerBracketError:
            return "Missing ending bracket on array initializer.";
        case ComplexOperationPerformedInArrayInitializerError:
            return "Attempted to perform a complex operation inside an array initializer.";
        case ArrayInitializerMemoryReadError:
            return "Failed to read the array initializer.";
        case InvalidExecutionBytecodeError:
            return "Attempted to execute an invalid bytecode.";
        case ArraySizeInitializationError:
            return "Failed to initialize the array size.";
        case BoolPointerError:
            return "Unable to dereference the bool pointer.";
        case DebrisNotFoundError:
            return "Temporary variable not found.";
        case UnsupportedNewDataTypeError:
            return "The new function does not support this data type.";
        case TooManyBoolElementsError:
            return "Too many elements were added to this bool.";
        case NewMissingDataTypeError:
            return "Improper usage of new.";
        case PreprocessorDefinitionError:
            return "Failed to create this preprocessor definition.";
        case RequiredStandardScriptError:
            return "Attempted to require a regular script.";
        case ScriptNameNotFoundError:
            return "Could not find the script by name.";
        case UnableToCreateStringError:
            return "Failed to create the string.";
        case MemoryReadError:
            return "Failed to read the memory.";
        case DataNotAStringError:
            return "The passed data is not a string.";
        case ScriptIsReadOnlyError:
            return "This memory is read only.";
        case UnsupportedStackDataTypeError:
            return "Attempted to write an unsupported stack data type to memory.";
        case MissingStackDataError:
            return "Stack data pointer is null.";
        case KeywordPrecursorPermissionError:
            return "This script does not have enough privilege to create keywords.";
        case EnumListReadFailureError:
            return "Enum list read failure.";
        case InvalidElementTypeError:
            return "Invalid element type.";
        case NonExistentEnumMemberError:
            return "Non existent enum member.";
        case EnumElementBytecodeOutsideError:
            return "Enum element created outside of an enum.";
        case EnumBracketMismatchError:
            return "Incomplete bracket pair in enum.";
        case FunctionCallInsideEnumError:
            return "Attempted to parse a function call inside an enum.";
        case EnumDefiningError:
            return "Failed to make the enum.";
        case CastedWithNonDataTypeError:
            return "Attempted to cast to a non data type.";
        case LambdaFunctionDefiningError:
            return "Failed to make the lamda function.";
        case AttemptedModificationOfConstantError:
            return "Attempted to modify a constant.";
        case CouldNotWriteMemoryError:
            return "Could not write to the memory.";
        case CouldNotReadMemoryError:
            return "Could not read the memory.";
        case CouldNotFindTheSymbolError:
            return "Could not find this symbol in the symbol table.";
        case AttemptedSymbolSettingsChangeOnNonSymbolError:
            return "Attempted to change symbol settings of a non-symbol.";
        case MissingStandardCallbackError:
            return "Standard call is missing its callback.";
        case StandardCallIdDoesNotExistError:
            return "This standard call id does not exist.";
        case StandardCallIdNotFoundError:
            return "Unable to decode the standard call id.";
        case InvalidParsingCommandError:
            return "Invalid pre-parsing command.";
        case AssignedNonSymbolError:
            return "Attempted to assign to a non-symbol.";
        case InvalidOperatorTypeError:
            return "Invalid operator type.";
        case DecrementedNonSymbolError:
            return "Attempted to decrement a non-symbol.";
        case IncrementedNonSymbolError:
            return "Attempted to increment a non-symbol.";
        case InvalidStackDataInReturnError:
            return "Invalid stack data in return call loop.";
        case BreakOutsideOfLoopError:
            return "Attempted to break outside of a loop.";
        case CannotGoHigherError:
            return "Attempted to enter the parent of the root scope.";
        case ContinueOutsideOfLoopError:
            return "Attempted to continue outside of a loop.";
        case ForLoopSaveError:
            return "Failed to find the loop save on the stack.";
        case StandardCallPrivilegeError:
            return "Privilege level error.";
        case BytecodeNotFoundError:
            return "A bytecode was not found in the bytecodes list.";
        case TooFewStandardCallArgumentsError:
            return "There were too few arguments passed to this standard call.";
        case TooManyStandardCallArgumentsError:
            return "There were too many arguments passed to this standard call.";
        case WhileLoopFailureError:
            return "Failed to execute the while loop.";
        case IfStatementFailureError:
            return "Failed to execute the if statement.";
        case CouldNotDeleteSymbolError:
            return "Failed to delete a symbol.";
        case InvalidArgumentError:
            return "Invalid argument type.";
        case StringGenerationError:
            return "String generation error.";
        case MissingParenthesisError:
            return "The statement is missing its parenthesis.";
        case LabelMemoryReadFailureError:
            return "Failed to read the name of this label.";
        case SymbolMemoryReadFailureError:
            return "Failed to read the name of this symbol.";
        case ZeroOperatorError:
            return "Attempted to use 0 as an operator. (Is the memory owned?)";
        case UnopenedFileError:
            return "The file could not be opened.";
        case CouldNotCreateScopeError:
            return "Failed to create the scope.";
        case CouldNotFindScopeError:
            return "Failed to find the scope.";
        case CouldNotDeleteScopeError:
            return "Failed to delete the scope.";
        case NotAFunctionError:
            return "Attempted to call a non function.";
        case ComplexOperationUsedError:
            return "Attempted to use a complex operation in function argument definition.";
        case FunctionCallValuesScrambledError:
            return "The function call stack values were scrambled.";
        case MissingOperationSaveError:
            return "The operation save could not be found on the stack.";
        case FunctionReturnError:
            return "Failed to return from the function.";
        case OperationReturnError:
            return "Failed to return from the operation.";
        case TooManyOperatorsError:
            return "Too many operators are in the queue.";
        case MissingFunctionDefineBracketError:
            return "Missing parenthisis on function name.";
        case UnexpectedEndOfScriptError:
            return "Unexpected end of script.";
        case ScriptTimeoutError:
            return "Script was disabled due to timeout.";
        case FunctionArgumentsIncorrectlyPassedError:
            return "The function call arguments were passed incorrectly.";
        case NotAPointerError:
            return "The passed data is not a pointer";
        case LabelParsingError:
            return "Label was not parsed correctly.";
        case LabelNotDefinedError:
            return "Label not defined.";
        case InvalidJumpDestinationError:
            return "Attempted to jump to an invalid address.";
        case NullOpcodeExecutionError:
            return "Attempted to execute zero. (Memory not owned by this script?)";
        case AttemptedGetAddressOfImmediateError:
            return "Attempted to get address of an immediate value.";
        case FailedToEnableScriptError:
            return "Failed to enable script.";
        case StackEmptyError:
            return "Attempted to pop data off an empty stack.";
        case VariableMemoryAssignmentFailedError:
            return "Failed to assign a variable.";
        case SymbolNotFoundError:
            return "A variable/function was not found.";
        case SymbolDefiningError:
            return "Failed to read the variable/function name.";
        case InternalMemoryAllocationError:
            return "The internal engine failed to allocate some memory. (Sorry!)";
        case InvalidOperationError:
            return "Attempted to perform an illegal operation.";
        case FloatingPointsModulatedError:
            return "Attempted to get the remainder of a floating point.";
        case StandardDataTypeRedefinitionError:
            return "Attempted to redefine a standard data type.";
        case StandardCallRedefinitionError:
            return "Attempted to redefine a standard function.";
        case InvalidDataTypeError:
            return "The data type is invalid.";
        case MissingParametersError:
            return "Too few arguments were passed.";
        case ExecutionFaultError:
            return "Execution fault.";
        case StackOverflowError:
            return "Stack overflow!";
        case AddressOutOfBoundsError:
            return "The referenced address is out of the allocated memory's bounds.";
        case WriteFailureError:
            return "Failed to write to the specified memory address.";
        case ScriptCodeNotWrittenError:
            return "Failed to load script code into memory.";
        case MemoryNotOwnedError:
            return "This memory section is being used by a memory protected script.";
        case NotEnoughMemoryError:
            return "Out of memory.";
        case MemorySectionNotFoundError:
            return "The memory section was not found.";
        case NoMemorySectionsError:
            return "There are currently no open memory sections.";
        case MemorySectionOccupiedError:
            return "This memory section is occupied.";
        case MissingOperandError:
            return "Operand is missing.";
        case NotANumberError:
            return "Invalid number.";
        case TemporaryNumberSizeError:
            return "Number size error. (This error will be patched soon)";
        case InvalidDigitError:
            return "Invalid digit.";
        case UnknownOperatorError:
            return "Unknown operator.";
        case UndefinedFunctionError:
            return "Function is undefined.";
        case UnfinishedStringError:
            return "Missing closing string quote.";
        case MissingCallEndBracketError:
            return "Incomplete parenthesis.";
        case MissingVariableNameError:
            return "The variable name was not entered correctly.";
        case TakenLabelError:
            return "This label is already in use.";
        case PrefixUsedOnOpcodeError:
            return "Prefixes cannot be used on standard calls.";
        case BitSizeError:
            return "Architecture must be either 32 bit or 64 bit.";
        case UndefinedVariableError:
            return "One or more variables are undefined.";
        case InvalidScopeError:
            return "The scope is invalid.";
        case GenericError:
            return "An error has occured.";
        default:
            return "Unknown error.";
    }
}

extern char *GetErrorModeTitle(int error_mode) {
    switch(error_mode) {
        case ExecutionErrorMode:
            return "[EXECUTION ERROR]";
        case ParsingErrorMode:
            return "[PARSING ERROR]";
        case EnvironmentErrorMode:
            return "[ENVIRONMENT ERROR]";
        default:
            return "[UNKNOWN ERROR]";
    }
}

