int ExecuteDoubleAndDoubleDouble(ScriptEnvironment *script_tasks, Script *subject_script, double (*callback) (double, double)) {
    StackData stack_data_0 = PopScriptStackData(subject_script);
    StackData stack_data_1 = PopScriptStackData(subject_script);
    if(!stack_data_0.error && !stack_data_1.error) {
        stack_data_0 = ExpandStackData(script_tasks, subject_script, stack_data_0);
        stack_data_1 = ExpandStackData(script_tasks, subject_script, stack_data_1);
        double d0 = CastToFloat64(stack_data_0);
        double d1 = CastToFloat64(stack_data_1);
        double result = callback(d0, d1);
        PushScriptStackData(subject_script, (byte*) &result, DoubleType);
    } else {
        PushScriptStackData(subject_script, 0, 0);
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int ExecuteDoubleAndDouble(ScriptEnvironment *script_tasks, Script *subject_script, double (*callback) (double)) {
    StackData stack_data_0 = PopScriptStackData(subject_script);
    if(!stack_data_0.error) {
        stack_data_0 = ExpandStackData(script_tasks, subject_script, stack_data_0);
        double d0 = CastToFloat64(stack_data_0);
        double result = callback(d0);
        PushScriptStackData(subject_script, (byte*) &result, DoubleType);
    } else {
        PushScriptStackData(subject_script, 0, 0);
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int ExecuteFloatAndFloat(ScriptEnvironment *script_tasks, Script *subject_script, float (*callback) (float)) {
    StackData stack_data_0 = PopScriptStackData(subject_script);
    if(!stack_data_0.error) {
        stack_data_0 = ExpandStackData(script_tasks, subject_script, stack_data_0);
        float d0 = CastToFloat32(stack_data_0);
        float result = callback(d0);
        PushScriptStackData(subject_script, (byte*) &result, FloatType);
    } else {
        PushScriptStackData(subject_script, 0, 0);
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int ExecuteLongAndLong(ScriptEnvironment *script_tasks, Script *subject_script, float128 (*callback) (float128)) {
    StackData stack_data_0 = PopScriptStackData(subject_script);
    if(!stack_data_0.error) {
        stack_data_0 = ExpandStackData(script_tasks, subject_script, stack_data_0);
        float128 d0 = CastToFloat128(stack_data_0);
        float128 result = callback(d0);
        PushScriptStackData(subject_script, (byte*) &result, LongDoubleType);
    } else {
        PushScriptStackData(subject_script, 0, 0);
    }
    return PushScriptStackData(subject_script, 0, 0);
}

int StandardInterruptCall(ScriptEnvironment *script_tasks, Script *subject_script) {
    StackData stack_data = PopScriptStackData(subject_script);
    uint64 interrupt_code = CastToUnsignedInt64(stack_data);
    switch(interrupt_code) {
        case 1:
            return ExecuteDoubleAndDoubleDouble(script_tasks, subject_script, pow);
        case 2:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, sqrt);
        case 3:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, sin);
        case 4:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, cos);
        case 5:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, tan);
        case 6:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, log);
        case 7:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, exp);
        case 8:
            return ExecuteFloatAndFloat(script_tasks, subject_script, cbrtf);
        case 9:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, cbrt);
        case 10:
            return ExecuteLongAndLong(script_tasks, subject_script, cbrtl);
        case 11:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, floor);
        case 12:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, ceil);
        case 13:
            return ExecuteDoubleAndDouble(script_tasks, subject_script, round);
    }
    return PushScriptStackData(subject_script, 0, 0);
}