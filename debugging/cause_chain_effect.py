# https://www.debuggingbook.org/html/DynamicInvariants.html

import sys 
import copy 


# The buggy program 
def remove_html_markup(s): 
    tag   = False 
    quote = False 
    out   = "" 
 
    for c in s: 
        if c == '<' and not quote: 
            tag = True 
        elif c == '>' and not quote: 
            tag = False 
        elif c == '"' or c == "'" and tag: 
            quote = not quote 
        elif not tag: 
            out = out + c 

    return out 


# The delta debugger    
def ddmin(s): 
    #assert test(s) == "FAIL" 
    n = 2     # Initial granularity 
    while len(s) >= 2: 
        start = 0 
        subset_length = int(len(s) / n) 
        some_complement_is_failing = False 
        while start < len(s): 
            complement = s[:start] + s[start + subset_length:] 
            if test(complement) == "FAIL": 
                s = complement 
                n = max(n - 1, 2) 
                some_complement_is_failing = True 
                break                 
            start += subset_length 

        if not some_complement_is_failing: 
            if len(s) == n: 
                break 
            n = min(n * 2, len(s)) 
    return s 


the_line      = None
the_iteration = None
the_state     = None
the_diff      = None 
the_input     = None 
 

def trace_fetch_state(frame, event, arg): 
    global the_line 
    global the_iteration 
    global the_state

    if event == "line":
        if frame.f_lineno == the_line:
            if not hasattr(trace_fetch_state, "counter"):
                trace_fetch_state.counter = 0
            trace_fetch_state.counter += 1

            if trace_fetch_state.counter == the_iteration:
                the_state = copy.deepcopy(frame.f_locals)
                sys.settrace(None)
                return None
    return trace_fetch_state 


def trace_apply_diff(frame, event, arg): 
    global the_line 
    global the_diff 
    global the_iteration 

    if event == "line":
        if frame.f_lineno == the_line:
            frame.f_locals.update(the_diff)
            sys.settrace(None)
            return None
    return trace_apply_diff 


def get_state(input, line, iteration): 
    global the_line 
    global the_iteration 
    global the_state 

    the_line      = line 
    the_iteration = iteration 
    
    sys.settrace(trace_fetch_state) 
    y = remove_html_markup(input) 
    sys.settrace(None) 

    return the_state 

 
def test(diffs): 
    global the_diff 
    global the_input 
    global the_line 
    global the_iteration 

    line = the_line 
    iteration = the_iteration 

    the_diff = diffs 
    sys.settrace(trace_apply_diff) 
    y = remove_html_markup(the_input) 
    sys.settrace(None) 

    the_line = line 
    the_iteration = iteration 

    if y.find('<') == -1: 
        return "PASS" 
    else: 
        return "FAIL" 
         
html_fail = '"<b>foo</b>"' 
html_pass = "'<b>foo</b>'" 

locations = [(8, 1), (14, 1), (14, 2), (14, 3), (23, 1)] 

 
def auto_cause_chain(locations): 
    global html_fail, html_pass, the_input, the_line, the_iteration, the_diff 
    print("The program was started with", repr(html_fail)) 

    for (line, iteration) in locations: 
        if hasattr(trace_fetch_state, "counter"):
            del trace_fetch_state.counter

        the_input = html_pass 
        state_pass = get_state(html_pass,line,iteration) 

        if hasattr(trace_fetch_state, "counter"):
            del trace_fetch_state.counter

        the_input = html_fail 
        state_fail = get_state(html_fail,line,iteration) 

        diffs = [] 
        for var in state_fail: 
            if var not in state_pass or state_fail[var] != state_pass.get(var): 
                diffs.append((var, state_fail[var])) 

        the_input = html_pass 
        the_line  = line 
        the_iteration  = iteration 
        cause = ddmin(diffs) 

        print ("Then, in Line " + repr(line) + " (iteration " + repr(iteration) + "),", end =" ") 
        for (var, value) in cause: 
            print (var, 'became', repr(value), end =" ") 
        print() 

    print("Then the program failed.") 
 
 
auto_cause_chain(locations) 