# https://www.debuggingbook.org/html/Tracer.html

import sys 

def remove_html_markup(s): 
    tag = False 
    quote = False 
    out = "" 

    for c in s: 
        assert tag or not quote 
        if c == '<' and not quote:    # start of markup 
            tag = True 
        elif c == '>' and not quote:  # end of markup 
            tag = False 
        elif (c =='"' or c=="'") and tag: 
            quote = not quote 
        elif not tag: 
            out = out + c 
  
    return out 


stepping = False 
breakpoints = {9:True,14:True} 
watchpoints = {}
last_values = {}


def debug(command, my_arg, my_locals): 
    global stepping, breakpoints, watchpoints, last_values

    # split command into parts for multiple p inputs
    parts = command.split()
    cmd = parts[0] if parts else ""
    args = parts[1:] if len(parts) > 1 else []

    if cmd == 's':      # step 
        stepping = True 
        return True
    
    elif cmd == 'c':    # continue 
        stepping = False 
        return True
    
    elif cmd == 'q':    # quit
        sys.exit(0)

    elif cmd == 'p':    # print
        if not args:  
            for name, value in my_locals.items():
                print(f"{name} = {repr(value)}")
        else:  
            for name in args:
                if name in my_locals:
                    print(f"{name} = {repr(my_locals[name])}")
                else:
                    print(f"Error: '{name}' not found")
        return False
    
    elif cmd == 'b':    # breakpoint
        if not args:
            print("Current breakpoints:", list(breakpoints.keys()))
        else:
            try:
                line_no = int(args[0])
                breakpoints[line_no] = True
                print(f"Breakpoint added at line {line_no}")
            except (ValueError, IndexError):
                print("Invalid line number. Usage: b <line_number>")
        return False
    
    elif cmd == 'w':    # watchpoint
        if not args:
            print("Current watchpoints:", list(watchpoints.keys()))
        else:
            var_name = args[0]
            if var_name in my_locals:
                watchpoints[var_name] = my_locals[var_name]
                print(f"Watchpoint added for variable '{var_name}'")
            else:
                print(f"Variable '{var_name}' not found in current scope")
        return False
    
    else:
        print("Unknown command", repr(command))
        return False


def input_command(): 
    command = input("(my-debugger) ") 
    return command 


def traceit(frame, event, arg): 
    global stepping, breakpoints, watchpoints, last_values   
    
    if event == "line": 
        current_line = frame.f_lineno
        current_locals = frame.f_locals
        should_break = False
        
        if current_line in breakpoints:
            print(f"Breakpoint at line {current_line}")
            should_break = True
            
        watch_triggered = False
        for var_name, last_value in watchpoints.items():
            if var_name in current_locals:
                if current_locals[var_name] != last_value:
                    print(f"Watchpoint triggered: {var_name} changed from {repr(last_value)} to {repr(current_locals[var_name])}")
                    watch_triggered = True
                    watchpoints[var_name] = current_locals[var_name]
        
        if stepping or should_break or watch_triggered:
            resume = False
            while not resume:
                print(event, current_line, frame.f_code.co_name, frame.f_locals) 
                command = input_command()
                resume = debug(command, arg, frame.f_locals)
    return traceit 


def remove_html_markup_traced(s): 
    sys.settrace(traceit) 
    ret = remove_html_markup(s) 
    sys.settrace(None) 
    return ret 


ret = remove_html_markup_traced('xyz') 
print(ret)
